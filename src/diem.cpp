#include "diem.h"
#include "loptc.h"
#include "dangky.h"
#include "sinhvien.h"
#include "lop.h"
#include "monhoc.h"
#include "utils.h"
#include "file_io.h"
#include <iostream>
#include <iomanip>
#include <string.h>

using namespace std;

// ====================================================================
// THUẬT TOÁN SẮP XẾP NỘI BỘ (KHÔNG DÙNG STL SORT THEO RULE 1 & 7)
// Sử dụng Insertion Sort (ổn định, tối ưu cho mảng kích thước <= 1000)
// Big-O: Tốt nhất O(N), Xấu nhất O(N^2), Bộ nhớ phụ O(1)
// ====================================================================

// Sắp xếp danh sách SV có điểm theo thứ tự Tên -> Họ -> Mã SV tăng dần
static void SapXepSinhVienDiem(SinhVienDiem ds[], int n) {
    for (int i = 1; i < n; i++) {
        SinhVienDiem key = ds[i];
        int j = i - 1;
        while (j >= 0) {
            int cmpTen = stricmp(ds[j].TEN, key.TEN);
            bool doiCho = false;
            if (cmpTen > 0) {
                doiCho = true;
            } else if (cmpTen == 0) {
                int cmpHo = stricmp(ds[j].HO, key.HO);
                if (cmpHo > 0) {
                    doiCho = true;
                } else if (cmpHo == 0) {
                    if (stricmp(ds[j].MASV, key.MASV) > 0) {
                        doiCho = true;
                    }
                }
            }

            if (doiCho) {
                ds[j + 1] = ds[j];
                j--;
            } else {
                break;
            }
        }
        ds[j + 1] = key;
    }
}

// Sắp xếp danh sách SV điểm trung bình theo thứ tự Tên -> Họ -> Mã SV tăng dần
static void SapXepSinhVienDiemTB(SinhVienDiemTB ds[], int n) {
    for (int i = 1; i < n; i++) {
        SinhVienDiemTB key = ds[i];
        int j = i - 1;
        while (j >= 0) {
            int cmpTen = stricmp(ds[j].TEN, key.TEN);
            bool doiCho = false;
            if (cmpTen > 0) {
                doiCho = true;
            } else if (cmpTen == 0) {
                int cmpHo = stricmp(ds[j].HO, key.HO);
                if (cmpHo > 0) {
                    doiCho = true;
                } else if (cmpHo == 0) {
                    if (stricmp(ds[j].MASV, key.MASV) > 0) {
                        doiCho = true;
                    }
                }
            }

            if (doiCho) {
                ds[j + 1] = ds[j];
                j--;
            } else {
                break;
            }
        }
        ds[j + 1] = key;
    }
}

// Sắp xếp danh sách mã môn học theo alphabet tăng dần
static void SapXepMaMonHoc(char dsMAMH[][11], char dsTENMH[][51], int n) {
    for (int i = 1; i < n; i++) {
        char keyMa[11];
        char keyTen[51];
        strcpy(keyMa, dsMAMH[i]);
        strcpy(keyTen, dsTENMH[i]);

        int j = i - 1;
        while (j >= 0 && stricmp(dsMAMH[j], keyMa) > 0) {
            strcpy(dsMAMH[j + 1], dsMAMH[j]);
            strcpy(dsTENMH[j + 1], dsTENMH[j]);
            j--;
        }
        strcpy(dsMAMH[j + 1], keyMa);
        strcpy(dsTENMH[j + 1], keyTen);
    }
}


// ====================================================================
// CÂU I: NHẬP ĐIỂM CHO LỚP TÍN CHỈ
// ====================================================================

// Lấy danh sách SV đăng ký của lớp tín chỉ (lọc SV chưa hủy và sắp xếp Tên+Họ)
// Big-O: O(K_DK * N_Lop * N_SV + K_DK^2)
int LayDSSVDangKyLTC(const DS_LopTC &dsltc, const DS_LOPSV &dslop, int maLopTC, SinhVienDiem dsSV[], int maxSV) {
    int idx = TimLopTCTheoMa(dsltc, maLopTC);
    if (idx == -1) return -1; // Không tìm thấy lớp tín chỉ

    LopTinChi* ltc = dsltc.nodes[idx];
    if (ltc == nullptr || ltc->HuyLop) return -2; // Lớp đã bị hủy

    int count = 0;
    for (PTR_DangKy p = ltc->FirstDK; p != nullptr; p = p->next) {
        if (p->dk.HuyDangKy) continue; // Bỏ qua sinh viên đã hủy đăng ký
        if (count >= maxSV) break;

        strcpy(dsSV[count].MASV, p->dk.MASV);
        dsSV[count].DIEM = p->dk.DIEM;

        // Tra cứu Họ và Tên từ danh sách lớp sinh viên toàn trường
        LOPSV* lopChuaSV = nullptr;
        PTRSV pSV = TimSVToanTruong(dslop, p->dk.MASV, lopChuaSV);
        if (pSV != nullptr) {
            strcpy(dsSV[count].HO, pSV->sv.HO);
            strcpy(dsSV[count].TEN, pSV->sv.TEN);
        } else {
            strcpy(dsSV[count].HO, "(Khong ro)");
            strcpy(dsSV[count].TEN, "");
        }
        count++;
    }

    // Sắp xếp sinh viên theo thứ tự Tên + Họ tăng dần
    SapXepSinhVienDiem(dsSV, count);
    return count;
}

// Lấy danh sách SV đăng ký qua tổ hợp thông tin lớp tín chỉ
int LayDSSVDangKyLTCTheoThongTin(const DS_LopTC &dsltc, const DS_LOPSV &dslop,
                                const char nienKhoa[], int hocKy, const char maMH[], int nhom,
                                SinhVienDiem dsSV[], int maxSV, int &maLopTCTimThay) {
    maLopTCTimThay = -1;
    char nkChuan[15];
    char mhChuan[15];
    strcpy(nkChuan, nienKhoa);
    strcpy(mhChuan, maMH);
    ChuanHoaMa(nkChuan);
    ChuanHoaMa(mhChuan);

    int idx = TimLopTCTrung(dsltc, nkChuan, hocKy, mhChuan, nhom);
    if (idx == -1) return -1;

    LopTinChi* ltc = dsltc.nodes[idx];
    if (ltc == nullptr || ltc->HuyLop) return -2;

    maLopTCTimThay = ltc->MALOPTC;
    return LayDSSVDangKyLTC(dsltc, dslop, maLopTCTimThay, dsSV, maxSV);
}

// Cập nhật điểm cho 1 sinh viên trong lớp tín chỉ
// Big-O: O(K_DK)
bool CapNhatDiemSinhVien(DS_LopTC &dsltc, int maLopTC, const char maSV[], float diemMoi) {
    if (diemMoi < 0.0f || diemMoi > 10.0f) {
        return false;
    }

    int idx = TimLopTCTheoMa(dsltc, maLopTC);
    if (idx == -1) return false;

    LopTinChi* ltc = dsltc.nodes[idx];
    if (ltc == nullptr || ltc->HuyLop) return false;

    PTR_DangKy p = TimDangKy(ltc->FirstDK, maSV);
    if (p == nullptr || p->dk.HuyDangKy) return false;

    p->dk.DIEM = diemMoi;
    return true;
}

// Giao diện nhập điểm trực tiếp trên Console (Câu i)
void NhapDiemLTC_Console(DS_LopTC &dsltc, const DS_LOPSV &dslop, TreeMonHoc rootMH) {
    cout << "\n=======================================================\n";
    cout << "             NHAP DIEM LOP TIN CHI (CAU I)             \n";
    cout << "=======================================================\n";

    char nienKhoa[15];
    int hocKy = 0;
    char maMH[15];
    int nhom = 0;

    cout << "Nhap Nien khoa (vi du: 2025-2026): ";
    cin.getline(nienKhoa, sizeof(nienKhoa));
    ChuanHoaMa(nienKhoa);
    if (!KiemTraNienKhoaHopLe(nienKhoa)) {
        cout << "[!] Nien khoa khong hop le (Dinh dang YYYY-YYYY)!\n";
        return;
    }

    hocKy = NhapSoNguyen("Nhap Hoc ky (1..3): ");
    if (hocKy < 1 || hocKy > 3) {
        cout << "[!] Hoc ky khong hop le!\n";
        return;
    }

    cout << "Nhap Ma mon hoc: ";
    cin.getline(maMH, sizeof(maMH));
    ChuanHoaMa(maMH);

    TreeMonHoc nodeMH = TimMonHoc(rootMH, maMH);
    if (nodeMH == nullptr) {
        cout << "[!] Ma mon hoc '" << maMH << "' khong ton tai tren he thong!\n";
        return;
    }

    nhom = NhapSoNguyen("Nhap Nhom lop: ");
    if (nhom <= 0) {
        cout << "[!] Nhom lop phai la so nguyen duong!\n";
        return;
    }

    int maLopTC = -1;
    SinhVienDiem dsSV[MAX_SV_DANGKY];
    int soLuong = LayDSSVDangKyLTCTheoThongTin(dsltc, dslop, nienKhoa, hocKy, maMH, nhom, dsSV, MAX_SV_DANGKY, maLopTC);

    if (soLuong == -1) {
        cout << "[!] Khong tim thay lop tin chi voi thong tin da nhap!\n";
        return;
    }
    if (soLuong == -2) {
        cout << "[!] Lop tin chi nay da bi huy, khong the nhap diem!\n";
        return;
    }
    if (soLuong == 0) {
        cout << "[!] Lop tin chi nay chua co sinh vien nao dang ky hop le!\n";
        return;
    }

    cout << "\n---------------------------------------------------------------------------------\n";
    cout << " LOP TIN CHI: " << maLopTC << " | MON: " << nodeMH->mh.TENMH << " (" << maMH << ")\n";
    cout << " Nien khoa: " << nienKhoa << " | Hoc ky: " << hocKy << " | Nhom: " << nhom << "\n";
    cout << " Tong so sinh vien: " << soLuong << "\n";
    cout << "---------------------------------------------------------------------------------\n";
    cout << left << setw(5)  << "STT"
         << setw(15) << "MASV"
         << setw(26) << "HO"
         << setw(14) << "TEN"
         << setw(8)  << "DIEM" << "\n";
    cout << "---------------------------------------------------------------------------------\n";

    for (int i = 0; i < soLuong; i++) {
        cout << left << setw(5)  << (i + 1)
             << setw(15) << dsSV[i].MASV
             << setw(26) << dsSV[i].HO
             << setw(14) << dsSV[i].TEN;
        if (dsSV[i].DIEM < 0) {
            cout << setw(8) << "--" << "\n";
        } else {
            cout << fixed << setprecision(1) << setw(8) << dsSV[i].DIEM << "\n";
        }
    }
    cout << "---------------------------------------------------------------------------------\n";

    cout << "\n[CHU Y] Nhap diem cho tung sinh vien tu 0.0 den 10.0.\n";
    cout << "       (Nhan Enter khong nhap gi de GIU NGUYEN diem hien tai; Nhap 'q' de thoat)\n\n";

    int soCapNhat = 0;
    for (int i = 0; i < soLuong; i++) {
        while (true) {
            cout << "[" << (i + 1) << "/" << soLuong << "] "
                 << dsSV[i].MASV << " - " << dsSV[i].HO << " " << dsSV[i].TEN
                 << " (Hien tai: ";
            if (dsSV[i].DIEM < 0) cout << "Chua co";
            else cout << fixed << setprecision(1) << dsSV[i].DIEM;
            cout << ") -> Diem moi: ";

            char buff[50];
            cin.getline(buff, sizeof(buff));
            ChuanHoaTen(buff);

            // Bỏ qua nếu nhấn Enter để giữ nguyên
            if (strlen(buff) == 0) {
                break;
            }

            // Người dùng muốn dừng nhập giữa chừng
            if (stricmp(buff, "q") == 0) {
                cout << "[!] Da dung qua trinh nhap diem.\n";
                goto KetThucNhap;
            }

            // Chuyển đổi chuỗi thành số float an toàn
            char* endPtr = nullptr;
            float diem = (float)strtod(buff, &endPtr);
            if (endPtr == buff || *endPtr != '\0' || diem < 0.0f || diem > 10.0f) {
                cout << "    [!] Diem khong hop le! Vui long nhap so thuc tu 0.0 den 10.0.\n";
                continue;
            }

            // Cập nhật vào danh sách lớp tín chỉ
            if (CapNhatDiemSinhVien(dsltc, maLopTC, dsSV[i].MASV, diem)) {
                dsSV[i].DIEM = diem;
                soCapNhat++;
            }
            break;
        }
    }

KetThucNhap:
    if (soCapNhat > 0) {
        // Tự động lưu dữ liệu ra file dangky.txt theo Rule 7
        if (GhiFileDangKy(dsltc)) {
            cout << "[OK] Da cap nhat va luu thanh cong diem cho " << soCapNhat << " sinh vien vao file!\n";
        } else {
            cout << "[!] Cap nhat bo nho thanh cong nhung ghi file that bai!\n";
        }
    } else {
        cout << "[i] Khong co diem nao thay doi.\n";
    }
}


// ====================================================================
// CÂU J: IN BẢNG ĐIỂM MÔN HỌC CỦA 1 LỚP TÍN CHỈ
// ====================================================================

// Big-O: O(N_LTC + K_DK log K_DK)
void InBangDiemMonHocLTC_Console(const DS_LopTC &dsltc, const DS_LOPSV &dslop, TreeMonHoc rootMH,
                                const char nienKhoa[], int hocKy, const char maMH[], int nhom) {
    char nkChuan[15];
    char mhChuan[15];
    strcpy(nkChuan, nienKhoa);
    strcpy(mhChuan, maMH);
    ChuanHoaMa(nkChuan);
    ChuanHoaMa(mhChuan);

    int maLopTC = -1;
    SinhVienDiem dsSV[MAX_SV_DANGKY];
    int soLuong = LayDSSVDangKyLTCTheoThongTin(dsltc, dslop, nkChuan, hocKy, mhChuan, nhom, dsSV, MAX_SV_DANGKY, maLopTC);

    if (soLuong == -1) {
        cout << "[!] Khong tim thay lop tin chi!\n";
        return;
    }
    if (soLuong == -2) {
        cout << "[!] Lop tin chi nay da bi huy!\n";
        return;
    }

    TreeMonHoc nodeMH = TimMonHoc(rootMH, mhChuan);
    const char* tenMH = (nodeMH != nullptr) ? nodeMH->mh.TENMH : "(Khong ro ten mon)";

    cout << "\n=================================================================================\n";
    cout << "                           BANG DIEM MON HOC " << tenMH << "\n";
    cout << "             Nien khoa: " << nkChuan << "   Hoc ky: " << hocKy << "   Nhom: " << nhom << "\n";
    cout << "=================================================================================\n";
    cout << left << setw(6)  << "STT"
         << setw(16) << "MASV"
         << setw(26) << "HO"
         << setw(16) << "TEN"
         << setw(8)  << "DIEM" << "\n";
    cout << "---------------------------------------------------------------------------------\n";

    if (soLuong == 0) {
        cout << "   (Lop tin chi chua co sinh vien nao dang ky hop le)\n";
    } else {
        for (int i = 0; i < soLuong; i++) {
            cout << left << setw(6)  << (i + 1)
                 << setw(16) << dsSV[i].MASV
                 << setw(26) << dsSV[i].HO
                 << setw(16) << dsSV[i].TEN;
            if (dsSV[i].DIEM < 0) {
                cout << setw(8) << "--" << "\n";
            } else {
                cout << fixed << setprecision(1) << setw(8) << dsSV[i].DIEM << "\n";
            }
        }
    }
    cout << "=================================================================================\n\n";
}


// ====================================================================
// CÂU K: IN ĐIỂM TRUNG BÌNH KẾT THÚC KHÓA HỌC CỦA 1 LỚP THEO SỐ TÍN CHỈ
// Công thức: ĐTB = sum(DiemMax_i * STC_i) / sum(STC_i)
// STC_i = STCLT + STCTH
// ====================================================================

// Tính ĐTB tích lũy cho 1 sinh viên (chỉ lấy điểm thi lớn nhất từng môn)
// Big-O: O(N_LTC * K_DK + M_SV * log(M_Tong))
float TinhDiemTrungBinhSV(const DS_LopTC &dsltc, TreeMonHoc rootMH, const char maSV[], int &tongTinChi) {
    tongTinChi = 0;

    // Cấu trúc mảng tạm lưu điểm lớn nhất của từng môn sinh viên đã học
    struct MonDaHoc {
        char MAMH[11];
        float diemMax;
        int tongSTC;
    };

    MonDaHoc dsMon[MAX_MON_TONGKET];
    int soMon = 0;

    // Duyệt qua tất cả các lớp tín chỉ trên hệ thống
    for (int i = 0; i < dsltc.n; i++) {
        LopTinChi* ltc = dsltc.nodes[i];
        if (ltc == nullptr || ltc->HuyLop) continue;

        // Tìm bản ghi đăng ký của SV trong lớp tín chỉ này
        PTR_DangKy p = TimDangKy(ltc->FirstDK, maSV);
        if (p == nullptr || p->dk.HuyDangKy || p->dk.DIEM < 0.0f) {
            continue; // Bỏ qua nếu SV không học, đã hủy, hoặc chưa có điểm
        }

        // Tìm xem môn học này đã có trong danh sách tạm chưa
        int viTriMon = -1;
        for (int m = 0; m < soMon; m++) {
            if (stricmp(dsMon[m].MAMH, ltc->MAMH) == 0) {
                viTriMon = m;
                break;
            }
        }

        if (viTriMon != -1) {
            // Môn này học lại -> Cập nhật điểm lớn nhất
            if (p->dk.DIEM > dsMon[viTriMon].diemMax) {
                dsMon[viTriMon].diemMax = p->dk.DIEM;
            }
        } else {
            // Môn học mới -> Tra cứu số tín chỉ từ Cây BST môn học
            TreeMonHoc nodeMH = TimMonHoc(rootMH, ltc->MAMH);
            if (nodeMH != nullptr) {
                int stc = nodeMH->mh.STCLT + nodeMH->mh.STCTH;
                if (stc > 0 && soMon < MAX_MON_TONGKET) {
                    strcpy(dsMon[soMon].MAMH, ltc->MAMH);
                    dsMon[soMon].diemMax = p->dk.DIEM;
                    dsMon[soMon].tongSTC = stc;
                    soMon++;
                }
            }
        }
    }

    if (soMon == 0) {
        return -1.0f; // Chưa có môn nào có điểm
    }

    float tongDiemNhanTinChi = 0.0f;
    int tongSTC = 0;

    for (int m = 0; m < soMon; m++) {
        tongDiemNhanTinChi += (dsMon[m].diemMax * dsMon[m].tongSTC);
        tongSTC += dsMon[m].tongSTC;
    }

    tongTinChi = tongSTC;
    if (tongSTC == 0) return -1.0f;

    return (tongDiemNhanTinChi / (float)tongSTC);
}

// Lấy danh sách điểm trung bình của tất cả SV trong 1 lớp (sắp xếp Tên + Họ)
// Big-O: O(N_SV * (N_LTC * K_DK) + N_SV^2)
int LayDSDiemTrungBinhLop(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                         const char maLop[], SinhVienDiemTB dsTB[], int maxSV) {
    char malopChuan[16];
    strcpy(malopChuan, maLop);
    ChuanHoaMa(malopChuan);

    int idx = TimLop(dslop, malopChuan);
    if (idx == -1) return -1; // Không tìm thấy lớp

    LOPSV* lop = dslop.nodes[idx];
    if (lop == nullptr) return -1;

    int count = 0;
    for (PTRSV p = lop->FirstSV; p != nullptr; p = p->next) {
        if (count >= maxSV) break;

        strcpy(dsTB[count].MASV, p->sv.MASV);
        strcpy(dsTB[count].HO, p->sv.HO);
        strcpy(dsTB[count].TEN, p->sv.TEN);

        int tongTC = 0;
        dsTB[count].DIEMTB = TinhDiemTrungBinhSV(dsltc, rootMH, p->sv.MASV, tongTC);
        dsTB[count].TONG_TIN_CHI = tongTC;
        count++;
    }

    SapXepSinhVienDiemTB(dsTB, count);
    return count;
}

// In bảng thống kê điểm trung bình kết thúc khóa học của 1 lớp ra Console (Câu k)
void InDiemTrungBinhKhoaHocLop_Console(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                                      const char maLop[]) {
    char malopChuan[16];
    strcpy(malopChuan, maLop);
    ChuanHoaMa(malopChuan);

    int idx = TimLop(dslop, malopChuan);
    if (idx == -1) {
        cout << "[!] Lop '" << malopChuan << "' khong ton tai tren he thong!\n";
        return;
    }

    SinhVienDiemTB dsTB[MAX_SV_LOP];
    int soLuong = LayDSDiemTrungBinhLop(dslop, dsltc, rootMH, malopChuan, dsTB, MAX_SV_LOP);

    cout << "\n=================================================================================\n";
    cout << "                 BANG THONG KE DIEM TRUNG BINH KHOA HOC                          \n";
    cout << "                 Lop : " << malopChuan << " - " << dslop.nodes[idx]->TENLOP << "\n";
    cout << "=================================================================================\n";
    cout << left << setw(6)  << "STT"
         << setw(16) << "MASV"
         << setw(26) << "HO"
         << setw(16) << "TEN"
         << setw(10) << "Diem TB" << "\n";
    cout << "---------------------------------------------------------------------------------\n";

    if (soLuong == 0) {
        cout << "   (Lop chua co sinh vien nao)\n";
    } else {
        for (int i = 0; i < soLuong; i++) {
            cout << left << setw(6)  << (i + 1)
                 << setw(16) << dsTB[i].MASV
                 << setw(26) << dsTB[i].HO
                 << setw(16) << dsTB[i].TEN;
            if (dsTB[i].DIEMTB < 0) {
                cout << setw(10) << "--" << "\n";
            } else {
                cout << fixed << setprecision(2) << setw(10) << dsTB[i].DIEMTB << "\n";
            }
        }
    }
    cout << "=================================================================================\n\n";
}


// ====================================================================
// CÂU L: BẢNG ĐIỂM TỔNG KẾT CÁC MÔN CỦA CÁC SINH VIÊN THUỘC 1 LỚP
// Ma trận: Hàng là SV, Cột là các môn học. Điểm là điểm max từng môn.
// ====================================================================

// Trích xuất ma trận bảng điểm tổng kết của 1 lớp
// Big-O: O(N_SV * N_LTC * K_DK + M_Mon log M_Mon)
bool LayBangDiemTongKetLop(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                          const char maLop[], BangDiemTongKet &bangDiem) {
    char malopChuan[16];
    strcpy(malopChuan, maLop);
    ChuanHoaMa(malopChuan);

    int idx = TimLop(dslop, malopChuan);
    if (idx == -1) return false;

    LOPSV* lop = dslop.nodes[idx];
    if (lop == nullptr) return false;

    strcpy(bangDiem.MALOP, lop->MALOP);
    strcpy(bangDiem.TENLOP, lop->TENLOP);
    bangDiem.soMon = 0;
    bangDiem.soSV = 0;

    // Bước 1: Thu thập danh sách các môn học duy nhất mà các SV trong lớp đã có điểm thi
    for (PTRSV pSV = lop->FirstSV; pSV != nullptr; pSV = pSV->next) {
        for (int i = 0; i < dsltc.n; i++) {
            LopTinChi* ltc = dsltc.nodes[i];
            if (ltc == nullptr || ltc->HuyLop) continue;

            PTR_DangKy pDK = TimDangKy(ltc->FirstDK, pSV->sv.MASV);
            if (pDK != nullptr && !pDK->dk.HuyDangKy && pDK->dk.DIEM >= 0.0f) {
                // Kiểm tra xem mã môn học đã có trong bảng điểm chưa
                bool daCo = false;
                for (int m = 0; m < bangDiem.soMon; m++) {
                    if (stricmp(bangDiem.danhSachMAMH[m], ltc->MAMH) == 0) {
                        daCo = true;
                        break;
                    }
                }
                if (!daCo && bangDiem.soMon < MAX_MON_TONGKET) {
                    strcpy(bangDiem.danhSachMAMH[bangDiem.soMon], ltc->MAMH);

                    TreeMonHoc nodeMH = TimMonHoc(rootMH, ltc->MAMH);
                    if (nodeMH != nullptr) {
                        strcpy(bangDiem.danhSachTENMH[bangDiem.soMon], nodeMH->mh.TENMH);
                    } else {
                        strcpy(bangDiem.danhSachTENMH[bangDiem.soMon], "");
                    }
                    bangDiem.soMon++;
                }
            }
        }
    }

    // Sắp xếp các môn học theo thứ tự Mã môn học tăng dần
    SapXepMaMonHoc(bangDiem.danhSachMAMH, bangDiem.danhSachTENMH, bangDiem.soMon);

    // Bước 2: Với mỗi sinh viên, lấy điểm lớn nhất của từng môn trong danhSachMAMH
    for (PTRSV pSV = lop->FirstSV; pSV != nullptr; pSV = pSV->next) {
        if (bangDiem.soSV >= MAX_SV_LOP) break;

        int row = bangDiem.soSV;
        strcpy(bangDiem.dsSV[row].MASV, pSV->sv.MASV);
        strcpy(bangDiem.dsSV[row].HO, pSV->sv.HO);
        strcpy(bangDiem.dsSV[row].TEN, pSV->sv.TEN);

        // Khởi tạo điểm tất cả các môn = -1.0f (chưa học / chưa có điểm)
        for (int m = 0; m < bangDiem.soMon; m++) {
            bangDiem.dsSV[row].diemMon[m] = -1.0f;
        }

        // Quét các lớp tín chỉ để tìm điểm thi lớn nhất của từng môn
        for (int i = 0; i < dsltc.n; i++) {
            LopTinChi* ltc = dsltc.nodes[i];
            if (ltc == nullptr || ltc->HuyLop) continue;

            PTR_DangKy pDK = TimDangKy(ltc->FirstDK, pSV->sv.MASV);
            if (pDK != nullptr && !pDK->dk.HuyDangKy && pDK->dk.DIEM >= 0.0f) {
                // Xác định vị trí môn học trong bảng danhSachMAMH
                for (int m = 0; m < bangDiem.soMon; m++) {
                    if (stricmp(bangDiem.danhSachMAMH[m], ltc->MAMH) == 0) {
                        if (pDK->dk.DIEM > bangDiem.dsSV[row].diemMon[m]) {
                            bangDiem.dsSV[row].diemMon[m] = pDK->dk.DIEM;
                        }
                        break;
                    }
                }
            }
        }
        bangDiem.soSV++;
    }

    return true;
}

// In bảng điểm tổng kết ra Console (Câu l)
void InBangDiemTongKetLop_Console(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                                 const char maLop[]) {
    BangDiemTongKet bangDiem;
    if (!LayBangDiemTongKetLop(dslop, dsltc, rootMH, maLop, bangDiem)) {
        cout << "[!] Lop '" << maLop << "' khong ton tai tren he thong!\n";
        return;
    }

    cout << "\n=================================================================================\n";
    cout << "                             BANG DIEM TONG KET                                  \n";
    cout << "                    Lop : " << bangDiem.MALOP << " - " << bangDiem.TENLOP << "\n";
    cout << "=================================================================================\n";

    if (bangDiem.soSV == 0) {
        cout << "   (Lop chua co sinh vien nao)\n";
        cout << "=================================================================================\n\n";
        return;
    }

    if (bangDiem.soMon == 0) {
        cout << "   (Cac sinh vien thuoc lop nay chua co diem thi cua bat ky mon hoc nao)\n";
        cout << "=================================================================================\n\n";
        return;
    }

    // In tiêu đề các cột
    cout << left << setw(5)  << "STT"
         << setw(14) << "Ma SV"
         << setw(26) << "Ho Ten";

    for (int m = 0; m < bangDiem.soMon; m++) {
        cout << setw(10) << bangDiem.danhSachMAMH[m];
    }
    cout << "\n";

    // In đường kẻ phân cách
    cout << "-----+-------------+-------------------------";
    for (int m = 0; m < bangDiem.soMon; m++) {
        cout << "+---------";
    }
    cout << "\n";

    // In từng dòng sinh viên
    for (int i = 0; i < bangDiem.soSV; i++) {
        char hoTen[70];
        strcpy(hoTen, bangDiem.dsSV[i].HO);
        strcat(hoTen, " ");
        strcat(hoTen, bangDiem.dsSV[i].TEN);

        cout << left << setw(5)  << (i + 1)
             << setw(14) << bangDiem.dsSV[i].MASV
             << setw(26) << hoTen;

        for (int m = 0; m < bangDiem.soMon; m++) {
            float d = bangDiem.dsSV[i].diemMon[m];
            if (d < 0.0f) {
                cout << setw(10) << ""; // Ô trống nếu chưa có điểm
            } else {
                cout << fixed << setprecision(1) << setw(10) << d;
            }
        }
        cout << "\n";
    }

    cout << "=================================================================================\n\n";
}

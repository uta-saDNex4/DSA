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
#include <ctype.h>
#include <stdlib.h>

using namespace std;

// ====================================================================
// THUẬT TOÁN SẮP XẾP NỘI BỘ (KHÔNG DÙNG STL SORT THEO RULE 1 & 7)
// Sử dụng Insertion Sort (ổn định, tối ưu cho mảng kích thước nhỏ/vừa)
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
static void SapXepMaMonHoc(char dsMAMH[][11], char dsTENMH[][51], int dsSTC[], int n) {
    for (int i = 1; i < n; i++) {
        char keyMa[11];
        char keyTen[51];
        int keySTC = dsSTC[i];
        strcpy(keyMa, dsMAMH[i]);
        strcpy(keyTen, dsTENMH[i]);

        int j = i - 1;
        while (j >= 0 && stricmp(dsMAMH[j], keyMa) > 0) {
            strcpy(dsMAMH[j + 1], dsMAMH[j]);
            strcpy(dsTENMH[j + 1], dsTENMH[j]);
            dsSTC[j + 1] = dsSTC[j];
            j--;
        }
        strcpy(dsMAMH[j + 1], keyMa);
        strcpy(dsTENMH[j + 1], keyTen);
        dsSTC[j + 1] = keySTC;
    }
}

// Hàm nội bộ cắt bỏ khoảng trắng 2 đầu chuỗi
static void CatKhoangTrang(char str[]) {
    if (str == nullptr) return;
    int len = strlen(str);
    if (len == 0) return;

    int start = 0;
    while (start < len && isspace((unsigned char)str[start])) start++;

    int end = len - 1;
    while (end >= start && isspace((unsigned char)str[end])) end--;

    int idx = 0;
    for (int i = start; i <= end; i++) {
        str[idx++] = str[i];
    }
    str[idx] = '\0';
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

// Cập nhật điểm cho 1 sinh viên trong lớp tín chỉ (cho phép điểm 0.0 - 10.0 hoặc -1.0 để hủy điểm)
// Big-O: O(K_DK)
bool CapNhatDiemSinhVien(DS_LopTC &dsltc, int maLopTC, const char maSV[], float diemMoi) {
    if ((diemMoi < 0.0f && diemMoi != -1.0f) || diemMoi > 10.0f) {
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
    cout << "\n=================================================================================\n";
    cout << "                    NHAP / CHINH SUA DIEM LOP TIN CHI (CAU I)                    \n";
    cout << "=================================================================================\n";

    char nienKhoa[15];
    int hocKy = 0;
    char maMH[15];
    int nhom = 0;

    cout << "Nhap Nien khoa (vi du: 2025-2026): ";
    cin.getline(nienKhoa, sizeof(nienKhoa));
    ChuanHoaMa(nienKhoa);
    if (!KiemTraNienKhoaHopLe(nienKhoa)) {
        cout << "[!] Nien khoa khong hop le (Dinh dang YYYY-YYYY, nam sau = nam truoc + 1)!\n";
        return;
    }

    hocKy = NhapSoNguyen("Nhap Hoc ky (1..3): ");
    if (hocKy < 1 || hocKy > 3) {
        cout << "[!] Hoc ky khong hop le (chi tu 1 den 3)!\n";
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
        cout << "[!] Lop tin chi nay da bi huy (HuyLop = true), khong the nhap diem!\n";
        return;
    }
    if (soLuong == 0) {
        cout << "[!] Lop tin chi nay chua co sinh vien nao dang ky hop le!\n";
        return;
    }

    cout << "\n---------------------------------------------------------------------------------\n";
    cout << " LOP TIN CHI: " << maLopTC << " | MON: " << nodeMH->mh.TENMH << " (" << maMH << ")\n";
    cout << " Nien khoa: " << nienKhoa << " | Hoc ky: " << hocKy << " | Nhom: " << nhom << "\n";
    cout << " Tong so sinh vien dang ky hop le: " << soLuong << "\n";
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

    cout << "\n[HUONG DAN NHAP LIEU]:\n";
    cout << " - Nhap so thuc tu 0.0 den 10.0 de cap nhat diem cho sinh vien.\n";
    cout << " - Nhan [Enter] de GIU NGUYEN diem hien tai.\n";
    cout << " - Nhap 'x' hoac '-1' de XOA DIEM (ve trang thai chua thi).\n";
    cout << " - Nhap 'q' de KET THUC som va luu cac diem da nhap.\n\n";

    int soCapNhat = 0;
    for (int i = 0; i < soLuong; i++) {
        while (true) {
            cout << "[" << (i + 1) << "/" << soLuong << "] "
                 << dsSV[i].MASV << " - " << dsSV[i].HO << " " << dsSV[i].TEN
                 << " (Diem hien tai: ";
            if (dsSV[i].DIEM < 0) cout << "--";
            else cout << fixed << setprecision(1) << dsSV[i].DIEM;
            cout << ") -> Nhap diem: ";

            char buff[50];
            cin.getline(buff, sizeof(buff));
            CatKhoangTrang(buff);

            // Nhấn Enter giữ nguyên điểm cũ
            if (strlen(buff) == 0) {
                break;
            }

            // Dừng tiến trình
            if (stricmp(buff, "q") == 0) {
                cout << "[*] Dung qua trinh nhap diem theo yeu cau cua nguoi dung.\n";
                goto KetThucNhap;
            }

            // Xóa điểm về -1
            if (stricmp(buff, "x") == 0 || strcmp(buff, "-1") == 0) {
                if (CapNhatDiemSinhVien(dsltc, maLopTC, dsSV[i].MASV, -1.0f)) {
                    dsSV[i].DIEM = -1.0f;
                    soCapNhat++;
                    cout << "    => Da xoa diem cua sinh vien (ve trang thai chua thi).\n";
                }
                break;
            }

            // Chuyển đổi và kiểm tra giá trị điểm số
            char* endPtr = nullptr;
            float diem = (float)strtod(buff, &endPtr);
            if (endPtr == buff || *endPtr != '\0' || diem < 0.0f || diem > 10.0f) {
                cout << "    [!] Loi: Diem phai la so thuc trong khoang tu 0.0 den 10.0! Vui long nhap lai.\n";
                continue;
            }

            // Cập nhật điểm vào danh sách đăng ký
            if (CapNhatDiemSinhVien(dsltc, maLopTC, dsSV[i].MASV, diem)) {
                dsSV[i].DIEM = diem;
                soCapNhat++;
                cout << "    => Cap nhat thanh cong: " << fixed << setprecision(1) << diem << "\n";
            }
            break;
        }
    }

KetThucNhap:
    if (soCapNhat > 0) {
        // Tự động lưu dữ liệu ra file dangky.txt theo Rule 7
        if (GhiFileDangKy(dsltc)) {
            cout << "\n[OK] Da cap nhat va luu thanh cong diem cho " << soCapNhat << " sinh vien vao file!\n";
        } else {
            cout << "\n[!] Cap nhat bo nho thanh cong nhung ghi file that bai!\n";
        }
    } else {
        cout << "\n[i] Khong co diem nao bi thay doi.\n";
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
        cout << "[!] Khong tim thay lop tin chi voi thong tin da nhap!\n";
        return;
    }
    if (soLuong == -2) {
        cout << "[!] Lop tin chi nay da bi huy!\n";
        return;
    }

    TreeMonHoc nodeMH = TimMonHoc(rootMH, mhChuan);
    const char* tenMH = (nodeMH != nullptr) ? nodeMH->mh.TENMH : "(Khong ro ten mon)";

    cout << "\n=================================================================================\n";
    cout << "                           BANG DIEM MON HOC: " << tenMH << "\n";
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
        int soCoDiem = 0;
        float tongDiem = 0.0f;

        for (int i = 0; i < soLuong; i++) {
            cout << left << setw(6)  << (i + 1)
                 << setw(16) << dsSV[i].MASV
                 << setw(26) << dsSV[i].HO
                 << setw(16) << dsSV[i].TEN;
            if (dsSV[i].DIEM < 0) {
                cout << setw(8) << "--" << "\n";
            } else {
                cout << fixed << setprecision(1) << setw(8) << dsSV[i].DIEM << "\n";
                tongDiem += dsSV[i].DIEM;
                soCoDiem++;
            }
        }
        cout << "---------------------------------------------------------------------------------\n";
        cout << "Tong so SV dang ky: " << soLuong
             << " | Da co diem: " << soCoDiem;
        if (soCoDiem > 0) {
            cout << " | Diem TB lop: " << fixed << setprecision(2) << (tongDiem / soCoDiem);
        }
        cout << "\n";
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
        int soCoDiem = 0;
        float tongDTBLop = 0.0f;

        for (int i = 0; i < soLuong; i++) {
            cout << left << setw(6)  << (i + 1)
                 << setw(16) << dsTB[i].MASV
                 << setw(26) << dsTB[i].HO
                 << setw(16) << dsTB[i].TEN;
            if (dsTB[i].DIEMTB < 0) {
                cout << setw(10) << "--" << "\n";
            } else {
                cout << fixed << setprecision(2) << setw(10) << dsTB[i].DIEMTB << "\n";
                tongDTBLop += dsTB[i].DIEMTB;
                soCoDiem++;
            }
        }
        cout << "---------------------------------------------------------------------------------\n";
        cout << "Si so lop: " << soLuong
             << " | So SV da tich luy diem: " << soCoDiem;
        if (soCoDiem > 0) {
            cout << " | Diem TB chung toan lop: " << fixed << setprecision(2) << (tongDTBLop / soCoDiem);
        }
        cout << "\n";
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

    // Bước 1: Thu thập danh sách các môn học duy nhất mà các SV trong lớp đã đăng ký
    for (PTRSV pSV = lop->FirstSV; pSV != nullptr; pSV = pSV->next) {
        for (int i = 0; i < dsltc.n; i++) {
            LopTinChi* ltc = dsltc.nodes[i];
            if (ltc == nullptr || ltc->HuyLop) continue;

            PTR_DangKy pDK = TimDangKy(ltc->FirstDK, pSV->sv.MASV);
            if (pDK != nullptr && !pDK->dk.HuyDangKy) {
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
                        bangDiem.danhSachSTC[bangDiem.soMon] = nodeMH->mh.STCLT + nodeMH->mh.STCTH;
                    } else {
                        strcpy(bangDiem.danhSachTENMH[bangDiem.soMon], "");
                        bangDiem.danhSachSTC[bangDiem.soMon] = 0;
                    }
                    bangDiem.soMon++;
                }
            }
        }
    }

    // Sắp xếp các môn học theo thứ tự Mã môn học tăng dần
    SapXepMaMonHoc(bangDiem.danhSachMAMH, bangDiem.danhSachTENMH, bangDiem.danhSachSTC, bangDiem.soMon);

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

// In bảng điểm tổng kết ra Console (Câu l) - Hỗ trợ phân trang cột chống vỡ màn hình
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
        cout << "   (Cac sinh vien thuoc lop nay chua dang ky mon hoc nao)\n";
        cout << "=================================================================================\n\n";
        return;
    }

    // Phân trang môn học: Mỗi trang hiển thị tối đa 6 cột môn để không vượt quá chiều rộng Console
    const int MON_MOI_TRANG = 6;
    int tongSoTrang = (bangDiem.soMon + MON_MOI_TRANG - 1) / MON_MOI_TRANG;

    for (int trang = 0; trang < tongSoTrang; trang++) {
        int batDau = trang * MON_MOI_TRANG;
        int ketThuc = batDau + MON_MOI_TRANG;
        if (ketThuc > bangDiem.soMon) ketThuc = bangDiem.soMon;

        if (tongSoTrang > 1) {
            cout << "\n>>> PHAN DOAN " << (trang + 1) << "/" << tongSoTrang
                 << " (Mon " << (batDau + 1) << " den " << ketThuc << " trong tong so " << bangDiem.soMon << " mon):\n";
        }

        // In tiêu đề các cột
        cout << left << setw(5)  << "STT"
             << setw(13) << "Ma SV"
             << setw(24) << "Ho Ten";

        for (int m = batDau; m < ketThuc; m++) {
            cout << setw(10) << bangDiem.danhSachMAMH[m];
        }
        cout << "\n";

        // In đường kẻ phân cách
        cout << "----+------------+-----------------------";
        for (int m = batDau; m < ketThuc; m++) {
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
                 << setw(13) << bangDiem.dsSV[i].MASV
                 << setw(24) << hoTen;

            for (int m = batDau; m < ketThuc; m++) {
                float d = bangDiem.dsSV[i].diemMon[m];
                if (d < 0.0f) {
                    cout << setw(10) << ""; // Để trống nếu chưa có điểm
                } else {
                    cout << fixed << setprecision(1) << setw(10) << d;
                }
            }
            cout << "\n";
        }
        cout << "----+------------+-----------------------";
        for (int m = batDau; m < ketThuc; m++) {
            cout << "+---------";
        }
        cout << "\n";
    }

    // In bảng chú giải Mã môn học -> Tên môn học & Tín chỉ
    cout << "\n---------------------------------------------------------------------------------\n";
    cout << "CHU GIAI MA MON HOC:\n";
    for (int m = 0; m < bangDiem.soMon; m++) {
        cout << "  - [" << left << setw(8) << bangDiem.danhSachMAMH[m] << "]: "
             << setw(35) << bangDiem.danhSachTENMH[m]
             << " (" << bangDiem.danhSachSTC[m] << " TC)\n";
    }
    cout << "---------------------------------------------------------------------------------\n\n";
}

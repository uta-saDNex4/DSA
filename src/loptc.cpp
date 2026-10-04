#include "loptc.h"
#include "dangky.h"
#include "sinhvien.h"
#include "monhoc.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <string.h>

using namespace std;

void KhoiTaoDS_LopTC(DS_LopTC &ds) {
    ds.n = 0;
    for (int i = 0; i < MAX_LOPTC; i++) {
        ds.nodes[i] = nullptr;
    }
}

int LayMaLopTCTuDong(const DS_LopTC &ds) {
    int maxId = 0;
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr && ds.nodes[i]->MALOPTC > maxId) {
            maxId = ds.nodes[i]->MALOPTC;
        }
    }
    return maxId + 1;
}

int TimLopTCTheoMa(const DS_LopTC &ds, int maLopTC) {
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr && ds.nodes[i]->MALOPTC == maLopTC) {
            return i;
        }
    }
    return -1;
}

int TimLopTCTrung(const DS_LopTC &ds, const char nienKhoa[], int hocKy, const char maMH[], int nhom) {
    for (int i = 0; i < ds.n; i++) {
        LopTinChi* ltc = ds.nodes[i];
        if (ltc != nullptr && !ltc->HuyLop &&
            stricmp(ltc->NienKhoa, nienKhoa) == 0 &&
            ltc->HocKy == hocKy &&
            stricmp(ltc->MAMH, maMH) == 0 &&
            ltc->Nhom == nhom) {
            return i;
        }
    }
    return -1;
}

// [CÂU A] Mở lớp tín chỉ: Thêm mới
bool ThemLopTC(DS_LopTC &ds, LopTinChi ltc, TreeMonHoc rootMH) {
    if (ds.n >= MAX_LOPTC) {
        cout << "[!] Danh sach lop tin chi da day (toi da " << MAX_LOPTC << " lop)!\n";
        return false;
    }

    ChuanHoaMa(ltc.MAMH);
    ChuanHoaMa(ltc.NienKhoa);

    // Kiểm tra định dạng Niên khóa YYYY-YYYY
    if (!KiemTraNienKhoaHopLe(ltc.NienKhoa)) {
        cout << "[!] Loi: Nien khoa '" << ltc.NienKhoa << "' khong hop le (Dinh dang chuan: YYYY-YYYY, vi du: 2025-2026)!\n";
        return false;
    }

    // Kiểm tra môn học tồn tại trên Cây BST môn học
    if (TimMonHoc(rootMH, ltc.MAMH) == nullptr) {
        cout << "[!] Loi: Ma mon hoc '" << ltc.MAMH << "' khong ton tai tren he thong!\n";
        return false;
    }

    // Kiểm tra trùng tổ hợp (Niên khóa, Học kỳ, Mã MH, Nhóm)
    if (TimLopTCTrung(ds, ltc.NienKhoa, ltc.HocKy, ltc.MAMH, ltc.Nhom) != -1) {
        cout << "[!] Loi: Lop tin chi nay da ton tai (trung Mon, Nien khoa, Hoc ky, Nhom)!\n";
        return false;
    }

    if (ltc.SoSVMin <= 0 || ltc.SoSVMax < ltc.SoSVMin) {
        cout << "[!] Loi: So luong sinh vien khong hop le (Min > 0 va Max >= Min)!\n";
        return false;
    }

    if (ltc.HocKy < 1 || ltc.HocKy > 3) {
        cout << "[!] Loi: Hoc ky chi nhan gia tri tu 1 den 3!\n";
        return false;
    }

    if (ltc.Nhom <= 0) {
        cout << "[!] Loi: Nhom lop phai la so nguyen duong!\n";
        return false;
    }

    LopTinChi* newLTC = new LopTinChi;
    newLTC->MALOPTC = LayMaLopTCTuDong(ds);
    strcpy(newLTC->MAMH, ltc.MAMH);
    strcpy(newLTC->NienKhoa, ltc.NienKhoa);
    newLTC->HocKy = ltc.HocKy;
    newLTC->Nhom = ltc.Nhom;
    newLTC->SoSVMin = ltc.SoSVMin;
    newLTC->SoSVMax = ltc.SoSVMax;
    newLTC->HuyLop = false;
    newLTC->FirstDK = nullptr;

    ds.nodes[ds.n] = newLTC;
    ds.n++;

    cout << "[OK] Da mo lop tin chi thanh cong! Ma lop TC duoc cap: " << newLTC->MALOPTC << "\n";
    return true;
}

// [CÂU A] Mở lớp tín chỉ: Xóa
bool XoaLopTC(DS_LopTC &ds, int maLopTC) {
    int idx = TimLopTCTheoMa(ds, maLopTC);
    if (idx == -1) {
        cout << "[!] Loi: Khong tim thay lop tin chi co ma " << maLopTC << "!\n";
        return false;
    }

    LopTinChi* ltc = ds.nodes[idx];
    int soSVDK = DemTongSVDangKy(ltc->FirstDK);
    if (soSVDK > 0) {
        cout << "[!] Khong the xoa lop tin chi ma " << maLopTC
             << " vi da co " << soSVDK << " sinh vien dang ky!\n";
        cout << "    (Goi y: Hay su dung chuc nang Huy lop tin chi o Cau h de bao toan lich su)\n";
        return false;
    }

    GiaiPhongDS_DangKy(ltc->FirstDK);
    delete ltc;

    // Dồn mảng con trỏ
    for (int i = idx; i < ds.n - 1; i++) {
        ds.nodes[i] = ds.nodes[i + 1];
    }
    ds.nodes[ds.n - 1] = nullptr;
    ds.n--;

    cout << "[OK] Da xoa lop tin chi ma " << maLopTC << " thanh cong.\n";
    return true;
}

// [CÂU A] Mở lớp tín chỉ: Hiệu chỉnh
bool HieuChinhLopTC(DS_LopTC &ds, int maLopTC, LopTinChi ltcMoi, TreeMonHoc rootMH) {
    int idx = TimLopTCTheoMa(ds, maLopTC);
    if (idx == -1) {
        cout << "[!] Loi: Khong tim thay lop tin chi co ma " << maLopTC << "!\n";
        return false;
    }

    LopTinChi* ltc = ds.nodes[idx];
    ChuanHoaMa(ltcMoi.MAMH);
    ChuanHoaMa(ltcMoi.NienKhoa);

    // Kiểm tra định dạng Niên khóa YYYY-YYYY
    if (!KiemTraNienKhoaHopLe(ltcMoi.NienKhoa)) {
        cout << "[!] Loi: Nien khoa '" << ltcMoi.NienKhoa << "' khong hop le (Dinh dang chuan: YYYY-YYYY)!\n";
        return false;
    }

    if (TimMonHoc(rootMH, ltcMoi.MAMH) == nullptr) {
        cout << "[!] Loi: Ma mon hoc '" << ltcMoi.MAMH << "' khong ton tai!\n";
        return false;
    }

    // RÀNG BUỘC TOÀN VẸN: Nếu lớp đã có sinh viên đăng ký, KHÔNG CHO ĐỔI Môn/Niên khóa/Học kỳ
    int soSVHienTai = DemSVDangKyHopLe(ltc->FirstDK);
    if (soSVHienTai > 0) {
        if (stricmp(ltc->MAMH, ltcMoi.MAMH) != 0 ||
            stricmp(ltc->NienKhoa, ltcMoi.NienKhoa) != 0 ||
            ltc->HocKy != ltcMoi.HocKy) {
            cout << "[!] Loi: Lop tin chi ma " << maLopTC << " da co " << soSVHienTai
                 << " sinh vien dang ky, KHONG DUOC PHEP thay doi Mon hoc, Nien khoa hoac Hoc ky!\n";
            cout << "    (Chi cho phep hieu chinh Nhom, SoSVMin, SoSVMax)\n";
            return false;
        }
    }

    // Kiểm tra trùng nếu thay đổi các thông tin định danh
    for (int i = 0; i < ds.n; i++) {
        if (i == idx) continue;
        LopTinChi* other = ds.nodes[i];
        if (other != nullptr && !other->HuyLop &&
            stricmp(other->NienKhoa, ltcMoi.NienKhoa) == 0 &&
            other->HocKy == ltcMoi.HocKy &&
            stricmp(other->MAMH, ltcMoi.MAMH) == 0 &&
            other->Nhom == ltcMoi.Nhom) {
            cout << "[!] Loi: Thong tin hieu chinh bi trung voi lop tin chi ma " << other->MALOPTC << "!\n";
            return false;
        }
    }

    if (ltcMoi.SoSVMax < soSVHienTai) {
        cout << "[!] Loi: So SV Max (" << ltcMoi.SoSVMax
             << ") khong duoc nho hon so SV hien tai da dang ky (" << soSVHienTai << ")!\n";
        return false;
    }

    if (ltcMoi.SoSVMin <= 0 || ltcMoi.SoSVMax < ltcMoi.SoSVMin) {
        cout << "[!] Loi: So luong sinh vien khong hop le (Min > 0 va Max >= Min)!\n";
        return false;
    }

    strcpy(ltc->MAMH, ltcMoi.MAMH);
    strcpy(ltc->NienKhoa, ltcMoi.NienKhoa);
    ltc->HocKy = ltcMoi.HocKy;
    ltc->Nhom = ltcMoi.Nhom;
    ltc->SoSVMin = ltcMoi.SoSVMin;
    ltc->SoSVMax = ltcMoi.SoSVMax;
    ltc->HuyLop = ltcMoi.HuyLop;

    cout << "[OK] Da hieu chinh lop tin chi ma " << maLopTC << " thanh cong.\n";
    return true;
}

// Cấu trúc hỗ trợ sắp xếp in danh sách sinh viên đăng ký ở câu b
struct SV_InLTC {
    char MASV[16];
    char HO_TEN[68];
    bool HuyDangKy;
};

static void QuickSortSV_InLTC(SV_InLTC arr[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        SV_InLTC tempPivot = arr[mid]; arr[mid] = arr[high]; arr[high] = tempPivot;

        char pivotMASV[16];
        strcpy(pivotMASV, arr[high].MASV);

        int i = low - 1;
        for (int j = low; j <= high - 1; j++) {
            if (stricmp(arr[j].MASV, pivotMASV) < 0) {
                i++;
                SV_InLTC temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }
        }
        SV_InLTC temp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = temp;

        int pi = i + 1;
        QuickSortSV_InLTC(arr, low, pi - 1);
        QuickSortSV_InLTC(arr, pi + 1, high);
    }
}

// [CÂU B] In danh sách sinh viên đã đăng ký lớp tín chỉ (sắp xếp tăng dần theo Mã SV)
void InDSSVDangKyLTC(const DS_LopTC &dsltc, const DS_LOPSV &dslop, const char nienKhoa[], int hocKy, const char maMH[], int nhom) {
    int idx = TimLopTCTrung(dsltc, nienKhoa, hocKy, maMH, nhom);
    if (idx == -1) {
        cout << "[!] Khong tim thay lop tin chi nao phu hop voi thong tin cung cap!\n";
        return;
    }

    LopTinChi* ltc = dsltc.nodes[idx];
    cout << "\n=========================================================================\n";
    cout << "           DANH SACH SINH VIEN DANG KY LOP TIN CHI\n";
    cout << "  Nien khoa: " << ltc->NienKhoa << " | Hoc ky: " << ltc->HocKy
         << " | Ma MH: " << ltc->MAMH << " | Nhom: " << ltc->Nhom
         << " | Ma LTC: " << ltc->MALOPTC << "\n";
    if (ltc->HuyLop) {
        cout << "  [TRANG THAI: LOP NAY DA BI HUY]\n";
    }
    cout << "=========================================================================\n";

    int countSV = DemTongSVDangKy(ltc->FirstDK);
    if (countSV == 0) {
        cout << "  (Lop tin chi nay chua co sinh vien nao dang ky)\n";
        cout << "=========================================================================\n";
        return;
    }

    // Đưa vào mảng động để sắp xếp theo Mã SV phục vụ in danh sách chuẩn tắc
    SV_InLTC* arr = new SV_InLTC[countSV];
    int k = 0;
    for (PTR_DangKy p = ltc->FirstDK; p != nullptr; p = p->next) {
        strcpy(arr[k].MASV, p->dk.MASV);
        arr[k].HuyDangKy = p->dk.HuyDangKy;

        LOPSV* lopSV = nullptr;
        PTRSV svNode = TimSVToanTruong(dslop, p->dk.MASV, lopSV);
        if (svNode != nullptr) {
            strcpy(arr[k].HO_TEN, svNode->sv.HO);
            strcat(arr[k].HO_TEN, " ");
            strcat(arr[k].HO_TEN, svNode->sv.TEN);
        } else {
            strcpy(arr[k].HO_TEN, "(Khong ro thong tin)");
        }
        k++;
    }

    // Sắp xếp theo Mã SV
    if (countSV > 1) {
        QuickSortSV_InLTC(arr, 0, countSV - 1);
    }

    cout << setfill('-') << setw(73) << "-" << setfill(' ') << "\n";
    cout << setw(6)  << left << "STT"
         << setw(15) << left << "MA SV"
         << setw(32) << left << "HO TEN"
         << setw(20) << left << "TRANG THAI" << "\n";
    cout << setfill('-') << setw(73) << "-" << setfill(' ') << "\n";

    for (int i = 0; i < countSV; i++) {
        const char* trangThai = arr[i].HuyDangKy ? "Da huy" : "Dang hoc";
        cout << setw(6)  << left << (i + 1)
             << setw(15) << left << arr[i].MASV
             << setw(32) << left << arr[i].HO_TEN
             << setw(20) << left << trangThai << "\n";
    }
    cout << setfill('-') << setw(73) << "-" << setfill(' ') << "\n";
    cout << "=> Tong cong: " << countSV << " sinh vien trong danh sach dang ky.\n";

    delete[] arr;
}

void InDanhSachLopTC(const DS_LopTC &ds, TreeMonHoc rootMH) {
    if (ds.n == 0) {
        cout << "[i] Danh sach lop tin chi dang trong.\n";
        return;
    }

    cout << "\n================================ DANH SACH CAC LOP TIN CHI ================================\n";
    cout << setfill('-') << setw(92) << "-" << setfill(' ') << "\n";
    cout << setw(8)  << left << "MALOPTC"
         << setw(12) << left << "MAMH"
         << setw(28) << left << "TEN MON HOC"
         << setw(10) << left << "NIEN KHOA"
         << setw(5)  << left << "HK"
         << setw(6)  << left << "NHOM"
         << setw(8)  << left << "MIN/MAX"
         << setw(7)  << left << "DA DK"
         << setw(8)  << left << "TRANG THAI" << "\n";
    cout << setfill('-') << setw(92) << "-" << setfill(' ') << "\n";

    for (int i = 0; i < ds.n; i++) {
        LopTinChi* ltc = ds.nodes[i];
        if (ltc == nullptr) continue;

        TreeMonHoc nodeMH = TimMonHoc(rootMH, ltc->MAMH);
        char tenMH[51] = "(Chua ro)";
        if (nodeMH != nullptr) {
            strcpy(tenMH, nodeMH->mh.TENMH);
        }

        int daDK = DemSVDangKyHopLe(ltc->FirstDK);
        char minMaxStr[16];
        snprintf(minMaxStr, sizeof(minMaxStr), "%d/%d", ltc->SoSVMin, ltc->SoSVMax);

        cout << setw(8)  << left << ltc->MALOPTC
             << setw(12) << left << ltc->MAMH
             << setw(28) << left << tenMH
             << setw(10) << left << ltc->NienKhoa
             << setw(5)  << left << ltc->HocKy
             << setw(6)  << left << ltc->Nhom
             << setw(8)  << left << minMaxStr
             << setw(7)  << left << daDK
             << setw(8)  << left << (ltc->HuyLop ? "[DA HUY]" : "Mo") << "\n";
    }
    cout << setfill('-') << setw(92) << "-" << setfill(' ') << "\n";
}

// [CÂU H] Tự động hủy các lớp tín chỉ có số SV đăng ký < SoSVMin (có xác nhận của user)
bool XacNhanVaHuyLopTC(DS_LopTC &dsltc, const char nienKhoa[], int hocKy) {
    int countCanHuy = 0;
    int indexCanHuy[MAX_LOPTC];

    for (int i = 0; i < dsltc.n; i++) {
        LopTinChi* ltc = dsltc.nodes[i];
        if (ltc == nullptr || ltc->HuyLop) continue;

        if (stricmp(ltc->NienKhoa, nienKhoa) == 0 && ltc->HocKy == hocKy) {
            int daDK = DemSVDangKyHopLe(ltc->FirstDK);
            if (daDK < ltc->SoSVMin) {
                indexCanHuy[countCanHuy++] = i;
            }
        }
    }

    if (countCanHuy == 0) {
        cout << "[OK] Tat ca cac lop tin chi trong Nien khoa " << nienKhoa
             << " - Hoc ky " << hocKy << " deu da dat du si so toi thieu (SoSVMin)!\n";
        return false;
    }

    cout << "\n[!] PHAT HIEN " << countCanHuy << " LOP TIN CHI THIEU SI SO (< SoSVMin):\n";
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << "\n";
    cout << setw(8)  << left << "MALOPTC"
         << setw(12) << left << "MAMH"
         << setw(8)  << left << "NHOM"
         << setw(12) << left << "SV DANG KY"
         << setw(12) << left << "SV TOI THIEU" << "\n";
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << "\n";

    for (int k = 0; k < countCanHuy; k++) {
        LopTinChi* ltc = dsltc.nodes[indexCanHuy[k]];
        int daDK = DemSVDangKyHopLe(ltc->FirstDK);
        cout << setw(8)  << left << ltc->MALOPTC
             << setw(12) << left << ltc->MAMH
             << setw(8)  << left << ltc->Nhom
             << setw(12) << left << daDK
             << setw(12) << left << ltc->SoSVMin << "\n";
    }
    cout << setfill('-') << setw(65) << "-" << setfill(' ') << "\n";

    // Xác nhận từ người dùng trước khi hủy
    char xacNhan;
    cout << "Ban co chac chan muon HUY tat ca " << countCanHuy << " lop tin chi tren khong? (Y/N): ";
    cin >> xacNhan;
    cin.ignore(1000, '\n');

    if (xacNhan == 'Y' || xacNhan == 'y') {
        for (int k = 0; k < countCanHuy; k++) {
            dsltc.nodes[indexCanHuy[k]]->HuyLop = true;
        }
        cout << "[OK] Da huy thanh cong " << countCanHuy << " lop tin chi!\n";
        return true;
    } else {
        cout << "[i] Da huy bo thao tac. Khong co lop tin chi nao bi huy.\n";
        return false;
    }
}

void GiaiPhongDS_LopTC(DS_LopTC &ds) {
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr) {
            GiaiPhongDS_DangKy(ds.nodes[i]->FirstDK);
            delete ds.nodes[i];
            ds.nodes[i] = nullptr;
        }
    }
    ds.n = 0;
}

#include "dangky.h"
#include "sinhvien.h"
#include "monhoc.h"
#include "utils.h"
#include <iostream>
#include <iomanip>
#include <string.h>

using namespace std;

void KhoiTaoDS_DangKy(PTR_DangKy &FirstDK) {
    FirstDK = nullptr;
}

int DemSVDangKyHopLe(PTR_DangKy FirstDK) {
    int count = 0;
    for (PTR_DangKy p = FirstDK; p != nullptr; p = p->next) {
        if (!p->dk.HuyDangKy) count++;
    }
    return count;
}

int DemTongSVDangKy(PTR_DangKy FirstDK) {
    int count = 0;
    for (PTR_DangKy p = FirstDK; p != nullptr; p = p->next) count++;
    return count;
}

PTR_DangKy TimDangKy(PTR_DangKy FirstDK, const char maSV[]) {
    for (PTR_DangKy p = FirstDK; p != nullptr; p = p->next) {
        if (stricmp(p->dk.MASV, maSV) == 0) return p;
    }
    return nullptr;
}

bool ThemDangKy(PTR_DangKy &FirstDK, const char maSV[]) {
    PTR_DangKy p = TimDangKy(FirstDK, maSV);
    if (p != nullptr) {
        if (!p->dk.HuyDangKy) {
            cout << "[!] Sinh vien '" << maSV << "' da dang ky lop nay roi!\n";
            return false;
        }
        // Phục hồi đăng ký nếu trước đó đã hủy
        p->dk.HuyDangKy = false;
        cout << "[OK] Phuc hoi dang ky thanh cong cho SV '" << maSV << "'.\n";
        return true;
    }

    PTR_DangKy newNode = new nodeDangKy;
    strcpy(newNode->dk.MASV, maSV);
    newNode->dk.DIEM = -1.0f;
    newNode->dk.HuyDangKy = false;
    newNode->next = FirstDK;
    FirstDK = newNode;

    cout << "[OK] Dang ky thanh cong cho SV '" << maSV << "'.\n";
    return true;
}

bool HuyDangKySV(PTR_DangKy FirstDK, const char maSV[]) {
    PTR_DangKy p = TimDangKy(FirstDK, maSV);
    if (p == nullptr || p->dk.HuyDangKy) {
        cout << "[!] Khong tim thay sinh vien '" << maSV << "' trong danh sach dang ky hop le!\n";
        return false;
    }

    if (p->dk.DIEM >= 0) {
        cout << "[!] Khong the huy dang ky vi sinh vien '" << maSV << "' da co diem thi (" << p->dk.DIEM << ")!\n";
        return false;
    }

    p->dk.HuyDangKy = true;
    cout << "[OK] Da huy dang ky thanh cong cho SV '" << maSV << "'.\n";
    return true;
}

bool PhucHoiDangKySV(PTR_DangKy FirstDK, const char maSV[]) {
    PTR_DangKy p = TimDangKy(FirstDK, maSV);
    if (p == nullptr) {
        cout << "[!] Khong tim thay ban ghi dang ky cua SV '" << maSV << "'!\n";
        return false;
    }
    p->dk.HuyDangKy = false;
    return true;
}

bool DaDangKyMonHocTrongKy(const DS_LopTC &dsltc, const char maSV[], const char nienKhoa[], int hocKy, const char maMH[]) {
    for (int i = 0; i < dsltc.n; i++) {
        LopTinChi* ltc = dsltc.nodes[i];
        if (ltc == nullptr || ltc->HuyLop) continue;

        if (stricmp(ltc->NienKhoa, nienKhoa) == 0 &&
            ltc->HocKy == hocKy &&
            stricmp(ltc->MAMH, maMH) == 0) {
            PTR_DangKy p = TimDangKy(ltc->FirstDK, maSV);
            if (p != nullptr && !p->dk.HuyDangKy) {
                return true;
            }
        }
    }
    return false;
}

// In danh sách các môn SV đã đăng ký trong kỳ kèm tổng số tín chỉ
static void InMonDaDangKyCuaSV(const DS_LopTC &dsltc, TreeMonHoc rootMH, const char maSV[], const char nienKhoa[], int hocKy) {
    cout << "\n--- CAC MON BAN DA DANG KY (NK: " << nienKhoa << " - HK: " << hocKy << ") ---\n";
    cout << setfill('-') << setw(72) << "-" << setfill(' ') << "\n";
    cout << setw(8)  << left << "MALOPTC"
         << setw(12) << left << "MAMH"
         << setw(32) << left << "TEN MON HOC"
         << setw(8)  << left << "NHOM"
         << setw(12) << left << "SO TC" << "\n";
    cout << setfill('-') << setw(72) << "-" << setfill(' ') << "\n";

    int tongTC = 0;
    int soMon = 0;
    for (int i = 0; i < dsltc.n; i++) {
        LopTinChi* ltc = dsltc.nodes[i];
        if (ltc == nullptr || ltc->HuyLop) continue;
        if (stricmp(ltc->NienKhoa, nienKhoa) != 0 || ltc->HocKy != hocKy) continue;

        PTR_DangKy p = TimDangKy(ltc->FirstDK, maSV);
        if (p != nullptr && !p->dk.HuyDangKy) {
            TreeMonHoc nodeMH = TimMonHoc(rootMH, ltc->MAMH);
            char tenMH[51] = "(Chua xac dinh)";
            int stc = 0;
            if (nodeMH != nullptr) {
                strcpy(tenMH, nodeMH->mh.TENMH);
                stc = nodeMH->mh.STCLT + nodeMH->mh.STCTH;
            }
            tongTC += stc;
            soMon++;

            cout << setw(8)  << left << ltc->MALOPTC
                 << setw(12) << left << ltc->MAMH
                 << setw(32) << left << tenMH
                 << setw(8)  << left << ltc->Nhom
                 << setw(12) << left << stc << "\n";
        }
    }
    cout << setfill('-') << setw(72) << "-" << setfill(' ') << "\n";
    cout << "=> Tong cong da dang ky: " << soMon << " mon | " << tongTC << " tin chi.\n";
}

// [CÂU G] Đăng ký lớp tín chỉ: Tra cứu thông tin, hiển thị lớp mở và xử lý đăng ký
void XuLyDangKyLTC(DS_LopTC &dsltc, const DS_LOPSV &dslop, TreeMonHoc rootMH, const char maSV[], const char nienKhoa[], int hocKy) {
    LOPSV* lopSV = nullptr;
    PTRSV svNode = TimSVToanTruong(dslop, maSV, lopSV);
    if (svNode == nullptr) {
        cout << "[!] Loi: Sinh vien co ma '" << maSV << "' khong ton tai trong he thong!\n";
        return;
    }

    cout << "\n================ THONG TIN SINH VIEN ================\n";
    cout << "  Ma SV   : " << svNode->sv.MASV << "\n";
    cout << "  Ho ten  : " << svNode->sv.HO << " " << svNode->sv.TEN << "\n";
    cout << "  Phai    : " << svNode->sv.PHAI << "\n";
    cout << "  SDT     : " << svNode->sv.SODT << "\n";
    if (lopSV != nullptr) {
        cout << "  Lop     : " << lopSV->MALOP << " - " << lopSV->TENLOP << "\n";
    }
    cout << "=====================================================\n";

    // Lọc danh sách lớp tín chỉ mở trong niên khóa, học kỳ
    int soLopMo = 0;
    for (int i = 0; i < dsltc.n; i++) {
        LopTinChi* ltc = dsltc.nodes[i];
        if (ltc != nullptr && !ltc->HuyLop &&
            stricmp(ltc->NienKhoa, nienKhoa) == 0 &&
            ltc->HocKy == hocKy) {
            soLopMo++;
        }
    }

    if (soLopMo == 0) {
        cout << "[i] Khong co lop tin chi nao mo trong Nien khoa " << nienKhoa << ", Hoc ky " << hocKy << ".\n";
        return;
    }

    cout << "\nDANH SACH LOP TIN CHI MO (NK: " << nienKhoa << " - HK: " << hocKy << "):\n";
    cout << setfill('-') << setw(80) << "-" << setfill(' ') << "\n";
    cout << setw(8)  << left << "MALOPTC"
         << setw(12) << left << "MAMH"
         << setw(30) << left << "TEN MON HOC"
         << setw(8)  << left << "NHOM"
         << setw(10) << left << "DA DK"
         << setw(10) << left << "CON TRONG" << "\n";
    cout << setfill('-') << setw(80) << "-" << setfill(' ') << "\n";

    for (int i = 0; i < dsltc.n; i++) {
        LopTinChi* ltc = dsltc.nodes[i];
        if (ltc == nullptr || ltc->HuyLop) continue;
        if (stricmp(ltc->NienKhoa, nienKhoa) != 0 || ltc->HocKy != hocKy) continue;

        TreeMonHoc nodeMH = TimMonHoc(rootMH, ltc->MAMH);
        char tenMH[51] = "(Chua xac dinh)";
        if (nodeMH != nullptr) {
            strcpy(tenMH, nodeMH->mh.TENMH);
        }

        int daDK = DemSVDangKyHopLe(ltc->FirstDK);
        int conTrong = ltc->SoSVMax - daDK;
        if (conTrong < 0) conTrong = 0;

        cout << setw(8)  << left << ltc->MALOPTC
             << setw(12) << left << ltc->MAMH
             << setw(30) << left << tenMH
             << setw(8)  << left << ltc->Nhom
             << setw(10) << left << daDK
             << setw(10) << left << conTrong << "\n";
    }
    cout << setfill('-') << setw(80) << "-" << setfill(' ') << "\n";

    // Tối ưu UX: Hiển thị trước các môn sinh viên đã đăng ký (nếu có) trong kỳ này
    InMonDaDangKyCuaSV(dsltc, rootMH, maSV, nienKhoa, hocKy);

    // Cho phép sinh viên đăng ký lớp hoặc hủy đăng ký
    while (true) {
        int chonMaTC = NhapSoNguyen("\nNhap MALOPTC muon dang ky hoac huy (nhap 0 de dung): ");
        if (chonMaTC == 0) break;

        LopTinChi* ltcChon = nullptr;
        for (int i = 0; i < dsltc.n; i++) {
            if (dsltc.nodes[i] != nullptr && dsltc.nodes[i]->MALOPTC == chonMaTC) {
                ltcChon = dsltc.nodes[i];
                break;
            }
        }

        if (ltcChon == nullptr || ltcChon->HuyLop ||
            stricmp(ltcChon->NienKhoa, nienKhoa) != 0 ||
            ltcChon->HocKy != hocKy) {
            cout << "[!] Lop tin chi ma " << chonMaTC << " khong hop le trong ky nay!\n";
            continue;
        }

        // Kiểm tra SV đã đăng ký chính lớp này chưa -> Cho phép hủy nếu muốn
        PTR_DangKy p = TimDangKy(ltcChon->FirstDK, maSV);
        if (p != nullptr && !p->dk.HuyDangKy) {
            cout << "[!] Ban da dang ky lop tin chi " << chonMaTC << " roi.\n";
            char xacNhanHuy;
            cout << "    Ban co muon HUY dang ky lop nay khong? (Y/N): ";
            cin >> xacNhanHuy;
            cin.ignore(1000, '\n');
            if (xacNhanHuy == 'Y' || xacNhanHuy == 'y') {
                if (HuyDangKySV(ltcChon->FirstDK, maSV)) {
                    InMonDaDangKyCuaSV(dsltc, rootMH, maSV, nienKhoa, hocKy);
                }
            }
            continue;
        }

        int daDK = DemSVDangKyHopLe(ltcChon->FirstDK);
        if (daDK >= ltcChon->SoSVMax) {
            cout << "[!] Lop tin chi " << chonMaTC << " da day si so (" << daDK << "/" << ltcChon->SoSVMax << ")!\n";
            continue;
        }

        // Kiểm tra SV đã đăng ký môn này ở nhóm khác trong cùng kỳ chưa
        if (DaDangKyMonHocTrongKy(dsltc, maSV, nienKhoa, hocKy, ltcChon->MAMH)) {
            cout << "[!] Sinh vien da dang ky mon '" << ltcChon->MAMH << "' o mot lop/nhom khac trong ky nay!\n";
            continue;
        }

        if (ThemDangKy(ltcChon->FirstDK, maSV)) {
            // Tối ưu UX: Hiển thị ngay tóm tắt các môn SV đã đăng ký trong kỳ
            InMonDaDangKyCuaSV(dsltc, rootMH, maSV, nienKhoa, hocKy);
        }
    }
}

void GiaiPhongDS_DangKy(PTR_DangKy &FirstDK) {
    while (FirstDK != nullptr) {
        PTR_DangKy p = FirstDK;
        FirstDK = FirstDK->next;
        delete p;
    }
}

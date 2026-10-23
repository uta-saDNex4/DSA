#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include "struct.h"
#include "monhoc.h"
#include "lop.h"
#include "sinhvien.h"
#include "loptc.h"
#include "dangky.h"
#include "file_io.h"
#include "diem.h"
#include "utils.h"

using namespace std;

// ====================================================================
// CÁC MENU CON THEO TỪNG PHÂN HỆ
// ====================================================================

// 1. Phân hệ Môn học (Câu e, f)
void MenuMonHoc(DS_MonHoc &dsMH, const DS_LopTC &dsLTC) {
    while (true) {
        cout << "\n=======================================================\n";
        cout << "        PHAN HE MON HOC (CAY NHI PHAN TIM KIEM)        \n";
        cout << "=======================================================\n";
        cout << " 1. [Cau e] Them mon hoc moi vao cay BST\n";
        cout << " 2. [Cau e] Hieu chinh thong tin mon hoc\n";
        cout << " 3. [Cau e] Xoa mon hoc (co kiem tra rang buoc LTC)\n";
        cout << " 4. [Cau f] In danh sach mon hoc (Tang dan theo Ten mon)\n";
        cout << " 0. Quay lai Menu chinh\n";
        cout << "=======================================================\n";
        int chon = NhapSoNguyen("Chon chuc nang (0..4): ");

        if (chon == 0) break;
        switch (chon) {
            case 1: {
                NhapMonHoc(dsMH);
                break;
            }
            case 2: {
                char maMH[15];
                cout << "Nhap Ma mon hoc can hieu chinh: ";
                cin.getline(maMH, sizeof(maMH));
                ChuanHoaMa(maMH);
                SuaMonHoc(dsMH.root, maMH);
                break;
            }
            case 3: {
                char maMH[15];
                cout << "Nhap Ma mon hoc can xoa: ";
                cin.getline(maMH, sizeof(maMH));
                ChuanHoaMa(maMH);
                XoaMonHoc(dsMH.root, maMH, dsMH.n, dsLTC);
                break;
            }
            case 4: {
                InDanhSachMonHoc(dsMH.root, dsMH.n);
                break;
            }
            default:
                cout << "[!] Lua chon khong hop le!\n";
                break;
        }
    }
}

// 2. Phân hệ Lớp học & Sinh viên (Câu c, d)
void MenuLopVaSinhVien(DS_LOPSV &dsLop, const DS_LopTC &dsLTC) {
    while (true) {
        cout << "\n=======================================================\n";
        cout << "        PHAN HE LOP SINH VIEN & SINH VIEN              \n";
        cout << "=======================================================\n";
        cout << " 1. Them lop sinh vien moi\n";
        cout << " 2. In danh sach tat ca cac lop sinh vien\n";
        cout << " 3. [Cau c] Nhap / Cap nhat sinh vien cua 1 lop\n";
        cout << " 4. [Cau c] Hieu chinh thong tin 1 sinh vien\n";
        cout << " 5. [Cau c] Xoa 1 sinh vien khoi lop\n";
        cout << " 6. [Cau d] In DSSV cua 1 lop (Alphabet Ma SV)\n";
        cout << " 0. Quay lai Menu chinh\n";
        cout << "=======================================================\n";
        int chon = NhapSoNguyen("Chon chuc nang (0..6): ");

        if (chon == 0) break;
        switch (chon) {
            case 1: {
                LOPSV lop;
                cout << "Nhap Ma lop moi: ";
                cin.getline(lop.MALOP, sizeof(lop.MALOP));
                ChuanHoaMa(lop.MALOP);
                cout << "Nhap Ten lop: ";
                cin.getline(lop.TENLOP, sizeof(lop.TENLOP));
                ChuanHoaTen(lop.TENLOP);
                ThemLop(dsLop, lop);
                break;
            }
            case 2: {
                InDanhSachLop(dsLop);
                break;
            }
            case 3: {
                char maLop[16];
                cout << "Nhap Ma lop can nhap sinh vien: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);
                int idx = TimLop(dsLop, maLop);
                if (idx == -1) {
                    cout << "[!] Khong tim thay lop '" << maLop << "'!\n";
                } else {
                    cout << "[i] Bat dau nhap sinh vien cho lop " << maLop << " (nhap Ma SV rong de ket thuc):\n";
                    NhapDanhSachSV(dsLop.nodes[idx]->FirstSV);
                }
                break;
            }
            case 4: {
                char maLop[16], maSV[16];
                cout << "Nhap Ma lop cua sinh vien: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);
                int idx = TimLop(dsLop, maLop);
                if (idx == -1) {
                    cout << "[!] Khong tim thay lop '" << maLop << "'!\n";
                } else {
                    cout << "Nhap Ma SV can sua: ";
                    cin.getline(maSV, sizeof(maSV));
                    ChuanHoaMa(maSV);
                    SuaSV(dsLop.nodes[idx]->FirstSV, maSV);
                }
                break;
            }
            case 5: {
                char maLop[16], maSV[16];
                cout << "Nhap Ma lop cua sinh vien: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);
                int idx = TimLop(dsLop, maLop);
                if (idx == -1) {
                    cout << "[!] Khong tim thay lop '" << maLop << "'!\n";
                } else {
                    cout << "Nhap Ma SV can xoa: ";
                    cin.getline(maSV, sizeof(maSV));
                    ChuanHoaMa(maSV);
                    XoaSV(dsLop.nodes[idx]->FirstSV, maSV, dsLTC);
                }
                break;
            }
            case 6: {
                char maLop[16];
                cout << "Nhap Ma lop can in danh sach: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);
                int idx = TimLop(dsLop, maLop);
                if (idx == -1) {
                    cout << "[!] Khong tim thay lop '" << maLop << "'!\n";
                } else {
                    InDSSVTheoMa(dsLop.nodes[idx]->FirstSV);
                }
                break;
            }
            default:
                cout << "[!] Lua chon khong hop le!\n";
                break;
        }
    }
}

// 3. Phân hệ Lớp tín chỉ (Câu a, b, h)
void MenuLopTinChi(DS_LopTC &dsLTC, const DS_LOPSV &dsLop, TreeMonHoc rootMH) {
    while (true) {
        cout << "\n=======================================================\n";
        cout << "        PHAN HE QUAN LY LOP TIN CHI (MANG CON TRO)     \n";
        cout << "=======================================================\n";
        cout << " 1. [Cau a] Mo lop tin chi moi\n";
        cout << " 2. [Cau a] Hieu chinh thong tin lop tin chi\n";
        cout << " 3. [Cau a] Xoa lop tin chi\n";
        cout << " 4. In danh sach tat ca cac lop tin chi\n";
        cout << " 5. [Cau b] In danh sach SV da dang ky 1 lop tin chi\n";
        cout << " 6. [Cau h] Tu dong huy cac lop tin chi < SoSVMin\n";
        cout << " 0. Quay lai Menu chinh\n";
        cout << "=======================================================\n";
        int chon = NhapSoNguyen("Chon chuc nang (0..6): ");

        if (chon == 0) break;
        switch (chon) {
            case 1: {
                LopTinChi ltc;
                ltc.MALOPTC = LayMaLopTCTuDong(dsLTC);
                cout << "==> Ma lop tin chi tu dong cap: " << ltc.MALOPTC << "\n";

                cout << "Nhap Ma mon hoc: ";
                cin.getline(ltc.MAMH, sizeof(ltc.MAMH));
                ChuanHoaMa(ltc.MAMH);

                cout << "Nhap Nien khoa (YYYY-YYYY): ";
                cin.getline(ltc.NienKhoa, sizeof(ltc.NienKhoa));
                ChuanHoaMa(ltc.NienKhoa);

                ltc.HocKy = NhapSoNguyen("Nhap Hoc ky (1..3): ");
                ltc.Nhom = NhapSoNguyen("Nhap Nhom: ");
                ltc.SoSVMin = NhapSoNguyen("Nhap So SV toi thieu (Min): ");
                ltc.SoSVMax = NhapSoNguyen("Nhap So SV toi da (Max): ");
                ltc.HuyLop = false;
                ltc.FirstDK = nullptr;

                ThemLopTC(dsLTC, ltc, rootMH);
                break;
            }
            case 2: {
                int maTC = NhapSoNguyen("Nhap Ma Lop TC can hieu chinh: ");
                int idx = TimLopTCTheoMa(dsLTC, maTC);
                if (idx == -1) {
                    cout << "[!] Khong tim thay lop tin chi ma " << maTC << "!\n";
                    break;
                }
                LopTinChi ltcMoi = *(dsLTC.nodes[idx]);
                cout << "Nhap Ma mon hoc moi: ";
                cin.getline(ltcMoi.MAMH, sizeof(ltcMoi.MAMH));
                ChuanHoaMa(ltcMoi.MAMH);

                cout << "Nhap Nien khoa moi (YYYY-YYYY): ";
                cin.getline(ltcMoi.NienKhoa, sizeof(ltcMoi.NienKhoa));
                ChuanHoaMa(ltcMoi.NienKhoa);

                ltcMoi.HocKy = NhapSoNguyen("Nhap Hoc ky moi: ");
                ltcMoi.Nhom = NhapSoNguyen("Nhap Nhom moi: ");
                ltcMoi.SoSVMin = NhapSoNguyen("Nhap So SV Min moi: ");
                ltcMoi.SoSVMax = NhapSoNguyen("Nhap So SV Max moi: ");

                HieuChinhLopTC(dsLTC, maTC, ltcMoi, rootMH);
                break;
            }
            case 3: {
                int maTC = NhapSoNguyen("Nhap Ma Lop TC can xoa: ");
                XoaLopTC(dsLTC, maTC);
                break;
            }
            case 4: {
                InDanhSachLopTC(dsLTC, rootMH);
                break;
            }
            case 5: {
                char nk[15], mh[15];
                cout << "Nhap Nien khoa: ";
                cin.getline(nk, sizeof(nk));
                ChuanHoaMa(nk);
                int hk = NhapSoNguyen("Nhap Hoc ky: ");
                cout << "Nhap Ma mon hoc: ";
                cin.getline(mh, sizeof(mh));
                ChuanHoaMa(mh);
                int nhom = NhapSoNguyen("Nhap Nhom: ");

                InDSSVDangKyLTC(dsLTC, dsLop, nk, hk, mh, nhom);
                break;
            }
            case 6: {
                char nk[15];
                cout << "Nhap Nien khoa can loc huy lop: ";
                cin.getline(nk, sizeof(nk));
                ChuanHoaMa(nk);
                int hk = NhapSoNguyen("Nhap Hoc ky can loc huy lop: ");
                XacNhanVaHuyLopTC(dsLTC, nk, hk);
                break;
            }
            default:
                cout << "[!] Lua chon khong hop le!\n";
                break;
        }
    }
}

// 4. Phân hệ Đăng ký lớp tín chỉ (Câu g)
void MenuDangKyLTC(DS_LopTC &dsLTC, const DS_LOPSV &dsLop, TreeMonHoc rootMH) {
    cout << "\n=======================================================\n";
    cout << "           DANG KY LOP TIN CHI CHO SINH VIEN (CAU G)   \n";
    cout << "=======================================================\n";
    char maSV[16];
    cout << "Nhap Ma sinh vien: ";
    cin.getline(maSV, sizeof(maSV));
    ChuanHoaMa(maSV);

    char nk[15];
    cout << "Nhap Nien khoa (YYYY-YYYY): ";
    cin.getline(nk, sizeof(nk));
    ChuanHoaMa(nk);

    int hk = NhapSoNguyen("Nhap Hoc ky (1..3): ");

    XuLyDangKyLTC(dsLTC, dsLop, rootMH, maSV, nk, hk);
}

// 5. Phân hệ Điểm số & Báo cáo thống kê (Câu i, j, k, l)
void MenuDiemSoVaThongKe(DS_LopTC &dsLTC, const DS_LOPSV &dsLop, TreeMonHoc rootMH) {
    while (true) {
        cout << "\n=======================================================\n";
        cout << "        PHAN HE DIEM SO & BAO CAO THONG KE             \n";
        cout << "=======================================================\n";
        cout << " 1. [Cau i] Nhap / Chinh sua diem lop tin chi\n";
        cout << " 2. [Cau j] In bang diem mon hoc cua 1 lop tin chi\n";
        cout << " 3. [Cau k] In bang thong ke diem trung binh khoa hoc cua 1 lop\n";
        cout << " 4. [Cau l] In bang diem tong ket cac mon cua cac SV thuoc 1 lop\n";
        cout << " 0. Quay lai Menu chinh\n";
        cout << "=======================================================\n";
        int chon = NhapSoNguyen("Chon chuc nang (0..4): ");

        if (chon == 0) break;
        switch (chon) {
            case 1: {
                NhapDiemLTC_Console(dsLTC, dsLop, rootMH);
                break;
            }
            case 2: {
                cout << "\n--- IN BANG DIEM MON HOC CUA 1 LOP TIN CHI (CAU J) ---\n";
                char nienKhoa[15], maMH[15];
                cout << "Nhap Nien khoa (vi du: 2025-2026): ";
                cin.getline(nienKhoa, sizeof(nienKhoa));
                ChuanHoaMa(nienKhoa);

                int hocKy = NhapSoNguyen("Nhap Hoc ky (1..3): ");
                cout << "Nhap Ma mon hoc: ";
                cin.getline(maMH, sizeof(maMH));
                ChuanHoaMa(maMH);

                int nhom = NhapSoNguyen("Nhap Nhom: ");
                InBangDiemMonHocLTC_Console(dsLTC, dsLop, rootMH, nienKhoa, hocKy, maMH, nhom);
                break;
            }
            case 3: {
                cout << "\n--- BANG THONG KE DIEM TRUNG BINH KHOA HOC (CAU K) ---\n";
                char maLop[16];
                cout << "Nhap Ma lop can thong ke: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);
                InDiemTrungBinhKhoaHocLop_Console(dsLop, dsLTC, rootMH, maLop);
                break;
            }
            case 4: {
                cout << "\n--- BANG DIEM TONG KET CAC MON CUA 1 LOP (CAU L) ---\n";
                char maLop[16];
                cout << "Nhap Ma lop can xem bang diem: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);
                InBangDiemTongKetLop_Console(dsLop, dsLTC, rootMH, maLop);
                break;
            }
            default:
                cout << "[!] Lua chon khong hop le!\n";
                break;
        }
    }
}

// In menu chính
void InMenuChinh() {
    cout << "\n================================================================================\n";
    cout << "             CHUONG TRINH QUAN LY DIEM SINH VIEN THEO HE TIN CHI                \n";
    cout << "                          (MON CTDL & GT - N24)                                 \n";
    cout << "================================================================================\n";
    cout << " 1. [MON HOC]      Nhap, Hieu chinh, Xoa, In danh sach mon hoc (Cau e, f)\n";
    cout << " 2. [LOP & SV]     Nhap sinh vien theo lop, In DSSV theo Ma (Cau c, d)\n";
    cout << " 3. [LOP TIN CHI]  Mo lop, Hieu chinh, Xoa, In DSSV da DK, Huy lop (Cau a, b, h)\n";
    cout << " 4. [DANG KY LTC]  Sinh vien dang ky / Huy dang ky lop tin chi (Cau g)\n";
    cout << " 5. [DIEM SO & TK] Nhap diem, In bang diem, Diem TB, Bang tong ket (Cau i, j, k, l)\n";
    cout << " 6. [LUU DU LIEU]  Luu toan bo du lieu ra cac file text (thu muc data/)\n";
    cout << " 7. [NAP DU LIEU]  Nap lai toan bo du lieu tu file text\n";
    cout << " 0. Thoat chuong trinh (Tu dong luu toan bo du lieu)\n";
    cout << "================================================================================\n";
}

int main() {
    // Khởi tạo các cấu trúc dữ liệu toàn bộ hệ thống
    DS_MonHoc dsMH;
    KhoiTaoCayMonHoc(dsMH.root);

    DS_LOPSV dsLop;
    KhoiTaoDanhSachLop(dsLop);

    DS_LopTC dsLTC;
    KhoiTaoDS_LopTC(dsLTC);

    // Đọc toàn bộ dữ liệu từ các file text trong thư mục data/ (Rule 7)
    cout << "[*] Dang tai du lieu tu thu muc data/...\n";
    if (DocToanBoDuLieu(dsMH, dsLop, dsLTC)) {
        cout << "[OK] Tai du lieu thanh cong!\n";
        cout << "   - So mon hoc: " << dsMH.n << "\n";
        cout << "   - So lop sinh vien: " << dsLop.n << "\n";
        cout << "   - So lop tin chi: " << dsLTC.n << "\n";
    } else {
        cout << "[i] File du lieu chua ton tai hoac khoi tao lan dau.\n";
    }

    int luaChon = -1;
    while (true) {
        InMenuChinh();
        luaChon = NhapSoNguyen("Chon phan he chuc nang (0..7): ");

        switch (luaChon) {
            case 1: {
                MenuMonHoc(dsMH, dsLTC);
                break;
            }
            case 2: {
                MenuLopVaSinhVien(dsLop, dsLTC);
                break;
            }
            case 3: {
                MenuLopTinChi(dsLTC, dsLop, dsMH.root);
                break;
            }
            case 4: {
                MenuDangKyLTC(dsLTC, dsLop, dsMH.root);
                break;
            }
            case 5: {
                MenuDiemSoVaThongKe(dsLTC, dsLop, dsMH.root);
                break;
            }
            case 6: {
                if (LuuToanBoDuLieu(dsMH.root, dsLop, dsLTC)) {
                    cout << "[OK] Da luu toan bo du lieu thanh cong vao thu muc data/!\n";
                } else {
                    cout << "[!] Luu du lieu that bai!\n";
                }
                break;
            }
            case 7: {
                GiaiPhongCay(dsMH.root);
                dsMH.n = 0;
                GiaiPhongDSLop(dsLop);
                GiaiPhongDS_LopTC(dsLTC);

                if (DocToanBoDuLieu(dsMH, dsLop, dsLTC)) {
                    cout << "[OK] Da doc lai du lieu thanh cong!\n";
                } else {
                    cout << "[!] Doc du lieu that bai!\n";
                }
                break;
            }
            case 0: {
                cout << "[*] Dang luu du lieu truoc khi thoat chuong trinh...\n";
                LuuToanBoDuLieu(dsMH.root, dsLop, dsLTC);

                // Dọn dẹp giải phóng toàn bộ bộ nhớ
                GiaiPhongCay(dsMH.root);
                GiaiPhongDSLop(dsLop);
                GiaiPhongDS_LopTC(dsLTC);
                cout << "[OK] Da giai phong bo nho. Tam biet!\n";
                return 0;
            }
            default:
                cout << "[!] Lua chon khong hop le, vui long chon lai!\n";
                break;
        }
    }

    return 0;
}

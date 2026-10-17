#include <iostream>
#include <iomanip>
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

void InMenu() {
    cout << "\n=======================================================\n";
    cout << "   CHUONG TRINH QUAN LY DIEM SINH VIEN THEO HE TIN CHI\n";
    cout << "             PHAN HE: DIEM SO & BAO CAO THONG KE       \n";
    cout << "=======================================================\n";
    cout << " 1. [Cau i] Nhap / Chinh sua diem lop tin chi\n";
    cout << " 2. [Cau j] In bang diem mon hoc cua 1 lop tin chi\n";
    cout << " 3. [Cau k] In diem trung binh ket thuc khoa hoc cua 1 lop\n";
    cout << " 4. [Cau l] In bang diem tong ket cac mon cua 1 lop\n";
    cout << " 5. Luu toan bo du lieu ra file (data/)\n";
    cout << " 6. Nap lai du lieu tu file\n";
    cout << " 0. Thoat chuong trinh\n";
    cout << "=======================================================\n";
    cout << "Chon chuc nang (0..6): ";
}

int main() {
    // Khởi tạo các cấu trúc dữ liệu của toàn bộ hệ thống
    DS_MonHoc dsMH;
    KhoiTaoCayMonHoc(dsMH.root);

    DS_LOPSV dsLop;
    KhoiTaoDanhSachLop(dsLop);

    DS_LopTC dsLTC;
    KhoiTaoDS_LopTC(dsLTC);

    // Đọc toàn bộ dữ liệu từ các file text trong thư mục data/
    cout << "[*] Dang tai du lieu tu thu muc data/...\n";
    if (DocToanBoDuLieu(dsMH, dsLop, dsLTC)) {
        cout << "[OK] Tai du lieu thanh cong!\n";
        cout << "   - So mon hoc: " << dsMH.n << "\n";
        cout << "   - So lop sinh vien: " << dsLop.n << "\n";
        cout << "   - So lop tin chi: " << dsLTC.n << "\n";
    } else {
        cout << "[!] Co loi khi doc du lieu (file co the chua ton tai)!\n";
    }

    int luaChon = -1;
    while (true) {
        InMenu();
        char buff[50];
        if (!cin.getline(buff, sizeof(buff))) {
            break;
        }
        if (strlen(buff) == 0) continue;
        luaChon = atoi(buff);

        switch (luaChon) {
            case 1: {
                // Câu i: Nhập điểm
                NhapDiemLTC_Console(dsLTC, dsLop, dsMH.root);
                break;
            }
            case 2: {
                // Câu j: In bảng điểm môn học
                cout << "\n--- IN BANG DIEM MON HOC CUA 1 LOP TIN CHI (CAU J) ---\n";
                char nienKhoa[15], maMH[15];
                int hocKy = 0, nhom = 0;

                cout << "Nhap Nien khoa (vi du: 2025-2026): ";
                cin.getline(nienKhoa, sizeof(nienKhoa));
                ChuanHoaMa(nienKhoa);

                hocKy = NhapSoNguyen("Nhap Hoc ky (1..3): ");
                cout << "Nhap Ma mon hoc: ";
                cin.getline(maMH, sizeof(maMH));
                ChuanHoaMa(maMH);

                nhom = NhapSoNguyen("Nhap Nhom: ");

                InBangDiemMonHocLTC_Console(dsLTC, dsLop, dsMH.root, nienKhoa, hocKy, maMH, nhom);
                break;
            }
            case 3: {
                // Câu k: In ĐTB kết thúc khóa học của 1 lớp
                cout << "\n--- BANG THONG KE DIEM TRUNG BINH KHOA HOC (CAU K) ---\n";
                char maLop[16];
                cout << "Nhap Ma lop can thong ke: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);

                InDiemTrungBinhKhoaHocLop_Console(dsLop, dsLTC, dsMH.root, maLop);
                break;
            }
            case 4: {
                // Câu l: Bảng điểm tổng kết các môn của 1 lớp
                cout << "\n--- BANG DIEM TONG KET CAC MON CUA 1 LOP (CAU L) ---\n";
                char maLop[16];
                cout << "Nhap Ma lop can xem bang diem: ";
                cin.getline(maLop, sizeof(maLop));
                ChuanHoaMa(maLop);

                InBangDiemTongKetLop_Console(dsLop, dsLTC, dsMH.root, maLop);
                break;
            }
            case 5: {
                // Lưu dữ liệu
                if (LuuToanBoDuLieu(dsMH.root, dsLop, dsLTC)) {
                    cout << "[OK] Da luu toan bo du lieu thanh cong vao thu muc data/!\n";
                } else {
                    cout << "[!] Luu du lieu that bai!\n";
                }
                break;
            }
            case 6: {
                // Đọc lại dữ liệu
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
                cout << "[*] Dang luu du lieu truoc khi thoat...\n";
                LuuToanBoDuLieu(dsMH.root, dsLop, dsLTC);

                // Dọn dẹp bộ nhớ
                GiaiPhongCay(dsMH.root);
                GiaiPhongDSLop(dsLop);
                GiaiPhongDS_LopTC(dsLTC);
                cout << "[OK] Tam biet!\n";
                return 0;
            }
            default:
                cout << "[!] Lua chon khong hop le, vui long chon lai!\n";
                break;
        }
    }

    return 0;
}

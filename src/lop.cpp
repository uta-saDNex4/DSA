#include "lop.h"
#include "sinhvien.h"
#include "utils.h"
#include "rang_buoc.h"
#include <iostream>
#include <string.h>
#include <iomanip>

using namespace std;

void KhoiTaoDanhSachLop(DS_LOPSV &ds) {
    ds.n = 0;
    for (int i = 0; i < MAX_LOPSV; i++) {
        ds.nodes[i] = nullptr;
    }
}

int TimLop(DS_LOPSV ds, char MALOP[]) {
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr && stricmp(ds.nodes[i]->MALOP, MALOP) == 0) {
            return i;
        }
    }
    return -1;
}

bool ThemLop(DS_LOPSV &ds, LOPSV lop) {
    if (ds.n >= MAX_LOPSV) {
        cout << "[!] Loi: Danh sach lop da day (toi da " << MAX_LOPSV << " lop)!\n";
        return false;
    }
    
    ChuanHoaMa(lop.MALOP);
    ChuanHoaTen(lop.TENLOP);
    
    if (KiemTraRong(lop.MALOP)) {
        cout << "[!] Loi: Ma lop khong duoc de trong!\n";
        return false;
    }
    if (KiemTraRong(lop.TENLOP)) {
        cout << "[!] Loi: Ten lop khong duoc de trong!\n";
        return false;
    }
    
    if (TimLop(ds, lop.MALOP) != -1) {
        cout << "[!] Loi: Ma lop '" << lop.MALOP << "' da ton tai!\n";
        return false;
    }
    
    ds.nodes[ds.n] = new LOPSV;
    strcpy(ds.nodes[ds.n]->MALOP, lop.MALOP);
    strcpy(ds.nodes[ds.n]->TENLOP, lop.TENLOP);
    ds.nodes[ds.n]->FirstSV = nullptr;
    
    ds.n++;
    cout << "[OK] Da them lop '" << lop.MALOP << "' thanh cong!\n";
    return true;
}

bool SuaLop(DS_LOPSV &ds, char MALOP[], char TENLOP_MOI[]) {
    ChuanHoaMa(MALOP);
    int idx = TimLop(ds, MALOP);
    if (idx == -1) {
        cout << "[!] Loi: Khong tim thay lop co ma '" << MALOP << "'!\n";
        return false;
    }
    
    ChuanHoaTen(TENLOP_MOI);
    if (KiemTraRong(TENLOP_MOI)) {
        cout << "[!] Loi: Ten lop khong duoc de trong!\n";
        return false;
    }
    
    strcpy(ds.nodes[idx]->TENLOP, TENLOP_MOI);
    cout << "[OK] Da cap nhat ten lop '" << MALOP << "' thanh cong!\n";
    return true;
}

void InDanhSachLop(DS_LOPSV ds) {
    if (ds.n == 0) {
        cout << "[!] Danh sach lop rong!\n";
        return;
    }
    
    int stt = 1;
    cout << "\n--- DANH SACH LOP ---\n";
    cout << setw(5) << left << "STT"
         << setw(15) << left << "MALOP" 
         << setw(40) << left << "TEN LOP"
         << setw(10) << left << "SI SO" << endl;
    cout << "-----------------------------------------------------------------------\n";
    
    for (int i = 0; i < ds.n; i++) {
        cout << setw(5) << left << stt++
             << setw(15) << left << ds.nodes[i]->MALOP 
             << setw(40) << left << ds.nodes[i]->TENLOP
             << setw(10) << left << DemSV(ds.nodes[i]->FirstSV) << endl;
    }
    cout << "=> Tong cong: " << ds.n << " lop.\n";
}

// Giải phóng toàn bộ danh sách lớp (kèm DSSV bên trong mỗi lớp)
void GiaiPhongDSLop(DS_LOPSV &ds) {
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr) {
            GiaiPhongDSSV(ds.nodes[i]->FirstSV);
            delete ds.nodes[i];
            ds.nodes[i] = nullptr;
        }
    }
    ds.n = 0;
}

bool XoaLop(DS_LOPSV &ds, char MALOP[], DS_LopTC dsltc) {
    ChuanHoaMa(MALOP);
    int idx = TimLop(ds, MALOP);
    if (idx == -1) {
        cout << "[!] Loi: Khong tim thay lop co ma '" << MALOP << "'!\n";
        return false;
    }
    
    // Ràng buộc 1: Lớp còn sinh viên không?
    if (ds.nodes[idx]->FirstSV != nullptr) {
        int soSV = DemSV(ds.nodes[idx]->FirstSV);
        cout << "[!] Khong the xoa! Lop '" << MALOP << "' dang co " << soSV << " sinh vien.\n";
        cout << "[!] Vui long xoa het sinh vien cua lop truoc khi xoa lop.\n";
        return false;
    }
    
    // Ràng buộc 2: Sinh viên trong lớp có đang đăng ký tín chỉ không (dù sinh viên đã bị xóa nhưng vẫn còn node đăng ký mồ côi? Theo lý thuyết sinh viên đã xóa thì đk cũng phải xóa, nhưng check cho an toàn).
    int soDK = DemDangKyByLop(dsltc, ds.nodes[idx]->FirstSV);
    if (soDK > 0) {
        cout << "[!] Khong the xoa! Co " << soDK << " dang ky tin chi lien quan den sinh vien lop nay.\n";
        InRangBuocLop(dsltc, ds.nodes[idx]->FirstSV);
        return false;
    }
    
    // Xóa lớp
    GiaiPhongDSSV(ds.nodes[idx]->FirstSV); // An toàn
    delete ds.nodes[idx];
    
    // Dịch các phần tử mảng con trỏ
    for (int i = idx; i < ds.n - 1; i++) {
        ds.nodes[i] = ds.nodes[i + 1];
    }
    ds.nodes[ds.n - 1] = nullptr;
    ds.n--;
    
    cout << "[OK] Da xoa lop '" << MALOP << "' thanh cong!\n";
    return true;
}

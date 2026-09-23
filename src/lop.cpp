#include "lop.h"
#include "utils.h"
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
        if (stricmp(ds.nodes[i]->MALOP, MALOP) == 0) {
            return i;
        }
    }
    return -1;
}

bool ThemLop(DS_LOPSV &ds, LOPSV lop) {
    if (ds.n >= MAX_LOPSV) {
        cout << "Loi: Danh sach lop da day!\n";
        return false;
    }
    
    ChuanHoaMa(lop.MALOP);
    ChuanHoaTen(lop.TENLOP);
    
    if (KiemTraRong(lop.MALOP) || KiemTraRong(lop.TENLOP)) {
        cout << "Loi: Ma lop va Ten lop khong duoc de trong!\n";
        return false;
    }
    
    if (TimLop(ds, lop.MALOP) != -1) {
        cout << "Loi: Ma lop da ton tai!\n";
        return false;
    }
    
    // Cấp phát động 1 node lớp mới
    ds.nodes[ds.n] = new LOPSV;
    strcpy(ds.nodes[ds.n]->MALOP, lop.MALOP);
    strcpy(ds.nodes[ds.n]->TENLOP, lop.TENLOP);
    ds.nodes[ds.n]->FirstSV = nullptr; // Lớp mới chưa có sinh viên nào
    
    ds.n++;
    return true;
}

bool SuaLop(DS_LOPSV &ds, char MALOP[], char TENLOP_MOI[]) {
    ChuanHoaMa(MALOP);
    int idx = TimLop(ds, MALOP);
    if (idx == -1) {
        cout << "Loi: Khong tim thay ma lop!\n";
        return false;
    }
    
    ChuanHoaTen(TENLOP_MOI);
    if (KiemTraRong(TENLOP_MOI)) {
        cout << "Loi: Ten lop khong duoc de trong!\n";
        return false;
    }
    
    strcpy(ds.nodes[idx]->TENLOP, TENLOP_MOI);
    return true;
}

void InDanhSachLop(DS_LOPSV ds) {
    if (ds.n == 0) {
        cout << "Danh sach lop rong!\n";
        return;
    }
    
    cout << "\n--- DANH SACH LOP ---\n";
    cout << setw(15) << left << "MALOP" 
         << setw(50) << left << "TEN LOP" << endl;
    cout << "----------------------------------------------------------------\n";
    
    for (int i = 0; i < ds.n; i++) {
        cout << setw(15) << left << ds.nodes[i]->MALOP 
             << setw(50) << left << ds.nodes[i]->TENLOP << endl;
    }
}

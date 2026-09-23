#include "sinhvien.h"
#include "utils.h"
#include "rang_buoc.h"
#include <iostream>
#include <string.h>
#include <iomanip>

using namespace std;

void KhoiTaoDanhSachSV(PTRSV &First) {
    First = nullptr;
}

PTRSV TimSV(PTRSV First, char MASV[]) {
    PTRSV p = First;
    while (p != nullptr) {
        if (strcmp(p->sv.MASV, MASV) == 0) return p;
        p = p->next;
    }
    return nullptr;
}

int SoSanhTen(SinhVien a, SinhVien b) {
    int cmpTen = stricmp(a.TEN, b.TEN);
    if (cmpTen != 0) return cmpTen;
    int cmpHo = stricmp(a.HO, b.HO);
    if (cmpHo != 0) return cmpHo;
    return stricmp(a.MASV, b.MASV);
}

void InsertOrderSV(PTRSV &First, SinhVien sv) {
    PTRSV p = new nodeSV;
    p->sv = sv;
    p->next = nullptr;
    
    // Danh sách rỗng hoặc chèn vào đầu
    if (First == nullptr || SoSanhTen(First->sv, p->sv) > 0) {
        p->next = First;
        First = p;
        return;
    }
    
    // Tìm vị trí chèn ở giữa hoặc cuối
    PTRSV curr = First;
    while (curr->next != nullptr && SoSanhTen(curr->next->sv, p->sv) < 0) {
        curr = curr->next;
    }
    
    p->next = curr->next;
    curr->next = p;
}

void NhapDanhSachSV(PTRSV &First) {
    SinhVien sv;
    while (true) {
        cout << "\nNhap Ma SV (de trong de thoat): ";
        cin.getline(sv.MASV, 16);
        ChuanHoaMa(sv.MASV);
        if (KiemTraRong(sv.MASV)) break;
        
        if (TimSV(First, sv.MASV) != nullptr) {
            cout << "[!] Loi: Ma sinh vien '" << sv.MASV << "' da ton tai!\n";
            continue;
        }
        
        do {
            cout << "Nhap Ho: ";
            cin.getline(sv.HO, 51);
            ChuanHoaTen(sv.HO);
            if (KiemTraRong(sv.HO)) cout << "[!] Loi: Ho khong duoc de trong!\n";
        } while (KiemTraRong(sv.HO));
        
        do {
            cout << "Nhap Ten: ";
            cin.getline(sv.TEN, 16);
            ChuanHoaTen(sv.TEN);
            if (KiemTraRong(sv.TEN)) cout << "[!] Loi: Ten khong duoc de trong!\n";
        } while (KiemTraRong(sv.TEN));
        
        do {
            cout << "Nhap Phai (NAM/NU): ";
            cin.getline(sv.PHAI, 4);
            ChuanHoaMa(sv.PHAI);
            if (strcmp(sv.PHAI, "NAM") != 0 && strcmp(sv.PHAI, "NU") != 0) {
                cout << "[!] Loi: Phai chi duoc nhap NAM hoac NU!\n";
            }
        } while (strcmp(sv.PHAI, "NAM") != 0 && strcmp(sv.PHAI, "NU") != 0);
        
        do {
            cout << "Nhap So DT: ";
            cin.getline(sv.SODT, 16);
            ChuanHoaMa(sv.SODT);
            if (KiemTraRong(sv.SODT)) cout << "[!] Loi: So dien thoai khong duoc de trong!\n";
        } while (KiemTraRong(sv.SODT));
        
        InsertOrderSV(First, sv);
        cout << "[OK] Da them sinh vien '" << sv.MASV << "' thanh cong!\n";
    }
}

// Xóa không in thông báo (dùng nội bộ khi SuaSV cần tách node rồi chèn lại)
bool XoaSVNoiB(PTRSV &First, char MASV[]) {
    if (First == nullptr) return false;
    
    if (strcmp(First->sv.MASV, MASV) == 0) {
        PTRSV p = First;
        First = First->next;
        delete p;
        return true;
    }
    
    PTRSV curr = First;
    while (curr->next != nullptr && strcmp(curr->next->sv.MASV, MASV) != 0) {
        curr = curr->next;
    }
    
    if (curr->next != nullptr) {
        PTRSV p = curr->next;
        curr->next = p->next;
        delete p;
        return true;
    }
    return false;
}

void XoaSV(PTRSV &First, char MASV[], DS_LopTC dsltc) {
    ChuanHoaMa(MASV);
    if (First == nullptr) {
        cout << "[!] Loi: Danh sach sinh vien rong!\n";
        return;
    }
    
    // 1. Kiểm tra MASV có tồn tại không
    if (TimSV(First, MASV) == nullptr) {
        cout << "[!] Loi: Khong tim thay sinh vien co ma '" << MASV << "'!\n";
        return;
    }
    
    // 2. Kiểm tra SV có đang đăng ký lớp tín chỉ nào không
    int soDK = DemDangKyByMASV(dsltc, MASV);
    if (soDK > 0) {
        cout << "[!] Khong the xoa! Sinh vien '" << MASV << "' dang dang ky " << soDK << " lop tin chi.\n";
        InRangBuocSV(dsltc, MASV);
        
        // Kiểm tra thêm: đã có điểm chưa
        if (SVDaCoHiem(dsltc, MASV)) {
            cout << "[!] Sinh vien da co diem thi. Hay huy dang ky truoc khi xoa.\n";
        } else {
            cout << "[!] Hay huy cac dang ky cua sinh vien truoc khi xoa.\n";
        }
        return;
    }
    
    // 3. Không có ràng buộc → cho xóa
    if (XoaSVNoiB(First, MASV)) {
        cout << "[OK] Da xoa sinh vien '" << MASV << "' thanh cong!\n";
    }
}

void SuaSV(PTRSV &First, char MASV[]) {
    ChuanHoaMa(MASV);
    PTRSV p = TimSV(First, MASV);
    if (p == nullptr) {
        cout << "[!] Loi: Khong tim thay sinh vien co ma '" << MASV << "'!\n";
        return;
    }
    
    SinhVien temp = p->sv;
    cout << "Sua thong tin sinh vien " << MASV << " (Nhan Enter de giu nguyen)\n";
    
    char input[51];
    
    cout << "Ho moi (" << temp.HO << "): ";
    cin.getline(input, 51);
    ChuanHoaTen(input);
    if (!KiemTraRong(input)) strcpy(temp.HO, input);
    
    cout << "Ten moi (" << temp.TEN << "): ";
    cin.getline(input, 51);
    ChuanHoaTen(input);
    if (!KiemTraRong(input)) strcpy(temp.TEN, input);
    
    cout << "Phai moi (" << temp.PHAI << "): ";
    cin.getline(input, 4);
    ChuanHoaMa(input);
    if (!KiemTraRong(input)) {
        if (strcmp(input, "NAM") == 0 || strcmp(input, "NU") == 0) {
            strcpy(temp.PHAI, input);
        } else {
            cout << "[!] Phai khong hop le, giu nguyen gia tri cu.\n";
        }
    }
    
    cout << "So DT moi (" << temp.SODT << "): ";
    cin.getline(input, 16);
    ChuanHoaMa(input);
    if (!KiemTraRong(input)) strcpy(temp.SODT, input);
    
    // Nếu Tên hoặc Họ thay đổi → vị trí trong SLL sai thứ tự → tách ra rồi chèn lại
    if (stricmp(temp.TEN, p->sv.TEN) != 0 || stricmp(temp.HO, p->sv.HO) != 0) {
        XoaSVNoiB(First, MASV); // Xóa im lặng, không in thông báo
        InsertOrderSV(First, temp);
    } else {
        p->sv = temp;
    }
    cout << "[OK] Da cap nhat sinh vien '" << MASV << "' thanh cong!\n";
}

void XuatDanhSachSinhVien(PTRSV First) {
    if (First == nullptr) {
        cout << "[!] Danh sach sinh vien rong!\n";
        return;
    }
    
    int stt = 1;
    cout << "\n--- DANH SACH SINH VIEN (SAP XEP TANG DAN THEO TEN) ---\n";
    cout << setw(5) << left << "STT"
         << setw(15) << left << "MASV" 
         << setw(35) << left << "HO VA TEN"
         << setw(10) << left << "PHAI"
         << setw(15) << left << "SO DT" << endl;
    cout << "-------------------------------------------------------------------------------\n";
    
    for (PTRSV p = First; p != nullptr; p = p->next) {
        char hoten[67];
        strcpy(hoten, p->sv.HO);
        strcat(hoten, " ");
        strcat(hoten, p->sv.TEN);
        
        cout << setw(5) << left << stt++
             << setw(15) << left << p->sv.MASV 
             << setw(35) << left << hoten
             << setw(10) << left << p->sv.PHAI
             << setw(15) << left << p->sv.SODT << endl;
    }
    cout << "=> Tong cong: " << (stt - 1) << " sinh vien.\n";
}

void QuickSortSV(PTRSV* arr, int low, int high) {
    if (low < high) {
        // Chọn pivot giữa để tránh worst case O(N^2) khi data đã sắp xếp
        int mid = low + (high - low) / 2;
        PTRSV t = arr[mid]; arr[mid] = arr[high]; arr[high] = t;
        
        char* pivot = arr[high]->sv.MASV;
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (stricmp(arr[j]->sv.MASV, pivot) < 0) {
                i++;
                PTRSV temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        PTRSV temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        
        int pi = i + 1;
        QuickSortSV(arr, low, pi - 1);
        QuickSortSV(arr, pi + 1, high);
    }
}

void InDSSVTheoMa(PTRSV First) {
    if (First == nullptr) {
        cout << "[!] Danh sach sinh vien rong!\n";
        return;
    }
    
    int count = 0;
    for (PTRSV p = First; p != nullptr; p = p->next) count++;
    
    PTRSV* arr = new PTRSV[count];
    int i = 0;
    for (PTRSV p = First; p != nullptr; p = p->next) {
        arr[i++] = p;
    }
    
    if (count > 1) QuickSortSV(arr, 0, count - 1);
    
    int stt = 1;
    cout << "\n--- DANH SACH SINH VIEN (SAP XEP TANG DAN THEO MA SV) ---\n";
    cout << setw(5) << left << "STT"
         << setw(15) << left << "MASV" 
         << setw(35) << left << "HO VA TEN"
         << setw(10) << left << "PHAI"
         << setw(15) << left << "SO DT" << endl;
    cout << "-------------------------------------------------------------------------------\n";
    
    for (int k = 0; k < count; k++) {
        char hoten[67];
        strcpy(hoten, arr[k]->sv.HO);
        strcat(hoten, " ");
        strcat(hoten, arr[k]->sv.TEN);
        
        cout << setw(5) << left << stt++
             << setw(15) << left << arr[k]->sv.MASV 
             << setw(35) << left << hoten
             << setw(10) << left << arr[k]->sv.PHAI
             << setw(15) << left << arr[k]->sv.SODT << endl;
    }
    cout << "=> Tong cong: " << count << " sinh vien.\n";
    
    delete[] arr;
}

int DemSV(PTRSV First) {
    int count = 0;
    for (PTRSV p = First; p != nullptr; p = p->next) count++;
    return count;
}

void GiaiPhongDSSV(PTRSV &First) {
    while (First != nullptr) {
        PTRSV p = First;
        First = First->next;
        delete p;
    }
}
#include "sinhvien.h"
#include <iostream>
#include <string.h>
#include <iomanip>

// Nếu compiler báo lỗi stricmp, đổi thành _stricmp (trên MSVC) hoặc tự định nghĩa.
// Đối với MinGW/GCC thường hỗ trợ stricmp.

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
    // So sánh Tên
    int cmpTen = stricmp(a.TEN, b.TEN);
    if (cmpTen != 0) return cmpTen;
    // Nếu trùng Tên, so sánh Họ
    int cmpHo = stricmp(a.HO, b.HO);
    if (cmpHo != 0) return cmpHo;
    // Nếu trùng Họ, so sánh Mã SV
    return stricmp(a.MASV, b.MASV);
}

void InsertOrderSV(PTRSV &First, SinhVien sv) {
    PTRSV p = new nodeSV;
    p->sv = sv;
    p->next = nullptr;
    
    // Nếu danh sách rỗng hoặc sinh viên mới nhỏ hơn sinh viên đứng đầu
    if (First == nullptr || SoSanhTen(First->sv, p->sv) > 0) {
        p->next = First;
        First = p;
        return;
    }
    
    PTRSV curr = First;
    // Tìm vị trí chèn
    while (curr->next != nullptr && SoSanhTen(curr->next->sv, p->sv) < 0) {
        curr = curr->next;
    }
    
    // Chèn p vào sau curr
    p->next = curr->next;
    curr->next = p;
}

void NhapDanhSachSV(PTRSV &First) {
    SinhVien sv;
    while (true) {
        cout << "Nhap Ma SV (de trong de thoat): ";
        cin.getline(sv.MASV, 16);
        if (strlen(sv.MASV) == 0) break;
        
        if (TimSV(First, sv.MASV) != nullptr) {
            cout << "Loi: Ma sinh vien da ton tai!\n";
            continue;
        }
        
        cout << "Nhap Ho: ";
        cin.getline(sv.HO, 51);
        cout << "Nhap Ten: ";
        cin.getline(sv.TEN, 16);
        cout << "Nhap Phai (NAM/NU): ";
        cin.getline(sv.PHAI, 4);
        cout << "Nhap So DT: ";
        cin.getline(sv.SODT, 16);
        
        InsertOrderSV(First, sv);
        cout << "=> Da them sinh vien thanh cong!\n\n";
    }
}

void XoaSV(PTRSV &First, char MASV[]) {
    if (First == nullptr) {
        cout << "Loi: Danh sach rong!\n";
        return;
    }
    
    if (strcmp(First->sv.MASV, MASV) == 0) {
        PTRSV p = First;
        First = First->next;
        delete p;
        cout << "=> Da xoa sinh vien thanh cong!\n";
        return;
    }
    
    PTRSV curr = First;
    while (curr->next != nullptr && strcmp(curr->next->sv.MASV, MASV) != 0) {
        curr = curr->next;
    }
    
    if (curr->next != nullptr) {
        PTRSV p = curr->next;
        curr->next = p->next;
        delete p;
        cout << "=> Da xoa sinh vien thanh cong!\n";
    } else {
        cout << "Loi: Khong tim thay ma sinh vien!\n";
    }
}

void SuaSV(PTRSV &First, char MASV[]) {
    PTRSV p = TimSV(First, MASV);
    if (p == nullptr) {
        cout << "Loi: Khong tim thay ma sinh vien!\n";
        return;
    }
    
    SinhVien temp = p->sv;
    cout << "Sua thong tin sinh vien " << MASV << " (Nhan Enter de giu nguyen)\n";
    
    char input[51];
    
    cout << "Ho moi (" << temp.HO << "): ";
    cin.getline(input, 51);
    if (strlen(input) > 0) strcpy(temp.HO, input);
    
    cout << "Ten moi (" << temp.TEN << "): ";
    cin.getline(input, 16);
    if (strlen(input) > 0) strcpy(temp.TEN, input);
    
    cout << "Phai moi (" << temp.PHAI << "): ";
    cin.getline(input, 4);
    if (strlen(input) > 0) strcpy(temp.PHAI, input);
    
    cout << "So DT moi (" << temp.SODT << "): ";
    cin.getline(input, 16);
    if (strlen(input) > 0) strcpy(temp.SODT, input);
    
    // Nếu tên hoặc họ thay đổi, vị trí trong SLL thay đổi -> Xóa node cũ rồi chèn lại
    if (stricmp(temp.TEN, p->sv.TEN) != 0 || stricmp(temp.HO, p->sv.HO) != 0) {
        XoaSV(First, MASV);
        InsertOrderSV(First, temp);
    } else {
        p->sv = temp;
    }
    cout << "=> Da cap nhat sinh vien thanh cong!\n";
}

void XuatDanhSachSinhVien(PTRSV First) {
    if (First == nullptr) {
        cout << "Danh sach sinh vien rong!\n";
        return;
    }
    
    cout << "\n--- DANH SACH SINH VIEN (SAP XEP TANG DAN THEO TEN) ---\n";
    cout << setw(15) << left << "MASV" 
         << setw(35) << left << "HO VA TEN"
         << setw(10) << left << "PHAI"
         << setw(15) << left << "SO DT" << endl;
    cout << "------------------------------------------------------------------------\n";
    
    for (PTRSV p = First; p != nullptr; p = p->next) {
        char hoten[67];
        strcpy(hoten, p->sv.HO);
        strcat(hoten, " ");
        strcat(hoten, p->sv.TEN);
        
        cout << setw(15) << left << p->sv.MASV 
             << setw(35) << left << hoten
             << setw(10) << left << p->sv.PHAI
             << setw(15) << left << p->sv.SODT << endl;
    }
}

void QuickSortSV(PTRSV* arr, int low, int high) {
    if (low < high) {
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
        cout << "Danh sach sinh vien rong!\n";
        return;
    }
    
    // 1. Đếm
    int count = 0;
    for (PTRSV p = First; p != nullptr; p = p->next) count++;
    
    // 2. Tạo mảng con trỏ tạm
    PTRSV* arr = new PTRSV[count];
    int i = 0;
    for (PTRSV p = First; p != nullptr; p = p->next) {
        arr[i++] = p;
    }
    
    // 3. Sắp xếp mảng tạm theo MASV bằng Quick Sort O(N log N)
    QuickSortSV(arr, 0, count - 1);
    
    // 4. In kết quả
    cout << "\n--- DANH SACH SINH VIEN (SAP XEP TANG DAN THEO MA SV) ---\n";
    cout << setw(15) << left << "MASV" 
         << setw(35) << left << "HO VA TEN"
         << setw(10) << left << "PHAI"
         << setw(15) << left << "SO DT" << endl;
    cout << "------------------------------------------------------------------------\n";
    
    for (int k = 0; k < count; k++) {
        char hoten[67];
        strcpy(hoten, arr[k]->sv.HO);
        strcat(hoten, " ");
        strcat(hoten, arr[k]->sv.TEN);
        
        cout << setw(15) << left << arr[k]->sv.MASV 
             << setw(35) << left << hoten
             << setw(10) << left << arr[k]->sv.PHAI
             << setw(15) << left << arr[k]->sv.SODT << endl;
    }
    
    // 5. Giải phóng mảng
    delete[] arr;
}
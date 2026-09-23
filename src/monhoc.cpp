#include "monhoc.h"
#include <iostream>
#include <string.h>
#include <iomanip>

using namespace std;

void KhoiTaoCayMonHoc(TreeMonHoc &root) {
    root = nullptr;
}

TreeMonHoc TimMonHoc(TreeMonHoc root, char MAMH[]) {
    if (root == nullptr) return nullptr;
    int cmp = stricmp(MAMH, root->mh.MAMH);
    if (cmp == 0) return root;
    if (cmp < 0) return TimMonHoc(root->left, MAMH);
    return TimMonHoc(root->right, MAMH);
}

void ThemMonHoc(TreeMonHoc &root, MonHoc mh, int &n) {
    if (root == nullptr) {
        root = new nodeMonHoc;
        root->mh = mh;
        root->left = nullptr;
        root->right = nullptr;
        n++;
        return;
    }
    
    int cmp = stricmp(mh.MAMH, root->mh.MAMH);
    if (cmp < 0) {
        ThemMonHoc(root->left, mh, n);
    } else if (cmp > 0) {
        ThemMonHoc(root->right, mh, n);
    } else {
        cout << "Loi: Ma mon hoc da ton tai!\n";
    }
}

void NhapMonHoc(DS_MonHoc &ds) {
    MonHoc mh;
    while (true) {
        cout << "Nhap Ma Mon Hoc (de trong de thoat): ";
        cin.getline(mh.MAMH, 11);
        if (strlen(mh.MAMH) == 0) break;
        
        if (TimMonHoc(ds.root, mh.MAMH) != nullptr) {
            cout << "Loi: Ma mon hoc da ton tai!\n";
            continue;
        }
        
        cout << "Nhap Ten Mon Hoc: ";
        cin.getline(mh.TENMH, 51);
        
        cout << "Nhap So TC Ly Thuyet: ";
        cin >> mh.STCLT;
        cout << "Nhap So TC Thuc Hanh: ";
        cin >> mh.STCTH;
        cin.ignore(); // Xóa bộ đệm sau khi nhập số
        
        ThemMonHoc(ds.root, mh, ds.n);
        cout << "=> Da them mon hoc thanh cong!\n\n";
    }
}

void SuaMonHoc(TreeMonHoc root, char MAMH[]) {
    TreeMonHoc p = TimMonHoc(root, MAMH);
    if (p == nullptr) {
        cout << "Loi: Khong tim thay ma mon hoc!\n";
        return;
    }
    
    cout << "Sua thong tin mon hoc " << MAMH << " (Nhan Enter hoac -1 de giu nguyen)\n";
    char input[51];
    
    cout << "Ten mon moi (" << p->mh.TENMH << "): ";
    cin.getline(input, 51);
    if (strlen(input) > 0) strcpy(p->mh.TENMH, input);
    
    cout << "So TC Ly Thuyet moi (" << p->mh.STCLT << "): ";
    cin.getline(input, 51);
    if (strlen(input) > 0) p->mh.STCLT = atoi(input);
    
    cout << "So TC Thuc Hanh moi (" << p->mh.STCTH << "): ";
    cin.getline(input, 51);
    if (strlen(input) > 0) p->mh.STCTH = atoi(input);
    
    cout << "=> Da cap nhat mon hoc thanh cong!\n";
}

// Hàm phụ trợ tìm node thế mạng (node phải nhất của cây con trái)
void NodeTheMang(TreeMonHoc &p, TreeMonHoc &rightMost) {
    if (rightMost->right != nullptr) {
        NodeTheMang(p, rightMost->right);
    } else {
        p->mh = rightMost->mh; // Copy data
        p = rightMost;         // Trỏ p tới node cần xóa thực sự
        rightMost = rightMost->left;
    }
}

bool XoaMonHoc(TreeMonHoc &root, char MAMH[], int &n) {
    if (root == nullptr) return false;
    
    int cmp = stricmp(MAMH, root->mh.MAMH);
    if (cmp < 0) {
        return XoaMonHoc(root->left, MAMH, n);
    } else if (cmp > 0) {
        return XoaMonHoc(root->right, MAMH, n);
    } else {
        // Tìm thấy node cần xóa
        TreeMonHoc p = root;
        
        if (root->left == nullptr) {
            root = root->right;
        } else if (root->right == nullptr) {
            root = root->left;
        } else {
            // Node có 2 con: tìm node thế mạng ở cây con trái
            NodeTheMang(p, root->left);
        }
        delete p;
        n--;
        return true;
    }
}

// Hàm duyệt LNR (In-order) nạp các node vào mảng
void DoCayRaMang(TreeMonHoc root, TreeMonHoc* arr, int &idx) {
    if (root != nullptr) {
        DoCayRaMang(root->left, arr, idx);
        arr[idx++] = root;
        DoCayRaMang(root->right, arr, idx);
    }
}

void InDanhSachMonHoc(TreeMonHoc root, int n) {
    if (root == nullptr || n == 0) {
        cout << "Danh sach mon hoc rong!\n";
        return;
    }
    
    // Cấp phát mảng con trỏ
    TreeMonHoc* arr = new TreeMonHoc[n];
    int idx = 0;
    
    // Đổ cây vào mảng
    DoCayRaMang(root, arr, idx);
    
    // Sắp xếp mảng theo tên môn học (Selection Sort)
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (stricmp(arr[i]->mh.TENMH, arr[j]->mh.TENMH) > 0) {
                TreeMonHoc temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
    
    // In danh sách
    cout << "\n--- DANH SACH MON HOC (SAP XEP TANG DAN THEO TEN) ---\n";
    cout << setw(15) << left << "MAMH" 
         << setw(40) << left << "TEN MON HOC"
         << setw(10) << left << "STC LT"
         << setw(10) << left << "STC TH" << endl;
    cout << "--------------------------------------------------------------------------\n";
    
    for (int i = 0; i < n; i++) {
        cout << setw(15) << left << arr[i]->mh.MAMH 
             << setw(40) << left << arr[i]->mh.TENMH
             << setw(10) << left << arr[i]->mh.STCLT
             << setw(10) << left << arr[i]->mh.STCTH << endl;
    }
    
    delete[] arr;
}

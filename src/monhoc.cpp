#include "monhoc.h"
#include "utils.h"
#include "rang_buoc.h"
#include <iostream>
#include <string.h>
#include <iomanip>

using namespace std;

void KhoiTaoCayMonHoc(TreeMonHoc &root) {
    root = nullptr;
}

TreeMonHoc TimMonHoc(TreeMonHoc root, const char MAMH[]) {
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
        cout << "[!] Loi: Ma mon hoc '" << mh.MAMH << "' da ton tai!\n";
    }
}

void NhapMonHoc(DS_MonHoc &ds) {
    MonHoc mh;
    while (true) {
        cout << "\nNhap Ma Mon Hoc (de trong de thoat): ";
        cin.getline(mh.MAMH, 11);
        ChuanHoaMa(mh.MAMH);
        if (KiemTraRong(mh.MAMH)) break;
        
        if (TimMonHoc(ds.root, mh.MAMH) != nullptr) {
            cout << "[!] Loi: Ma mon hoc '" << mh.MAMH << "' da ton tai!\n";
            continue;
        }
        
        do {
            cout << "Nhap Ten Mon Hoc: ";
            cin.getline(mh.TENMH, 51);
            ChuanHoaTen(mh.TENMH);
            if (KiemTraRong(mh.TENMH)) cout << "[!] Loi: Ten mon hoc khong duoc de trong!\n";
        } while (KiemTraRong(mh.TENMH));
        
        do {
            mh.STCLT = NhapSoNguyen("Nhap So TC Ly Thuyet (>= 0): ");
            if (mh.STCLT < 0) cout << "[!] Loi: So tin chi khong duoc am!\n";
        } while (mh.STCLT < 0);
        
        do {
            mh.STCTH = NhapSoNguyen("Nhap So TC Thuc Hanh (>= 0): ");
            if (mh.STCTH < 0) cout << "[!] Loi: So tin chi khong duoc am!\n";
        } while (mh.STCTH < 0);
        
        if (mh.STCLT + mh.STCTH == 0) {
            cout << "[!] Loi: Tong so tin chi phai lon hon 0!\n";
            continue;
        }
        
        ThemMonHoc(ds.root, mh, ds.n);
        cout << "[OK] Da them mon hoc '" << mh.MAMH << "' thanh cong!\n";
    }
}

void SuaMonHoc(TreeMonHoc root, const char MAMH[]) {
    char ma[11];
    strcpy(ma, MAMH);
    ChuanHoaMa(ma);
    TreeMonHoc p = TimMonHoc(root, ma);
    if (p == nullptr) {
        cout << "[!] Loi: Khong tim thay mon hoc co ma '" << ma << "'!\n";
        return;
    }
    
    cout << "Sua thong tin mon hoc " << ma << " (Nhan Enter de giu nguyen)\n";
    char input[51];
    
    cout << "Ten mon moi (" << p->mh.TENMH << "): ";
    cin.getline(input, 51);
    ChuanHoaTen(input);
    if (!KiemTraRong(input)) strcpy(p->mh.TENMH, input);
    
    int stclt_moi = p->mh.STCLT;
    int stcth_moi = p->mh.STCTH;

    cout << "So TC Ly Thuyet moi (" << p->mh.STCLT << "): ";
    cin.getline(input, 51);
    ChuanHoaMa(input);
    if (!KiemTraRong(input)) {
        int val = atoi(input);
        if (val >= 0) {
            stclt_moi = val;
        } else {
            cout << "[!] Gia tri khong hop le, giu nguyen.\n";
        }
    }
    
    cout << "So TC Thuc Hanh moi (" << p->mh.STCTH << "): ";
    cin.getline(input, 51);
    ChuanHoaMa(input);
    if (!KiemTraRong(input)) {
        int val = atoi(input);
        if (val >= 0) {
            stcth_moi = val;
        } else {
            cout << "[!] Gia tri khong hop le, giu nguyen.\n";
        }
    }

    if (stclt_moi + stcth_moi == 0) {
        cout << "[!] Loi: Tong so tin chi (LT + TH) phai lon hon 0! Khong the cap nhat tin chi bang 0.\n";
    } else {
        p->mh.STCLT = stclt_moi;
        p->mh.STCTH = stcth_moi;
    }
    
    cout << "[OK] Da cap nhat mon hoc '" << ma << "' thanh cong!\n";
}

// Hàm phụ trợ tìm node thế mạng (node phải nhất của cây con trái)
void NodeTheMang(TreeMonHoc &p, TreeMonHoc &rightMost) {
    if (rightMost->right != nullptr) {
        NodeTheMang(p, rightMost->right);
    } else {
        p->mh = rightMost->mh;
        p = rightMost;
        rightMost = rightMost->left;
    }
}

bool XoaMonHocNoiB(TreeMonHoc &root, const char MAMH[], int &n) {
    if (root == nullptr) return false;
    
    int cmp = stricmp(MAMH, root->mh.MAMH);
    if (cmp < 0) {
        return XoaMonHocNoiB(root->left, MAMH, n);
    } else if (cmp > 0) {
        return XoaMonHocNoiB(root->right, MAMH, n);
    } else {
        TreeMonHoc p = root;
        
        if (root->left == nullptr) {
            root = root->right;        
        } else if (root->right == nullptr) {
            root = root->left;         
        } else {
            NodeTheMang(p, root->left); 
        }
        delete p;
        n--;
        return true;
    }
}

bool XoaMonHoc(TreeMonHoc &root, const char MAMH[], int &n, const DS_LopTC &dsltc) {
    char ma[11];
    strcpy(ma, MAMH);
    ChuanHoaMa(ma);
    if (root == nullptr) {
        cout << "[!] Loi: Danh sach mon hoc rong!\n";
        return false;
    }
    
    if (TimMonHoc(root, ma) == nullptr) {
        cout << "[!] Loi: Khong tim thay mon hoc co ma '" << ma << "'!\n";
        return false;
    }
    
    int soLopTC = DemLopTCByMAMH(dsltc, ma);
    if (soLopTC > 0) {
        cout << "[!] Khong the xoa! Mon hoc '" << ma << "' dang duoc su dung boi " << soLopTC << " lop tin chi.\n";
        InRangBuocMonHoc(dsltc, ma);
        return false;
    }
    
    if (XoaMonHocNoiB(root, ma, n)) {
        cout << "[OK] Da xoa mon hoc '" << ma << "' thanh cong!\n";
        return true;
    }
    return false;
}

void InOrderTraversal(TreeMonHoc root, TreeMonHoc* arr, int &idx) {
    if (root != nullptr) {
        InOrderTraversal(root->left, arr, idx);
        arr[idx++] = root;
        InOrderTraversal(root->right, arr, idx);
    }
}

void QuickSortMH(TreeMonHoc* arr, int low, int high) {
    if (low < high) {
        // Chọn pivot giữa để tránh worst case
        int mid = low + (high - low) / 2;
        TreeMonHoc t = arr[mid]; arr[mid] = arr[high]; arr[high] = t;
        
        char* pivot = arr[high]->mh.TENMH;
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (stricmp(arr[j]->mh.TENMH, pivot) < 0) {
                i++;
                TreeMonHoc temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        TreeMonHoc temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;
        
        int pi = i + 1;
        QuickSortMH(arr, low, pi - 1);
        QuickSortMH(arr, pi + 1, high);
    }
}

void InDanhSachMonHoc(TreeMonHoc root, int n) {
    if (root == nullptr || n == 0) {
        cout << "[!] Danh sach mon hoc rong!\n";
        return;
    }
    
    TreeMonHoc* arr = new TreeMonHoc[n];
    int idx = 0;
    InOrderTraversal(root, arr, idx);
    
    if (n > 1) QuickSortMH(arr, 0, n - 1);
    
    int stt = 1;
    cout << "\n--- DANH SACH MON HOC (SAP XEP TANG DAN THEO TEN) ---\n";
    cout << setw(5) << left << "STT"
         << setw(15) << left << "MAMH" 
         << setw(40) << left << "TEN MON HOC"
         << setw(10) << left << "STC LT"
         << setw(10) << left << "STC TH" << endl;
    cout << "-------------------------------------------------------------------------------\n";
    
    for (int i = 0; i < n; i++) {
        cout << setw(5) << left << stt++
             << setw(15) << left << arr[i]->mh.MAMH 
             << setw(40) << left << arr[i]->mh.TENMH
             << setw(10) << left << arr[i]->mh.STCLT
             << setw(10) << left << arr[i]->mh.STCTH << endl;
    }
    cout << "=> Tong cong: " << n << " mon hoc.\n";
    
    delete[] arr;
}

// Giải phóng toàn bộ cây BST
void GiaiPhongCay(TreeMonHoc &root) {
    if (root != nullptr) {
        GiaiPhongCay(root->left);
        GiaiPhongCay(root->right);
        delete root;
        root = nullptr;
    }
}

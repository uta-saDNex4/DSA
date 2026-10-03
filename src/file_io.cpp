#include "file_io.h"
#include "monhoc.h"
#include "sinhvien.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

// --- FILE I/O CHO MÔN HỌC ---

void GhiCayRaFile(TreeMonHoc root, ofstream &outFile) {
    if (root != nullptr) {
        GhiCayRaFile(root->left, outFile);
        outFile << root->mh.MAMH << "|"
                << root->mh.TENMH << "|"
                << root->mh.STCLT << "|"
                << root->mh.STCTH << "\n";
        GhiCayRaFile(root->right, outFile);
    }
}

bool GhiFileMonHoc(TreeMonHoc root, const char* filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "[!] Loi: Khong the mo file '" << filename << "' de ghi!\n";
        return false;
    }
    GhiCayRaFile(root, outFile);
    outFile.close();
    cout << "[OK] Da luu danh sach mon hoc vao file '" << filename << "'.\n";
    return true;
}

bool DocFileMonHoc(DS_MonHoc &ds, const char* filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "[!] Loi: File '" << filename << "' khong ton tai hoac khong the mo!\n";
        return false;
    }
    
    // Giải phóng cây cũ nếu có
    GiaiPhongCay(ds.root);
    ds.n = 0;
    
    MonHoc mh;
    char buffer[256];
    int dong = 0;
    while (inFile.getline(buffer, 256)) {
        if (strlen(buffer) == 0) continue;
        dong++;
        
        char* token = strtok(buffer, "|");
        if (!token) { cout << "[!] Canh bao: Dong " << dong << " du lieu sai dinh dang, bo qua.\n"; continue; }
        strcpy(mh.MAMH, token);
        
        token = strtok(NULL, "|");
        if (!token) { cout << "[!] Canh bao: Dong " << dong << " thieu ten mon hoc, bo qua.\n"; continue; }
        strcpy(mh.TENMH, token);
        
        token = strtok(NULL, "|");
        if (!token) { cout << "[!] Canh bao: Dong " << dong << " thieu STC LT, bo qua.\n"; continue; }
        mh.STCLT = atoi(token);
        
        token = strtok(NULL, "|");
        if (!token) { cout << "[!] Canh bao: Dong " << dong << " thieu STC TH, bo qua.\n"; continue; }
        mh.STCTH = atoi(token);
        
        ThemMonHoc(ds.root, mh, ds.n);
    }
    
    inFile.close();
    cout << "[OK] Da doc " << ds.n << " mon hoc tu file '" << filename << "'.\n";
    return true;
}

// --- FILE I/O CHO LỚP VÀ SINH VIÊN ---

bool GhiFileLopVaSinhVien(DS_LOPSV ds, const char* filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "[!] Loi: Khong the mo file '" << filename << "' de ghi!\n";
        return false;
    }
    
    outFile << ds.n << "\n";
    for (int i = 0; i < ds.n; i++) {
        LOPSV* lop = ds.nodes[i];
        outFile << lop->MALOP << "|" << lop->TENLOP << "\n";
        
        for (PTRSV p = lop->FirstSV; p != nullptr; p = p->next) {
            outFile << p->sv.MASV << "|"
                    << p->sv.HO << "|"
                    << p->sv.TEN << "|"
                    << p->sv.PHAI << "|"
                    << p->sv.SODT << "\n";
        }
        outFile << "END_LOP\n";
    }
    
    outFile.close();
    cout << "[OK] Da luu " << ds.n << " lop vao file '" << filename << "'.\n";
    return true;
}

bool DocFileLopVaSinhVien(DS_LOPSV &ds, const char* filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "[!] Loi: File '" << filename << "' khong ton tai hoac khong the mo!\n";
        return false;
    }
    
    // Giải phóng data cũ
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr) {
            GiaiPhongDSSV(ds.nodes[i]->FirstSV);
            delete ds.nodes[i];
            ds.nodes[i] = nullptr;
        }
    }
    ds.n = 0;
    
    char buffer[256];
    inFile.getline(buffer, 256);
    if (strlen(buffer) == 0) {
        cout << "[!] Canh bao: File rong, khong co du lieu.\n";
        inFile.close();
        return true;
    }
    int soLop = atoi(buffer);
    
    for (int i = 0; i < soLop; i++) {
        if (!inFile.getline(buffer, 256)) break; // EOF bất ngờ
        
        ds.nodes[i] = new LOPSV;
        
        char* token = strtok(buffer, "|");
        if (token) strcpy(ds.nodes[i]->MALOP, token);
        
        token = strtok(NULL, "|");
        if (token) strcpy(ds.nodes[i]->TENLOP, token);
        
        ds.nodes[i]->FirstSV = nullptr;
        
        while (inFile.getline(buffer, 256)) {
            if (strcmp(buffer, "END_LOP") == 0) break;
            if (strlen(buffer) == 0) continue;
            
            SinhVien sv;
            token = strtok(buffer, "|");
            if (!token) continue;
            strcpy(sv.MASV, token);
            
            token = strtok(NULL, "|");
            if (!token) continue;
            strcpy(sv.HO, token);
            
            token = strtok(NULL, "|");
            if (!token) continue;
            strcpy(sv.TEN, token);
            
            token = strtok(NULL, "|");
            if (!token) continue;
            strcpy(sv.PHAI, token);
            
            token = strtok(NULL, "|");
            if (!token) continue;
            strcpy(sv.SODT, token);
            
            InsertOrderSV(ds.nodes[i]->FirstSV, sv);
        }
        ds.n++;
    }
    
    inFile.close();
    cout << "[OK] Da doc " << ds.n << " lop tu file '" << filename << "'.\n";
    return true;
}

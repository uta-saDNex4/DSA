#include "file_io.h"
#include "monhoc.h"
#include "sinhvien.h"
#include <fstream>
#include <iostream>
#include <string.h>

using namespace std;

// --- FILE I/O CHO MÔN HỌC ---

// Đổ cây vào file theo Duyệt LNR
void GhiCayRaFile(TreeMonHoc root, ofstream &outFile) {
    if (root != nullptr) {
        GhiCayRaFile(root->left, outFile);
        outFile << root->mh.MAMH << ","
                << root->mh.TENMH << ","
                << root->mh.STCLT << ","
                << root->mh.STCTH << endl;
        GhiCayRaFile(root->right, outFile);
    }
}

bool GhiFileMonHoc(TreeMonHoc root, const char* filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) return false;
    GhiCayRaFile(root, outFile);
    outFile.close();
    return true;
}

bool DocFileMonHoc(DS_MonHoc &ds, const char* filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) return false;
    
    KhoiTaoCayMonHoc(ds.root);
    ds.n = 0;
    
    MonHoc mh;
    char buffer[256];
    while (inFile.getline(buffer, 256)) {
        if (strlen(buffer) == 0) continue;
        
        char* token = strtok(buffer, ",");
        if (token) strcpy(mh.MAMH, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(mh.TENMH, token);
        
        token = strtok(NULL, ",");
        if (token) mh.STCLT = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) mh.STCTH = atoi(token);
        
        ThemMonHoc(ds.root, mh, ds.n);
    }
    
    inFile.close();
    return true;
}

// --- FILE I/O CHO LỚP VÀ SINH VIÊN ---

bool GhiFileLopVaSinhVien(DS_LOPSV ds, const char* filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) return false;
    
    outFile << ds.n << endl; // Lưu số lượng lớp trước
    for (int i = 0; i < ds.n; i++) {
        LOPSV* lop = ds.nodes[i];
        outFile << lop->MALOP << "," << lop->TENLOP << endl;
        
        // Lưu sinh viên của lớp đó
        for (PTRSV p = lop->FirstSV; p != nullptr; p = p->next) {
            outFile << p->sv.MASV << ","
                    << p->sv.HO << ","
                    << p->sv.TEN << ","
                    << p->sv.PHAI << ","
                    << p->sv.SODT << endl;
        }
        outFile << "END_LOP" << endl; // Dấu hiệu kết thúc 1 lớp
    }
    
    outFile.close();
    return true;
}

bool DocFileLopVaSinhVien(DS_LOPSV &ds, const char* filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) return false;
    
    char buffer[256];
    inFile.getline(buffer, 256);
    int soLop = atoi(buffer);
    ds.n = 0;
    
    for (int i = 0; i < soLop; i++) {
        ds.nodes[i] = new LOPSV;
        inFile.getline(buffer, 256);
        
        char* token = strtok(buffer, ",");
        if (token) strcpy(ds.nodes[i]->MALOP, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(ds.nodes[i]->TENLOP, token);
        
        ds.nodes[i]->FirstSV = nullptr;
        
        // Đọc danh sách sinh viên
        while (inFile.getline(buffer, 256)) {
            if (strcmp(buffer, "END_LOP") == 0) break;
            
            SinhVien sv;
            token = strtok(buffer, ",");
            if (token) strcpy(sv.MASV, token);
            
            token = strtok(NULL, ",");
            if (token) strcpy(sv.HO, token);
            
            token = strtok(NULL, ",");
            if (token) strcpy(sv.TEN, token);
            
            token = strtok(NULL, ",");
            if (token) strcpy(sv.PHAI, token);
            
            token = strtok(NULL, ",");
            if (token) strcpy(sv.SODT, token);
            
            InsertOrderSV(ds.nodes[i]->FirstSV, sv);
        }
        ds.n++;
    }
    
    inFile.close();
    return true;
}

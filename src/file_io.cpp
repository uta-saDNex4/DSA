#include "file_io.h"
#include "monhoc.h"
#include "lop.h"
#include "sinhvien.h"
#include "loptc.h"
#include "dangky.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

// Đường dẫn mặc định phân tách 5 file độc lập theo Rule 7
const char* DEFAULT_PATH_MONHOC   = "data/monhoc.txt";
const char* DEFAULT_PATH_LOP      = "data/lop.txt";
const char* DEFAULT_PATH_SINHVIEN = "data/sinhvien.txt";
const char* DEFAULT_PATH_LOPTC    = "data/loptc.txt";
const char* DEFAULT_PATH_DANGKY   = "data/dangky.txt";

static void DamBaoThuMucTonTai(const char* filepath) {
    fs::path p(filepath);
    if (p.has_parent_path()) {
        fs::create_directories(p.parent_path());
    }
}

static bool HoanTatGhiAnToan(const char* filepath, const string& tempPath) {
    error_code ec;
    fs::path target(filepath);
    if (fs::exists(target, ec)) {
        fs::remove(target, ec);
    }
    fs::rename(tempPath, target, ec);
    if (ec) {
        fs::remove(tempPath, ec);
        return false;
    }
    return true;
}

// ====================================================================
// 1. MÔN HỌC (CÂY BST)
// ====================================================================

static void GhiCayRaStream(TreeMonHoc root, ofstream &outFile) {
    if (root != nullptr) {
        GhiCayRaStream(root->left, outFile);
        outFile << root->mh.MAMH << "|"
                << root->mh.TENMH << "|"
                << root->mh.STCLT << "|"
                << root->mh.STCTH << "\n";
        GhiCayRaStream(root->right, outFile);
    }
}

bool GhiFileMonHoc(TreeMonHoc root, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_MONHOC;
    DamBaoThuMucTonTai(path);

    string tempPath = string(path) + ".tmp";
    ofstream outFile(tempPath);
    if (!outFile.is_open()) {
        cout << "[!] Loi: Khong the mo file tam de ghi mon hoc: " << tempPath << "\n";
        return false;
    }

    GhiCayRaStream(root, outFile);
    outFile.close();

    if (!HoanTatGhiAnToan(path, tempPath)) {
        cout << "[!] Loi: Khong the hoan tat ghi an toan file mon hoc: " << path << "\n";
        return false;
    }

    return true;
}

bool DocFileMonHoc(DS_MonHoc &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_MONHOC;
    ifstream inFile(path);
    if (!inFile.is_open()) {
        return false;
    }

    GiaiPhongCay(ds.root);
    ds.n = 0;

    MonHoc mh;
    char buffer[256];
    int dong = 0;

    while (inFile.getline(buffer, 256)) {
        if (KiemTraRong(buffer)) continue;
        dong++;

        char* token = strtok(buffer, "|");
        if (!token) continue;
        strcpy(mh.MAMH, token);
        ChuanHoaMa(mh.MAMH);

        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(mh.TENMH, token);
        ChuanHoaTen(mh.TENMH);

        token = strtok(NULL, "|");
        if (!token) continue;
        mh.STCLT = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        mh.STCTH = atoi(token);

        ThemMonHoc(ds.root, mh, ds.n);
    }

    inFile.close();
    return true;
}

// ====================================================================
// 2. LỚP SINH VIÊN (MẢNG CON TRỎ)
// ====================================================================

bool GhiFileLop(const DS_LOPSV &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_LOP;
    DamBaoThuMucTonTai(path);

    string tempPath = string(path) + ".tmp";
    ofstream outFile(tempPath);
    if (!outFile.is_open()) {
        cout << "[!] Loi: Khong the mo file tam de ghi lop: " << tempPath << "\n";
        return false;
    }

    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr) {
            outFile << ds.nodes[i]->MALOP << "|" << ds.nodes[i]->TENLOP << "\n";
        }
    }
    outFile.close();

    return HoanTatGhiAnToan(path, tempPath);
}

bool DocFileLop(DS_LOPSV &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_LOP;
    ifstream inFile(path);
    if (!inFile.is_open()) {
        return false;
    }

    // Giải phóng danh sách cũ
    GiaiPhongDSLop(ds);

    char buffer[256];
    while (inFile.getline(buffer, 256)) {
        if (KiemTraRong(buffer)) continue;

        char* token = strtok(buffer, "|");
        if (!token) continue;
        char maLop[16];
        strcpy(maLop, token);
        ChuanHoaMa(maLop);

        token = strtok(NULL, "|");
        if (!token) continue;
        char tenLop[51];
        strcpy(tenLop, token);
        ChuanHoaTen(tenLop);

        if (ds.n < MAX_LOPSV) {
            LOPSV* lop = new LOPSV;
            strcpy(lop->MALOP, maLop);
            strcpy(lop->TENLOP, tenLop);
            lop->FirstSV = nullptr;
            ds.nodes[ds.n++] = lop;
        }
    }

    inFile.close();
    return true;
}

// ====================================================================
// 3. SINH VIÊN (DSLK ĐƠN - KÈM MALOP)
// ====================================================================

bool GhiFileSinhVien(const DS_LOPSV &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_SINHVIEN;
    DamBaoThuMucTonTai(path);

    string tempPath = string(path) + ".tmp";
    ofstream outFile(tempPath);
    if (!outFile.is_open()) {
        cout << "[!] Loi: Khong the mo file tam de ghi sinh vien: " << tempPath << "\n";
        return false;
    }

    for (int i = 0; i < ds.n; i++) {
        LOPSV* lop = ds.nodes[i];
        if (lop == nullptr) continue;

        for (PTRSV p = lop->FirstSV; p != nullptr; p = p->next) {
            outFile << lop->MALOP << "|"
                    << p->sv.MASV << "|"
                    << p->sv.HO << "|"
                    << p->sv.TEN << "|"
                    << p->sv.PHAI << "|"
                    << p->sv.SODT << "\n";
        }
    }
    outFile.close();

    return HoanTatGhiAnToan(path, tempPath);
}

bool DocFileSinhVien(DS_LOPSV &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_SINHVIEN;
    ifstream inFile(path);
    if (!inFile.is_open()) {
        return false;
    }

    // Đảm bảo xóa sạch các DSSV cũ trong các lớp trước khi nạp
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr) {
            GiaiPhongDSSV(ds.nodes[i]->FirstSV);
        }
    }

    char buffer[256];
    int dong = 0;
    while (inFile.getline(buffer, 256)) {
        if (KiemTraRong(buffer)) continue;
        dong++;

        char* token = strtok(buffer, "|");
        if (!token) continue;
        char maLop[16];
        strcpy(maLop, token);
        ChuanHoaMa(maLop);

        int idxLop = TimLop(ds, maLop);
        if (idxLop == -1) {
            cout << "[!] Canh bao: Dong " << dong << " chua Ma lop '" << maLop << "' khong ton tai, bo qua.\n";
            continue;
        }

        SinhVien sv;
        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(sv.MASV, token);
        ChuanHoaMa(sv.MASV);

        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(sv.HO, token);
        ChuanHoaTen(sv.HO);

        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(sv.TEN, token);
        ChuanHoaTen(sv.TEN);

        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(sv.PHAI, token);
        ChuanHoaMa(sv.PHAI);

        token = strtok(NULL, "|");
        if (!token) continue;
        strcpy(sv.SODT, token);
        ChuanHoaMa(sv.SODT);

        // Chèn vào đúng lớp, luôn tự động bảo đảm thứ tự Tên + Họ
        InsertOrderSV(ds.nodes[idxLop]->FirstSV, sv);
    }

    inFile.close();
    return true;
}

// ====================================================================
// 4. LỚP TÍN CHỈ (MẢNG CON TRỎ)
// ====================================================================

bool GhiFileLopTC(const DS_LopTC &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_LOPTC;
    DamBaoThuMucTonTai(path);

    string tempPath = string(path) + ".tmp";
    ofstream outFile(tempPath);
    if (!outFile.is_open()) {
        cout << "[!] Loi: Khong the mo file tam de ghi lop tin chi: " << tempPath << "\n";
        return false;
    }

    for (int i = 0; i < ds.n; i++) {
        LopTinChi* ltc = ds.nodes[i];
        if (ltc != nullptr) {
            outFile << ltc->MALOPTC << "|"
                    << ltc->MAMH << "|"
                    << ltc->NienKhoa << "|"
                    << ltc->HocKy << "|"
                    << ltc->Nhom << "|"
                    << ltc->SoSVMin << "|"
                    << ltc->SoSVMax << "|"
                    << (ltc->HuyLop ? 1 : 0) << "\n";
        }
    }
    outFile.close();

    return HoanTatGhiAnToan(path, tempPath);
}

bool DocFileLopTC(DS_LopTC &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_LOPTC;
    ifstream inFile(path);
    if (!inFile.is_open()) {
        return false;
    }

    GiaiPhongDS_LopTC(ds);

    char buffer[256];
    while (inFile.getline(buffer, 256)) {
        if (KiemTraRong(buffer)) continue;

        char* token = strtok(buffer, "|");
        if (!token) continue;
        int maLopTC = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        char maMH[11];
        strcpy(maMH, token);
        ChuanHoaMa(maMH);

        token = strtok(NULL, "|");
        if (!token) continue;
        char nienKhoa[10];
        strcpy(nienKhoa, token);
        ChuanHoaMa(nienKhoa);

        token = strtok(NULL, "|");
        if (!token) continue;
        int hocKy = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        int nhom = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        int svMin = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        int svMax = atoi(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        bool huyLop = (atoi(token) == 1);

        if (ds.n < MAX_LOPTC) {
            LopTinChi* ltc = new LopTinChi;
            ltc->MALOPTC = maLopTC;
            strcpy(ltc->MAMH, maMH);
            strcpy(ltc->NienKhoa, nienKhoa);
            ltc->HocKy = hocKy;
            ltc->Nhom = nhom;
            ltc->SoSVMin = svMin;
            ltc->SoSVMax = svMax;
            ltc->HuyLop = huyLop;
            ltc->FirstDK = nullptr;
            ds.nodes[ds.n++] = ltc;
        }
    }

    inFile.close();
    return true;
}

// ====================================================================
// 5. ĐĂNG KÝ (DSLK ĐƠN KÈM MALOPTC)
// ====================================================================

bool GhiFileDangKy(const DS_LopTC &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_DANGKY;
    DamBaoThuMucTonTai(path);

    string tempPath = string(path) + ".tmp";
    ofstream outFile(tempPath);
    if (!outFile.is_open()) {
        cout << "[!] Loi: Khong the mo file tam de ghi dang ky: " << tempPath << "\n";
        return false;
    }

    for (int i = 0; i < ds.n; i++) {
        LopTinChi* ltc = ds.nodes[i];
        if (ltc == nullptr) continue;

        for (PTR_DangKy p = ltc->FirstDK; p != nullptr; p = p->next) {
            outFile << ltc->MALOPTC << "|"
                    << p->dk.MASV << "|"
                    << p->dk.DIEM << "|"
                    << (p->dk.HuyDangKy ? 1 : 0) << "\n";
        }
    }
    outFile.close();

    return HoanTatGhiAnToan(path, tempPath);
}

bool DocFileDangKy(DS_LopTC &ds, const char* filename) {
    const char* path = filename ? filename : DEFAULT_PATH_DANGKY;
    ifstream inFile(path);
    if (!inFile.is_open()) {
        return false;
    }

    // Dọn sạch DSLK đăng ký cũ trong tất cả các lớp tín chỉ
    for (int i = 0; i < ds.n; i++) {
        if (ds.nodes[i] != nullptr) {
            GiaiPhongDS_DangKy(ds.nodes[i]->FirstDK);
        }
    }

    char buffer[256];
    int dong = 0;
    while (inFile.getline(buffer, 256)) {
        if (KiemTraRong(buffer)) continue;
        dong++;

        char* token = strtok(buffer, "|");
        if (!token) continue;
        int maLopTC = atoi(token);

        int idxLTC = TimLopTCTheoMa(ds, maLopTC);
        if (idxLTC == -1) {
            cout << "[!] Canh bao: Dong " << dong << " chua Ma Lop TC " << maLopTC << " khong ton tai, bo qua.\n";
            continue;
        }

        token = strtok(NULL, "|");
        if (!token) continue;
        char maSV[16];
        strcpy(maSV, token);
        ChuanHoaMa(maSV);

        token = strtok(NULL, "|");
        if (!token) continue;
        float diem = (float)atof(token);

        token = strtok(NULL, "|");
        if (!token) continue;
        bool huyDK = (atoi(token) == 1);

        // Chèn vào đầu danh sách đăng ký của lớp tín chỉ
        PTR_DangKy node = new nodeDangKy;
        strcpy(node->dk.MASV, maSV);
        node->dk.DIEM = diem;
        node->dk.HuyDangKy = huyDK;
        node->next = ds.nodes[idxLTC]->FirstDK;
        ds.nodes[idxLTC]->FirstDK = node;
    }

    inFile.close();
    return true;
}

// ====================================================================
// 6. CÁC HÀM TỔNG HỢP TOÀN HỆ THỐNG
// ====================================================================

bool LuuToanBoDuLieu(TreeMonHoc rootMH, const DS_LOPSV &dsLop, const DS_LopTC &dsLTC) {
    cout << "\n[...] Dang luu toan bo du lieu he thong...\n";
    bool ok1 = GhiFileMonHoc(rootMH);
    bool ok2 = GhiFileLop(dsLop);
    bool ok3 = GhiFileSinhVien(dsLop);
    bool ok4 = GhiFileLopTC(dsLTC);
    bool ok5 = GhiFileDangKy(dsLTC);

    if (ok1 && ok2 && ok3 && ok4 && ok5) {
        cout << "[OK] Luu toan bo du lieu thanh cong vao thu muc 'data/'.\n";
        return true;
    }
    cout << "[!] Co loi xay ra khi luu mot so file du lieu!\n";
    return false;
}

bool DocToanBoDuLieu(DS_MonHoc &dsMH, DS_LOPSV &dsLop, DS_LopTC &dsLTC) {
    cout << "\n[...] Dang doc du lieu he thong tu thu muc 'data/'...\n";
    bool ok1 = DocFileMonHoc(dsMH);
    bool ok2 = DocFileLop(dsLop);
    bool ok3 = DocFileSinhVien(dsLop);
    bool ok4 = DocFileLopTC(dsLTC);
    bool ok5 = DocFileDangKy(dsLTC);

    if (ok1 && ok2 && ok3 && ok4 && ok5) {
        cout << "[OK] Nap du lieu hoan tat thanh cong!\n";
        return true;
    }
    cout << "[i] Mot so file du lieu chua ton tai hoac khoi tao lan dau.\n";
    return false;
}

// ====================================================================
// 7. HÀM TƯƠNG THÍCH NGƯỢC
// ====================================================================

bool GhiFileLopVaSinhVien(const DS_LOPSV &ds, const char* filename) {
    ofstream outFile(filename);
    if (!outFile.is_open()) return false;

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
    return true;
}

bool DocFileLopVaSinhVien(DS_LOPSV &ds, const char* filename) {
    ifstream inFile(filename);
    if (!inFile.is_open()) return false;

    GiaiPhongDSLop(ds);

    char buffer[256];
    if (!inFile.getline(buffer, 256) || KiemTraRong(buffer)) {
        inFile.close();
        return true;
    }
    int soLop = atoi(buffer);

    for (int i = 0; i < soLop; i++) {
        if (!inFile.getline(buffer, 256)) break;

        ds.nodes[i] = new LOPSV;
        char* token = strtok(buffer, "|");
        if (token) strcpy(ds.nodes[i]->MALOP, token);
        token = strtok(NULL, "|");
        if (token) strcpy(ds.nodes[i]->TENLOP, token);
        ds.nodes[i]->FirstSV = nullptr;

        while (inFile.getline(buffer, 256)) {
            if (strcmp(buffer, "END_LOP") == 0) break;
            if (KiemTraRong(buffer)) continue;

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
    return true;
}

#ifndef STRUCT_H
#define STRUCT_H

const int MAX_LOPSV = 10000;
const int MAX_LOPTC = 10000;

// === 1. DANH SÁCH SINH VIÊN & LỚP SV ===
struct SinhVien {
    char MASV[16];
    char HO[51];
    char TEN[16];
    char PHAI[4];
    char SODT[16];
};

struct nodeSV {
    SinhVien sv;
    nodeSV *next = nullptr;
};
typedef nodeSV *PTRSV;

struct LOPSV {
    char MALOP[16];
    char TENLOP[51];
    PTRSV FirstSV = nullptr;
};

struct DS_LOPSV {
    int n = 0;
    LOPSV *nodes[MAX_LOPSV];
};

// === 2. DANH SÁCH MÔN HỌC (CÂY NHỊ PHÂN TÌM KIẾM - BST) ===
struct MonHoc {
    char MAMH[11];
    char TENMH[51];
    int STCLT;
    int STCTH;
};

struct nodeMonHoc {
    MonHoc mh;
    nodeMonHoc *left = nullptr;
    nodeMonHoc *right = nullptr;
};
typedef nodeMonHoc *TreeMonHoc;

// === 3. DANH SÁCH ĐĂNG KÝ (DSLK ĐƠN) ===
struct DangKy {
    char MASV[16];
    float DIEM;
    bool HuyDangKy;
};

struct nodeDangKy {
    DangKy dk;
    nodeDangKy *next = nullptr;
};
typedef nodeDangKy *PTR_DangKy;

// === 4. DANH SÁCH LỚP TÍN CHỈ (MẢNG CON TRỎ) ===
struct LopTinChi {
    int MALOPTC;         // Mã tự tăng
    char MAMH[11];
    char NienKhoa[10];
    int HocKy;
    int Nhom;
    int SoSVMin;
    int SoSVMax;
    bool HuyLop;
    PTR_DangKy FirstDK = nullptr; // Danh sách SV đăng ký
};

struct DS_LopTC {
    int n = 0;
    LopTinChi *nodes[MAX_LOPTC];
};

#endif

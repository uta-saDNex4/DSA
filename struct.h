const int MAX_LOPSV = 10000;

struct SinhVien{    // tạo info
    char MASV[16];
    char HO[51];
    char TEN[16];
    char PHAI[4];
    char SODT[16];
};

struct nodeSV{
    SinhVien sv;   //kdl info
    nodeSV *next;
};

typedef nodeSV *PTRSV;
PTRSV First = nullptr;

struct LOPSV{
    char MALOP[16];
    char TENLOP[51];
    PTRSV FirstSV = nullptr;
};

struct DS_LOPSV{
    int n = 0;
    LOPSV *nodes[MAX_LOPSV];
};


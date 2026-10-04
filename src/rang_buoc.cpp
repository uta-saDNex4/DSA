#include "rang_buoc.h"
#include <iostream>
#include <string.h>

using namespace std;

int DemDangKyByMASV(const DS_LopTC &dsltc, const char MASV[]) {
    int count = 0;
    for (int i = 0; i < dsltc.n; i++) {
        if (dsltc.nodes[i] == nullptr || dsltc.nodes[i]->HuyLop) continue;
        PTR_DangKy p = dsltc.nodes[i]->FirstDK;
        while (p != nullptr) {
            if (stricmp(p->dk.MASV, MASV) == 0 && !p->dk.HuyDangKy) {
                count++;
                break;
            }
            p = p->next;
        }
    }
    return count;
}

bool SVDaCoDiem(const DS_LopTC &dsltc, const char MASV[]) {
    for (int i = 0; i < dsltc.n; i++) {
        if (dsltc.nodes[i] == nullptr) continue;
        PTR_DangKy p = dsltc.nodes[i]->FirstDK;
        while (p != nullptr) {
            if (stricmp(p->dk.MASV, MASV) == 0 && p->dk.DIEM >= 0) {
                return true;
            }
            p = p->next;
        }
    }
    return false;
}

int DemLopTCByMAMH(const DS_LopTC &dsltc, const char MAMH[]) {
    int count = 0;
    for (int i = 0; i < dsltc.n; i++) {
        if (dsltc.nodes[i] == nullptr) continue;
        if (stricmp(dsltc.nodes[i]->MAMH, MAMH) == 0) {
            count++;
        }
    }
    return count;
}

int DemDangKyByLop(const DS_LopTC &dsltc, PTRSV FirstSV) {
    int count = 0;
    for (PTRSV p = FirstSV; p != nullptr; p = p->next) {
        count += DemDangKyByMASV(dsltc, p->sv.MASV);
    }
    return count;
}

void InRangBuocSV(const DS_LopTC &dsltc, const char MASV[]) {
    cout << "  Cac rang buoc cua sinh vien '" << MASV << "':\n";
    bool coRangBuoc = false;
    for (int i = 0; i < dsltc.n; i++) {
        if (dsltc.nodes[i] == nullptr) continue;
        PTR_DangKy p = dsltc.nodes[i]->FirstDK;
        while (p != nullptr) {
            if (stricmp(p->dk.MASV, MASV) == 0) {
                if (!p->dk.HuyDangKy || p->dk.DIEM >= 0) {
                    cout << "  - Lop TC ma " << dsltc.nodes[i]->MALOPTC
                         << " (Mon: " << dsltc.nodes[i]->MAMH << ")";
                    if (!p->dk.HuyDangKy) cout << " [DANG DANG KY]";
                    if (p->dk.DIEM >= 0) cout << " [DA CO DIEM: " << p->dk.DIEM << "]";
                    if (dsltc.nodes[i]->HuyLop) cout << " [LOP BI HUY]";
                    cout << "\n";
                    coRangBuoc = true;
                }
            }
            p = p->next;
        }
    }
    if (!coRangBuoc) {
        cout << "  (Khong co rang buoc nao)\n";
    }
}

void InRangBuocMonHoc(const DS_LopTC &dsltc, const char MAMH[]) {
    cout << "  Cac rang buoc cua mon hoc '" << MAMH << "':\n";
    bool coRangBuoc = false;
    for (int i = 0; i < dsltc.n; i++) {
        if (dsltc.nodes[i] == nullptr) continue;
        if (stricmp(dsltc.nodes[i]->MAMH, MAMH) == 0) {
            int soSVDK = 0;
            PTR_DangKy p = dsltc.nodes[i]->FirstDK;
            while (p != nullptr) {
                if (!p->dk.HuyDangKy) soSVDK++;
                p = p->next;
            }
            cout << "  - Lop TC ma " << dsltc.nodes[i]->MALOPTC
                 << " (NK: " << dsltc.nodes[i]->NienKhoa
                 << ", HK: " << dsltc.nodes[i]->HocKy
                 << ", Nhom: " << dsltc.nodes[i]->Nhom
                 << ", SV: " << soSVDK << ")";
            if (dsltc.nodes[i]->HuyLop) cout << " [DA HUY]";
            cout << "\n";
            coRangBuoc = true;
        }
    }
    if (!coRangBuoc) {
        cout << "  (Khong co rang buoc nao)\n";
    }
}

void InRangBuocLop(const DS_LopTC &dsltc, PTRSV FirstSV) {
    cout << "  Cac sinh vien trong lop dang co dang ky:\n";
    bool coRangBuoc = false;
    for (PTRSV sv = FirstSV; sv != nullptr; sv = sv->next) {
        int dk = DemDangKyByMASV(dsltc, sv->sv.MASV);
        if (dk > 0) {
            cout << "  - SV '" << sv->sv.MASV << "' (" << sv->sv.HO << " " << sv->sv.TEN
                 << ") dang dang ky " << dk << " lop tin chi\n";
            coRangBuoc = true;
        }
    }
    if (!coRangBuoc) {
        cout << "  (Khong co rang buoc nao)\n";
    }
}

#ifndef LOP_H
#define LOP_H

#include "struct.h"

void KhoiTaoDanhSachLop(DS_LOPSV &ds);

int TimLop(DS_LOPSV ds, char MALOP[]);

bool ThemLop(DS_LOPSV &ds, LOPSV lop);

bool XoaLop(DS_LOPSV &ds, char MALOP[], DS_LopTC dsltc);

bool SuaLop(DS_LOPSV &ds, char MALOP[], char TENLOP_MOI[]);

void InDanhSachLop(DS_LOPSV ds);

// Giải phóng toàn bộ danh sách lớp (kèm DSSV bên trong)
void GiaiPhongDSLop(DS_LOPSV &ds);

#endif

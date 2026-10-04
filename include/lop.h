#ifndef LOP_H
#define LOP_H

#include "struct.h"

void KhoiTaoDanhSachLop(DS_LOPSV &ds);

// Tìm lớp theo MALOP (trả về index 0..n-1, -1 nếu không thấy)
int TimLop(const DS_LOPSV &ds, const char MALOP[]);

bool ThemLop(DS_LOPSV &ds, LOPSV lop);

bool XoaLop(DS_LOPSV &ds, const char MALOP[], const DS_LopTC &dsltc);

bool SuaLop(DS_LOPSV &ds, const char MALOP[], const char TENLOP_MOI[]);

void InDanhSachLop(const DS_LOPSV &ds);

// Giải phóng toàn bộ danh sách lớp (kèm DSSV bên trong)
void GiaiPhongDSLop(DS_LOPSV &ds);

#endif

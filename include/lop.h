#ifndef LOP_H
#define LOP_H

#include "struct.h"

// Khởi tạo mảng con trỏ Lớp SV
void KhoiTaoDanhSachLop(DS_LOPSV &ds);

// Tìm lớp theo MALOP, trả về vị trí (index), không thấy trả về -1
int TimLop(DS_LOPSV ds, char MALOP[]);

// Thêm lớp mới
bool ThemLop(DS_LOPSV &ds, LOPSV lop);

// Cập nhật thông tin lớp
bool SuaLop(DS_LOPSV &ds, char MALOP[], char TENLOP_MOI[]);

// In danh sách các lớp
void InDanhSachLop(DS_LOPSV ds);

#endif

#ifndef FILE_IO_H
#define FILE_IO_H

#include "struct.h"

// Đọc/Ghi dữ liệu Môn Học (Cây Nhị Phân)
bool SaveMonHocToFile(TreeMonHoc root, const char* filename);
bool LoadMonHocFromFile(DS_MonHoc &ds, const char* filename);

// Đọc/Ghi dữ liệu Lớp và Sinh Viên
bool SaveLopVaSinhVienToFile(DS_LOPSV ds, const char* filename);
bool LoadLopVaSinhVienFromFile(DS_LOPSV &ds, const char* filename);

#endif

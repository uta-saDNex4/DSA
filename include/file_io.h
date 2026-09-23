#ifndef FILE_IO_H
#define FILE_IO_H

#include "struct.h"

// Đọc/Ghi dữ liệu Môn Học (Cây Nhị Phân)
bool GhiFileMonHoc(TreeMonHoc root, const char* filename);
bool DocFileMonHoc(DS_MonHoc &ds, const char* filename);

// Đọc/Ghi dữ liệu Lớp và Sinh Viên
bool GhiFileLopVaSinhVien(DS_LOPSV ds, const char* filename);
bool DocFileLopVaSinhVien(DS_LOPSV &ds, const char* filename);

#endif

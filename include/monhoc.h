#ifndef MONHOC_H
#define MONHOC_H

#include "struct.h"

// Khởi tạo cây môn học
void KhoiTaoCayMonHoc(TreeMonHoc &root);

// Thêm môn học vào cây BST (sắp xếp theo MAMH)
void ThemMonHoc(TreeMonHoc &root, MonHoc mh, int &n);

// Nhập liên tục danh sách môn học
void NhapMonHoc(DS_MonHoc &ds);

// Xóa 1 môn học khỏi BST theo MAMH
bool XoaMonHoc(TreeMonHoc &root, char MAMH[], int &n);

// Cập nhật môn học
void SuaMonHoc(TreeMonHoc root, char MAMH[]);

// Tìm môn học theo MAMH
TreeMonHoc TimMonHoc(TreeMonHoc root, char MAMH[]);

// In danh sách môn học tăng dần theo Tên môn học (Câu F)
void InDanhSachMonHoc(TreeMonHoc root, int n);

#endif

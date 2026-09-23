#ifndef MONHOC_H
#define MONHOC_H

#include "struct.h"

void KhoiTaoCayMonHoc(TreeMonHoc &root);

void ThemMonHoc(TreeMonHoc &root, MonHoc mh, int &n);

void NhapMonHoc(DS_MonHoc &ds);

bool XoaMonHoc(TreeMonHoc &root, char MAMH[], int &n);

void SuaMonHoc(TreeMonHoc root, char MAMH[]);

TreeMonHoc TimMonHoc(TreeMonHoc root, char MAMH[]);

// In danh sách môn học tăng dần theo Tên môn học (Câu F)
void InDanhSachMonHoc(TreeMonHoc root, int n);

// Giải phóng toàn bộ cây BST
void GiaiPhongCay(TreeMonHoc &root);

#endif

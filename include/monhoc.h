#ifndef MONHOC_H
#define MONHOC_H

#include "struct.h"

void KhoiTaoCayMonHoc(TreeMonHoc &root);

void ThemMonHoc(TreeMonHoc &root, MonHoc mh, int &n);

void NhapMonHoc(DS_MonHoc &ds);

// Xóa môn học CÓ kiểm tra ràng buộc (cần DS_LopTC)
bool XoaMonHoc(TreeMonHoc &root, const char MAMH[], int &n, const DS_LopTC &dsltc);

// Xóa môn học KHÔNG kiểm tra ràng buộc (dùng nội bộ / giải phóng)
bool XoaMonHocNoiB(TreeMonHoc &root, const char MAMH[], int &n);

void SuaMonHoc(TreeMonHoc root, const char MAMH[]);

TreeMonHoc TimMonHoc(TreeMonHoc root, const char MAMH[]);

void InDanhSachMonHoc(TreeMonHoc root, int n);

void GiaiPhongCay(TreeMonHoc &root);

#endif

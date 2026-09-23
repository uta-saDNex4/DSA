#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "struct.h"

void KhoiTaoDanhSachSV(PTRSV &First);

PTRSV TimSV(PTRSV First, char MASV[]);

// Hàm phụ trợ so sánh tên
int CompareName(SinhVien a, SinhVien b);

// Chèn sinh viên có thứ tự (theo Tên -> Họ -> Mã SV)
void InsertSortedSV(PTRSV &First, SinhVien sv);

// Nhập danh sách sinh viên mới (dừng khi mã SV rỗng)
void NhapDanhSachSV(PTRSV &First);

// Xóa sinh viên theo mã
void XoaSV(PTRSV &First, char MASV[]);

// Hiệu chỉnh sinh viên
void SuaSV(PTRSV &First, char MASV[]);

// In danh sách gốc (theo thứ tự tên)
void XuatDanhSachSinhVien(PTRSV First);

// In danh sách theo thứ tự alphabet của Mã SV (Câu D)
void InDSSVTheoMa(PTRSV First);

#endif
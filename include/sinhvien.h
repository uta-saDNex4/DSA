#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "struct.h"

void KhoiTaoDanhSachSV(PTRSV &First);

PTRSV TimSV(PTRSV First, char MASV[]);

int SoSanhTen(SinhVien a, SinhVien b);

void InsertOrderSV(PTRSV &First, SinhVien sv);

// Nhập danh sách sinh viên mới (dừng khi mã SV rỗng)
void NhapDanhSachSV(PTRSV &First);

// Xóa sinh viên theo mã (có thông báo)
void XoaSV(PTRSV &First, char MASV[]);

// Xóa sinh viên theo mã (không thông báo, dùng nội bộ cho SuaSV)
bool XoaSVNoiB(PTRSV &First, char MASV[]);

// Hiệu chỉnh sinh viên
void SuaSV(PTRSV &First, char MASV[]);

// In danh sách gốc (theo thứ tự tên)
void XuatDanhSachSinhVien(PTRSV First);

// In danh sách theo thứ tự alphabet của Mã SV (Câu D)
void InDSSVTheoMa(PTRSV First);

// Đếm số lượng sinh viên trong danh sách
int DemSV(PTRSV First);

// Giải phóng toàn bộ danh sách sinh viên
void GiaiPhongDSSV(PTRSV &First);

#endif
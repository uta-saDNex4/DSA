#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "struct.h"

void KhoiTaoDanhSachSV(PTRSV &First);

PTRSV TimSV(PTRSV First, char MASV[]);

int SoSanhTen(SinhVien a, SinhVien b);

void InsertOrderSV(PTRSV &First, SinhVien sv);

void NhapDanhSachSV(PTRSV &First);

// Xóa sinh viên CÓ kiểm tra ràng buộc (cần DS_LopTC)
void XoaSV(PTRSV &First, char MASV[], DS_LopTC dsltc);

// Xóa sinh viên KHÔNG thông báo (dùng nội bộ cho SuaSV)
bool XoaSVNoiB(PTRSV &First, char MASV[]);

void SuaSV(PTRSV &First, char MASV[]);

void XuatDanhSachSinhVien(PTRSV First);

// In danh sách theo thứ tự alphabet của Mã SV (Câu D)
void InDSSVTheoMa(PTRSV First);

int DemSV(PTRSV First);

void GiaiPhongDSSV(PTRSV &First);

#endif
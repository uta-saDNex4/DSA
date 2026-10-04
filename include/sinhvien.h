#ifndef SINHVIEN_H
#define SINHVIEN_H

#include "struct.h"

void KhoiTaoDanhSachSV(PTRSV &First);

PTRSV TimSV(PTRSV First, const char MASV[]);

int SoSanhTen(SinhVien a, SinhVien b);

void InsertOrderSV(PTRSV &First, SinhVien sv);

void NhapDanhSachSV(PTRSV &First);

// Xóa sinh viên CÓ kiểm tra ràng buộc (cần const DS_LopTC &dsltc)
void XoaSV(PTRSV &First, const char MASV[], const DS_LopTC &dsltc);

// Xóa sinh viên KHÔNG thông báo (dùng nội bộ cho SuaSV)
bool XoaSVNoiB(PTRSV &First, const char MASV[]);

void SuaSV(PTRSV &First, const char MASV[]);

void XuatDanhSachSinhVien(PTRSV First);

// In danh sách theo thứ tự alphabet của Mã SV (Câu D)
void InDSSVTheoMa(PTRSV First);

int DemSV(PTRSV First);

// Tìm kiếm sinh viên trên toàn bộ các lớp học (tra cứu thông tin SV theo Mã SV)
PTRSV TimSVToanTruong(const DS_LOPSV &dsLop, const char MASV[], LOPSV* &lopChuaSV);

void GiaiPhongDSSV(PTRSV &First);

#endif
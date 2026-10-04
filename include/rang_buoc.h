#ifndef RANG_BUOC_H
#define RANG_BUOC_H

#include "struct.h"

// ====================================================================
// MODULE KIỂM TRA RÀNG BUỘC THAM CHIẾU (Referential Integrity)
// Dùng chung cho tất cả các module: SinhVien, Lop, MonHoc
// Tối ưu: Sử dụng const DS_LopTC &dsltc (chuyển từ 80KB stack copy sang 8 bytes reference)
// ====================================================================

// Kiểm tra sinh viên có đăng ký lớp tín chỉ nào không (chưa hủy)
int DemDangKyByMASV(const DS_LopTC &dsltc, const char MASV[]);

// Kiểm tra sinh viên có điểm ở bất kỳ lớp tín chỉ nào không (DIEM >= 0)
bool SVDaCoDiem(const DS_LopTC &dsltc, const char MASV[]);
inline bool SVDaCoHiem(const DS_LopTC &dsltc, const char MASV[]) { return SVDaCoDiem(dsltc, MASV); }

// Kiểm tra môn học có đang được sử dụng bởi lớp tín chỉ nào không
int DemLopTCByMAMH(const DS_LopTC &dsltc, const char MAMH[]);

// Kiểm tra 1 lớp SV có SV nào đang đăng ký tín chỉ không
int DemDangKyByLop(const DS_LopTC &dsltc, PTRSV FirstSV);

// In danh sách các ràng buộc cụ thể (giúp user biết tại sao không xóa được)
void InRangBuocSV(const DS_LopTC &dsltc, const char MASV[]);
void InRangBuocMonHoc(const DS_LopTC &dsltc, const char MAMH[]);
void InRangBuocLop(const DS_LopTC &dsltc, PTRSV FirstSV);

#endif

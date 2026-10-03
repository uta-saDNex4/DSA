#ifndef RANG_BUOC_H
#define RANG_BUOC_H

#include "struct.h"

// ====================================================================
// MODULE KIỂM TRA RÀNG BUỘC THAM CHIẾU (Referential Integrity)
// Dùng chung cho tất cả các module: SinhVien, Lop, MonHoc
// ====================================================================

// Kiểm tra sinh viên có đăng ký lớp tín chỉ nào không (chưa hủy)
// Trả về số lượng lớp tín chỉ mà SV đang đăng ký (chưa hủy đăng ký)
int DemDangKyByMASV(DS_LopTC dsltc, char MASV[]);

// Kiểm tra sinh viên có điểm ở bất kỳ lớp tín chỉ nào không
// Trả về true nếu SV đã có ít nhất 1 điểm (DIEM >= 0)
bool SVDaCoHiem(DS_LopTC dsltc, char MASV[]);

// Kiểm tra môn học có đang được sử dụng bởi lớp tín chỉ nào không
// Trả về số lượng lớp tín chỉ đang dùng môn học này
int DemLopTCByMAMH(DS_LopTC dsltc, char MAMH[]);

// Kiểm tra 1 lớp SV có SV nào đang đăng ký tín chỉ không
// Duyệt toàn bộ DSSV của lớp → check từng MASV trong DS_LopTC
int DemDangKyByLop(DS_LopTC dsltc, PTRSV FirstSV);

// In danh sách các ràng buộc cụ thể (giúp user biết tại sao không xóa được)
void InRangBuocSV(DS_LopTC dsltc, char MASV[]);
void InRangBuocMonHoc(DS_LopTC dsltc, char MAMH[]);
void InRangBuocLop(DS_LopTC dsltc, PTRSV FirstSV);

#endif

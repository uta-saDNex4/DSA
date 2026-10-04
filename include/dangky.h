#ifndef DANGKY_H
#define DANGKY_H

#include "struct.h"

// Khởi tạo danh sách liên kết đơn đăng ký
void KhoiTaoDS_DangKy(PTR_DangKy &FirstDK);

// Đếm số lượng sinh viên đang đăng ký hợp lệ (chưa hủy đăng ký)
int DemSVDangKyHopLe(PTR_DangKy FirstDK);

// Đếm tổng số sinh viên từng đăng ký (kể cả đã hủy)
int DemTongSVDangKy(PTR_DangKy FirstDK);

// Tìm kiếm bản ghi đăng ký theo Mã SV trong 1 lớp tín chỉ
PTR_DangKy TimDangKy(PTR_DangKy FirstDK, const char maSV[]);

// Thêm sinh viên vào danh sách đăng ký của lớp tín chỉ
bool ThemDangKy(PTR_DangKy &FirstDK, const char maSV[]);

// Hủy đăng ký của sinh viên (đánh dấu HuyDangKy = true)
bool HuyDangKySV(PTR_DangKy FirstDK, const char maSV[]);

// Phục hồi đăng ký nếu trước đó đã hủy (HuyDangKy = false)
bool PhucHoiDangKySV(PTR_DangKy FirstDK, const char maSV[]);

// Kiểm tra sinh viên đã đăng ký môn học này ở bất kỳ nhóm nào trong kỳ chưa
bool DaDangKyMonHocTrongKy(const DS_LopTC &dsltc, const char maSV[], const char nienKhoa[], int hocKy, const char maMH[]);

// [CÂU G] Nghiệp vụ đăng ký lớp tín chỉ cho sinh viên
void XuLyDangKyLTC(DS_LopTC &dsltc, const DS_LOPSV &dslop, TreeMonHoc rootMH, const char maSV[], const char nienKhoa[], int hocKy);

// Giải phóng toàn bộ danh sách liên kết đơn đăng ký
void GiaiPhongDS_DangKy(PTR_DangKy &FirstDK);

#endif

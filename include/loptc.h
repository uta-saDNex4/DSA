#ifndef LOPTC_H
#define LOPTC_H

#include "struct.h"

// Khởi tạo danh sách tuyến tính lớp tín chỉ (mảng con trỏ)
void KhoiTaoDS_LopTC(DS_LopTC &ds);

// Cấp phát mã lớp tín chỉ tự động tăng (tìm mã lớn nhất hiện có + 1)
int LayMaLopTCTuDong(const DS_LopTC &ds);

// Tìm kiếm lớp tín chỉ theo MALOPTC (trả về chỉ số mảng 0..n-1, -1 nếu không thấy)
int TimLopTCTheoMa(const DS_LopTC &ds, int maLopTC);

// Kiểm tra trùng lặp lớp tín chỉ theo tổ hợp: Niên khóa, Học kỳ, Mã MH, Nhóm
int TimLopTCTrung(const DS_LopTC &ds, const char nienKhoa[], int hocKy, const char maMH[], int nhom);

// [CÂU A] Mở lớp tín chỉ: Thêm mới lớp tín chỉ
bool ThemLopTC(DS_LopTC &ds, LopTinChi ltc, TreeMonHoc rootMH);

// [CÂU A] Xóa lớp tín chỉ (Chỉ cho phép xóa khi chưa có SV đăng ký hoặc có điểm)
bool XoaLopTC(DS_LopTC &ds, int maLopTC);

// [CÂU A] Hiệu chỉnh thông tin lớp tín chỉ
bool HieuChinhLopTC(DS_LopTC &ds, int maLopTC, LopTinChi ltcMoi, TreeMonHoc rootMH);

// [CÂU B] In danh sách sinh viên đã đăng ký lớp tín chỉ theo tham số (có sắp xếp theo Mã SV)
void InDSSVDangKyLTC(const DS_LopTC &dsltc, const DS_LOPSV &dslop, const char nienKhoa[], int hocKy, const char maMH[], int nhom);

// In toàn bộ danh sách lớp tín chỉ kèm trạng thái
void InDanhSachLopTC(const DS_LopTC &ds, TreeMonHoc rootMH);

// [CÂU H] Tự động lọc và hủy các lớp tín chỉ có số SV đăng ký < SoSVMin (có xác nhận)
bool XacNhanVaHuyLopTC(DS_LopTC &dsltc, const char nienKhoa[], int hocKy);

// Giải phóng toàn bộ bộ nhớ mảng con trỏ lớp tín chỉ và các DSLK đăng ký con
void GiaiPhongDS_LopTC(DS_LopTC &ds);

#endif

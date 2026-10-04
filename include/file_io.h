#ifndef FILE_IO_H
#define FILE_IO_H

#include "struct.h"

// Đường dẫn mặc định lưu trữ 5 file dữ liệu độc lập (Chuẩn Rule 7)
extern const char* DEFAULT_PATH_MONHOC;
extern const char* DEFAULT_PATH_LOP;
extern const char* DEFAULT_PATH_SINHVIEN;
extern const char* DEFAULT_PATH_LOPTC;
extern const char* DEFAULT_PATH_DANGKY;

// 1. Môn học (Cây BST)
bool GhiFileMonHoc(TreeMonHoc root, const char* filename = nullptr);
bool DocFileMonHoc(DS_MonHoc &ds, const char* filename = nullptr);

// 2. Lớp sinh viên (Mảng con trỏ)
bool GhiFileLop(const DS_LOPSV &ds, const char* filename = nullptr);
bool DocFileLop(DS_LOPSV &ds, const char* filename = nullptr);

// 3. Sinh viên (DSLK đơn - lưu kèm MALOP để phân tách độc lập với lớp)
bool GhiFileSinhVien(const DS_LOPSV &ds, const char* filename = nullptr);
bool DocFileSinhVien(DS_LOPSV &ds, const char* filename = nullptr);

// 4. Lớp tín chỉ (Mảng con trỏ)
bool GhiFileLopTC(const DS_LopTC &ds, const char* filename = nullptr);
bool DocFileLopTC(DS_LopTC &ds, const char* filename = nullptr);

// 5. Đăng ký (DSLK đơn - lưu kèm MALOPTC để liên kết về lớp tín chỉ)
bool GhiFileDangKy(const DS_LopTC &ds, const char* filename = nullptr);
bool DocFileDangKy(DS_LopTC &ds, const char* filename = nullptr);

// 6. Các hàm tổng hợp toàn bộ hệ thống
bool LuuToanBoDuLieu(TreeMonHoc rootMH, const DS_LOPSV &dsLop, const DS_LopTC &dsLTC);
bool DocToanBoDuLieu(DS_MonHoc &dsMH, DS_LOPSV &dsLop, DS_LopTC &dsLTC);

// 7. Hàm hỗ trợ tương thích ngược (nếu cần)
bool GhiFileLopVaSinhVien(const DS_LOPSV &ds, const char* filename);
bool DocFileLopVaSinhVien(DS_LOPSV &ds, const char* filename);

#endif

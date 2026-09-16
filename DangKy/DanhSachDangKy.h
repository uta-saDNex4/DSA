#ifndef DANHSACHDANGKY_H
#define DANHSACHDANGKY_H

#include "DangKy.h"

// Một node trong danh sách liên kết đơn
struct NodeDangKy
{
    DangKy data;
    NodeDangKy* next;
};

// Danh sách liên kết đơn đăng ký
struct DanhSachDangKy
{
    NodeDangKy* head;
    int soLuong;
};

// Khởi tạo danh sách
void khoiTaoDanhSach(DanhSachDangKy& ds);

// Kiểm tra danh sách rỗng
bool danhSachRong(const DanhSachDangKy& ds);

// Tạo node mới
NodeDangKy* taoNode(const DangKy& dk);

// Tìm sinh viên theo mã sinh viên
NodeDangKy* timSinhVien(
    const DanhSachDangKy& ds,
    const string& maSV
);

// Kiểm tra sinh viên đã đăng ký chưa
bool daDangKy(
    const DanhSachDangKy& ds,
    const string& maSV
);

// Thêm sinh viên vào danh sách
bool themDangKy(
    DanhSachDangKy& ds,
    const DangKy& dk
);

// Xóa đăng ký theo mã sinh viên
bool xoaDangKy(
    DanhSachDangKy& ds,
    const string& maSV
);

// Hủy đăng ký, không xóa node
bool huyDangKy(
    DanhSachDangKy& ds,
    const string& maSV
);

// Khôi phục đăng ký
bool khoiPhucDangKy(
    DanhSachDangKy& ds,
    const string& maSV
);

// Nhập hoặc sửa điểm
bool capNhatDiem(
    DanhSachDangKy& ds,
    const string& maSV,
    float diemMoi
);

// Đếm số đăng ký chưa bị hủy
int demDangKyHienTai(
    const DanhSachDangKy& ds
);

// In danh sách đăng ký
void inDanhSachDangKy(
    const DanhSachDangKy& ds
);

// Giải phóng bộ nhớ
void giaiPhongDanhSach(
    DanhSachDangKy& ds
);

#endif
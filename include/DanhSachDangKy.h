#include "DangKy.h"

void khoiTaoDanhSach(DanhSachDangKy& ds);

bool danhSachRong(const DanhSachDangKy& ds);

bool themDangKy(
    DanhSachDangKy& ds,
    const string& maSV
);

NodeDangKy* timDangKy(
    DanhSachDangKy& ds,
    const string& maSV
);

bool xoaDangKy(
    DanhSachDangKy& ds,
    const string& maSV
);

bool huyDangKy(
    DanhSachDangKy& ds,
    const string& maSV
);

bool capNhatDiem(
    DanhSachDangKy& ds,
    const string& maSV,
    float diem
);

int demSoSinhVienDangKy(
    const DanhSachDangKy& ds
);

void inDanhSachDangKy(
    const DanhSachDangKy& ds
);

void giaiPhongDanhSach(
    DanhSachDangKy& ds
);

#ifndef DANGKY_H
#define DANGKY_H

#include <string>

using namespace std;

// Thông tin một sinh viên đăng ký lớp tín chỉ
struct DangKy
{
    string maSV;
    float diem;
    bool huyDangKy;
};

// Hàm tạo một đăng ký
DangKy taoDangKy(
    const string& maSV,
    float diem = -1.0f,
    bool huyDangKy = false
);

#endif
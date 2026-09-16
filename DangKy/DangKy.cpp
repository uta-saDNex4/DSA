#include "DangKy.h"

DangKy taoDangKy(
    const string& maSV,
    float diem,
    bool huyDangKy
)
{
    DangKy dk;

    dk.maSV = maSV;
    dk.diem = diem;
    dk.huyDangKy = huyDangKy;

    return dk;
}
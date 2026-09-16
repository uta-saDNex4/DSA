#include "DSDangKy.h"

#include <iostream>
#include <iomanip>

using namespace std;

// Khởi tạo danh sách
void khoiTaoDanhSach(DanhSachDangKy& ds)
{
    ds.head = nullptr;
    ds.soLuong = 0;
}

// Kiểm tra danh sách rỗng
bool danhSachRong(const DanhSachDangKy& ds)
{
    return ds.head == nullptr;
}

// Tạo node mới
NodeDangKy* taoNode(const DangKy& dk)
{
    NodeDangKy* node = new NodeDangKy;

    node->data = dk;
    node->next = nullptr;

    return node;
}

// Tìm sinh viên theo mã sinh viên
NodeDangKy* timSinhVien(
    const DanhSachDangKy& ds,
    const string& maSV
)
{
    NodeDangKy* current = ds.head;

    while (current != nullptr)
    {
        if (current->data.maSV == maSV)
        {
            return current;
        }

        current = current->next;
    }

    return nullptr;
}

// Kiểm tra sinh viên đã đăng ký chưa
bool daDangKy(
    const DanhSachDangKy& ds,
    const string& maSV
)
{
    NodeDangKy* node = timSinhVien(ds, maSV);

    if (node == nullptr)
    {
        return false;
    }

    // Nếu đã có node nhưng đã hủy,
    // ta vẫn xem là đã từng đăng ký
    return true;
}

// Thêm đăng ký
bool themDangKy(
    DanhSachDangKy& ds,
    const DangKy& dk
)
{
    // Không cho phép mã sinh viên rỗng
    if (dk.maSV.empty())
    {
        return false;
    }

    // Không cho phép trùng mã sinh viên
    if (daDangKy(ds, dk.maSV))
    {
        return false;
    }

    NodeDangKy* nodeMoi = taoNode(dk);

    // Trường hợp danh sách rỗng
    if (ds.head == nullptr)
    {
        ds.head = nodeMoi;
    }
    else
    {
        // Thêm vào cuối danh sách
        NodeDangKy* current = ds.head;

        while (current->next != nullptr)
        {
            current = current->next;
        }

        current->next = nodeMoi;
    }

    ds.soLuong++;

    return true;
}

// Xóa node theo mã sinh viên
bool xoaDangKy(
    DanhSachDangKy& ds,
    const string& maSV
)
{
    if (ds.head == nullptr)
    {
        return false;
    }

    NodeDangKy* current = ds.head;
    NodeDangKy* previous = nullptr;

    // Tìm node cần xóa
    while (
        current != nullptr &&
        current->data.maSV != maSV
    )
    {
        previous = current;
        current = current->next;
    }

    // Không tìm thấy
    if (current == nullptr)
    {
        return false;
    }

    // Xóa node đầu
    if (previous == nullptr)
    {
        ds.head = current->next;
    }
    else
    {
        // Bỏ qua node hiện tại
        previous->next = current->next;
    }

    delete current;
    ds.soLuong--;

    return true;
}

// Hủy đăng ký bằng cờ boolean
bool huyDangKy(
    DanhSachDangKy& ds,
    const string& maSV
)
{
    NodeDangKy* node = timSinhVien(ds, maSV);

    if (node == nullptr)
    {
        return false;
    }

    node->data.huyDangKy = true;

    return true;
}

// Khôi phục đăng ký
bool khoiPhucDangKy(
    DanhSachDangKy& ds,
    const string& maSV
)
{
    NodeDangKy* node = timSinhVien(ds, maSV);

    if (node == nullptr)
    {
        return false;
    }

    node->data.huyDangKy = false;

    return true;
}

// Kiểm tra điểm
bool diemHopLe(float diem)
{
    return diem >= 0.0f && diem <= 10.0f;
}

// Cập nhật điểm
bool capNhatDiem(
    DanhSachDangKy& ds,
    const string& maSV,
    float diemMoi
)
{
    if (!diemHopLe(diemMoi))
    {
        return false;
    }

    NodeDangKy* node = timSinhVien(ds, maSV);

    if (node == nullptr)
    {
        return false;
    }

    node->data.diem = diemMoi;

    return true;
}

// Đếm số sinh viên đang đăng ký
int demDangKyHienTai(
    const DanhSachDangKy& ds
)
{
    int dem = 0;

    NodeDangKy* current = ds.head;

    while (current != nullptr)
    {
        if (!current->data.huyDangKy)
        {
            dem++;
        }

        current = current->next;
    }

    return dem;
}

// In danh sách đăng ký
void inDanhSachDangKy(
    const DanhSachDangKy& ds
)
{
    if (ds.head == nullptr)
    {
        cout << "Danh sach dang ky rong.\n";
        return;
    }

    cout << left
         << setw(5) << "STT"
         << setw(15) << "MASV"
         << setw(10) << "DIEM"
         << setw(15) << "TRANG THAI"
         << '\n';

    cout << string(45, '-') << '\n';

    NodeDangKy* current = ds.head;
    int stt = 1;

    while (current != nullptr)
    {
        cout << left
             << setw(5) << stt
             << setw(15) << current->data.maSV;

        if (current->data.diem < 0)
        {
            cout << setw(10) << "Chua co";
        }
        else
        {
            cout << setw(10) << fixed
                 << setprecision(2)
                 << current->data.diem;
        }

        if (current->data.huyDangKy)
        {
            cout << setw(15) << "Da huy";
        }
        else
        {
            cout << setw(15) << "Dang ky";
        }

        cout << '\n';

        current = current->next;
        stt++;
    }
}

// Giải phóng toàn bộ danh sách
void giaiPhongDanhSach(
    DanhSachDangKy& ds
)
{
    NodeDangKy* current = ds.head;

    while (current != nullptr)
    {
        NodeDangKy* nodeCanXoa = current;

        current = current->next;

        delete nodeCanXoa;
    }

    ds.head = nullptr;
    ds.soLuong = 0;
}
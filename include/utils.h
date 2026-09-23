#ifndef UTILS_H
#define UTILS_H

// Xóa khoảng trắng thừa ở đầu, cuối và giữa các từ. Viết hoa chữ cái đầu (VD: "  nguyen   van A " -> "Nguyen Van A")
void ChuanHoaTen(char str[]);

// Xóa toàn bộ khoảng trắng thừa và viết hoa tất cả (Dùng cho Mã SV, Mã Lớp, Mã Môn)
void ChuanHoaMa(char str[]);

// Kiểm tra xem chuỗi có rỗng hoặc chỉ toàn khoảng trắng không
bool KiemTraRong(const char str[]);

#endif

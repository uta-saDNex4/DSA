#ifndef UTILS_H
#define UTILS_H

// Xóa khoảng trắng thừa ở đầu, cuối và giữa các từ. Viết hoa chữ cái đầu (VD: "  nguyen   van A " -> "Nguyen Van A")
void ChuanHoaTen(char str[]);

// Xóa toàn bộ khoảng trắng thừa và viết hoa tất cả (Dùng cho Mã SV, Mã Lớp, Mã Môn)
void ChuanHoaMa(char str[]);

// Kiểm tra xem chuỗi có rỗng hoặc chỉ toàn khoảng trắng không
bool KiemTraRong(const char str[]);

// Nhập số nguyên an toàn (chặn nhập chữ, ký tự đặc biệt)
int NhapSoNguyen(const char thongBao[]);

// Kiểm tra định dạng Niên khóa hợp lệ (dạng YYYY-YYYY với năm sau = năm trước + 1)
bool KiemTraNienKhoaHopLe(const char nk[]);

// Tách và chuẩn hóa Họ và Tên: Tên chỉ giữ từ cuối cùng. Nếu Tên có nhiều từ (VD: "Quang Thọ") thì tự đẩy các từ trước ("Quang") sang Họ
void TachVaChuanHoaHoTen(char ho[], char ten[], int maxHoLen = 51, int maxTenLen = 16);

// Nhập chuỗi từ bàn phím Console, tự động viết hoa chữ cái đầu ngay khi đang gõ phím (Real-time on-the-fly)
void NhapChuoiTuDongHoa(char buffer[], int maxLen, bool vietHoaChuDau = true);

#endif

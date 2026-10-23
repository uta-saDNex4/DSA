#ifndef DIEM_H
#define DIEM_H

#include "struct.h"

// Giới hạn phục vụ bảng điểm và thống kê
const int MAX_SV_DANGKY = 1000;    // Số SV tối đa trong 1 lớp tín chỉ
const int MAX_MON_TONGKET = 200;    // Số môn tối đa trong bảng tổng kết của 1 lớp
const int MAX_SV_LOP = 500;         // Số SV tối đa trong 1 lớp học

// Cấu trúc chứa thông tin SV và Điểm của 1 lớp tín chỉ (Câu i, j)
struct SinhVienDiem {
    char MASV[16];
    char HO[51];
    char TEN[16];
    float DIEM; // -1 nếu chưa nhập điểm, 0.0 - 10.0 nếu đã có điểm
};

// Cấu trúc chứa thông tin SV và Điểm trung bình kết thúc khóa học (Câu k)
struct SinhVienDiemTB {
    char MASV[16];
    char HO[51];
    char TEN[16];
    float DIEMTB;      // Điểm trung bình tích lũy theo số tín chỉ (-1 nếu chưa có điểm)
    int TONG_TIN_CHI;  // Tổng số tín chỉ tích lũy
};

// Cấu trúc một dòng trong Bảng điểm tổng kết (Câu l)
struct SinhVienDiemTK {
    char MASV[16];
    char HO[51];
    char TEN[16];
    float diemMon[MAX_MON_TONGKET]; // Điểm lớn nhất từng môn (-1 nếu chưa có điểm)
};

// Cấu trúc toàn bộ Bảng điểm tổng kết các môn của 1 lớp (Câu l)
struct BangDiemTongKet {
    char MALOP[16];
    char TENLOP[51];
    int soMon;                                     // Số môn học các SV trong lớp đã học/đăng ký
    char danhSachMAMH[MAX_MON_TONGKET][11];         // Danh sách Mã MH tương ứng từng cột
    char danhSachTENMH[MAX_MON_TONGKET][51];        // Tên môn học (để hiển thị chú giải)
    int danhSachSTC[MAX_MON_TONGKET];              // Tổng số tín chỉ của từng môn
    int soSV;                                      // Số SV trong lớp
    SinhVienDiemTK dsSV[MAX_SV_LOP];               // Dữ liệu từng sinh viên (các hàng)
};

// ====================================================================
// CÂU I: NHẬP ĐIỂM CHO LỚP TÍN CHỈ
// ====================================================================

// Lấy danh sách SV đăng ký của lớp tín chỉ để phục vụ nhập điểm (sắp xếp tăng dần theo Tên + Họ)
int LayDSSVDangKyLTC(const DS_LopTC &dsltc, const DS_LOPSV &dslop, int maLopTC, SinhVienDiem dsSV[], int maxSV);

// Lấy danh sách SV đăng ký qua tổ hợp Niên khóa, Học kỳ, Mã MH, Nhóm
int LayDSSVDangKyLTCTheoThongTin(const DS_LopTC &dsltc, const DS_LOPSV &dslop,
                                const char nienKhoa[], int hocKy, const char maMH[], int nhom,
                                SinhVienDiem dsSV[], int maxSV, int &maLopTCTimThay);

// Cập nhật điểm cho 1 sinh viên trong lớp tín chỉ (cho phép điểm 0.0 - 10.0 hoặc -1.0 để hủy điểm)
bool CapNhatDiemSinhVien(DS_LopTC &dsltc, int maLopTC, const char maSV[], float diemMoi);

// Giao diện nhập điểm trực tiếp trên Console (tự động lưu vào file)
void NhapDiemLTC_Console(DS_LopTC &dsltc, const DS_LOPSV &dslop, TreeMonHoc rootMH);


// ====================================================================
// CÂU J: IN BẢNG ĐIỂM MÔN HỌC CỦA 1 LỚP TÍN CHỈ
// ====================================================================

// In bảng điểm môn học của 1 lớp tín chỉ ra Console theo đúng định dạng đề bài
void InBangDiemMonHocLTC_Console(const DS_LopTC &dsltc, const DS_LOPSV &dslop, TreeMonHoc rootMH,
                                const char nienKhoa[], int hocKy, const char maMH[], int nhom);


// ====================================================================
// CÂU K: IN ĐIỂM TRUNG BÌNH KẾT THÚC KHÓA HỌC CỦA 1 LỚP THEO SỐ TÍN CHỈ
// ====================================================================

// Tính điểm trung bình tích lũy của 1 sinh viên (chỉ lấy điểm thi lớn nhất từng môn)
float TinhDiemTrungBinhSV(const DS_LopTC &dsltc, TreeMonHoc rootMH, const char maSV[], int &tongTinChi);

// Lấy danh sách ĐTB của tất cả SV trong 1 lớp (dùng cho cả Console và GUI)
int LayDSDiemTrungBinhLop(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                         const char maLop[], SinhVienDiemTB dsTB[], int maxSV);

// In bảng thống kê điểm trung bình khóa học ra Console theo đúng định dạng đề bài
void InDiemTrungBinhKhoaHocLop_Console(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                                      const char maLop[]);


// ====================================================================
// CÂU L: BẢNG ĐIỂM TỔNG KẾT CÁC MÔN CỦA CÁC SINH VIÊN THUỘC 1 LỚP
// ====================================================================

// Trích xuất ma trận bảng điểm tổng kết của 1 lớp (chỉ hiển thị điểm thi lớn nhất từng môn)
bool LayBangDiemTongKetLop(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                          const char maLop[], BangDiemTongKet &bangDiem);

// In bảng điểm tổng kết ra Console theo đúng định dạng ma trận lưới (có phân trang chống vỡ màn hình)
void InBangDiemTongKetLop_Console(const DS_LOPSV &dslop, const DS_LopTC &dsltc, TreeMonHoc rootMH,
                                 const char maLop[]);

#endif // DIEM_H

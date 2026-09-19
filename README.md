# ĐỀ TÀI MÔN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT

**Quản lý điểm sinh viên theo hệ tín chỉ**

## 1. Tổ chức các danh sách

### 1. Danh sách Môn học

- **Cấu trúc**: Cây nhị phân tìm kiếm (BST)
- **Thuộc tính**:
  - `MAMH` (C10)
  - `TENMH` (C50)
  - `STCLT`: Số tín chỉ lý thuyết (số nguyên)
  - `STCTH`: Số tín chỉ thực hành (số nguyên)

### 2. Danh sách Lớp
- **Cấu trúc**: Danh sách tuyến tính, tối đa 10000 lớp
- **Thuộc tính**:
  - `MALOP`
  - `TENLOP`
  - `con trỏ DSSV`: Con trỏ trỏ đến danh sách sinh viên thuộc lớp

### 3. Danh sách Sinh viên

- **Cấu trúc**: Danh sách liên kết đơn
- **Thuộc tính**:
  - `MASV` (kiểu chuỗi)
  - `HO`
  - `TEN`
  - `PHAI`
  - `SODT`

### 4. Danh sách Lớp tín chỉ

- **Cấu trúc**: Danh sách tuyến tính, là một mảng con trỏ, tối đa 10000 lớp
- **Thuộc tính**:
  - `MALOPTC`: Số nguyên, tự động tăng
  - `MAMH`
  - `NienKhoa`
  - `HocKy`
  - `Nhom`
  - `SoSVMin`: Số sinh viên tối thiểu
  - `SoSVMax`: Số sinh viên tối đa
  - `HuyLop`: Boolean, trạng thái hủy lớp
  - `con trỏ DSSVDK`: Con trỏ trỏ đến danh sách sinh viên đăng ký lớp tín chỉ

### 5. Danh sách Đăng ký

- **Cấu trúc**: Danh sách liên kết đơn
- **Thuộc tính**:
  - `MASV`
  - `DIEM`: Điểm thi (float)
  - `HuyDangKy`: Boolean, trạng thái hủy đăng ký

---

# 2. Các chức năng của chương trình

### a. Mở lớp tín chỉ

- Cho phép cập nhật thông tin lớp tín chỉ:
  - **Thêm**
  - **Xóa**
  - **Hiệu chỉnh**
- `MALOPTC` được chương trình tự động tăng.

### b. In danh sách sinh viên đã đăng ký lớp tín chỉ

- User nhập các thông tin:
  - `NienKhoa`
  - `HocKy`
  - `MAMH`
  - `Nhom`
- Chương trình tự động tìm lớp tín chỉ tương ứng.
- In danh sách sinh viên đã đăng ký.

**Kết xuất:**

| MASV  | Họ tên       | Trạng thái |
| ----- | ------------ | ---------- |
| SV001 | Nguyễn Văn A |            |
| SV002 | Trần Văn B   | Đã hủy     |

---

### c. Nhập sinh viên

- Nhập vào `MALOP` trước.
- Sau đó nhập danh sách sinh viên thuộc lớp.
- Danh sách sinh viên luôn được sắp xếp **tăng dần theo tên sinh viên**.

**Lưu ý:**

- Phải hỗ trợ đầy đủ 3 chức năng:
  - **Thêm**
  - **Xóa**
  - **Hiệu chỉnh**
- Quá trình nhập sinh viên mới kết thúc khi nhập `MASV` là chuỗi rỗng.

---

### d. In danh sách sinh viên của một lớp

- User nhập vào `MALOP`.
- Chương trình in danh sách sinh viên thuộc lớp.
- Danh sách được sắp xếp **alphabet tăng dần theo mã sinh viên (`MASV`)**.

**Kết xuất:**

| STT | MASV  | HỌ         | TÊN  | PHÁI | SĐT    |
| --- | ----- | ---------- | ---- | ---- | ------ |
| 1   | SV001 | Nguyễn Văn | An   | Nam  | 090... |
| 2   | SV002 | Trần Thị   | Bình | Nữ   | 091... |

---

### e. Nhập môn học

- Cho phép cập nhật thông tin môn học.
- Hỗ trợ đầy đủ:
  - **Thêm môn học**
  - **Xóa môn học**
  - **Hiệu chỉnh môn học**

**Thông tin môn học:**

- `MAMH`
- `TENMH`
- `STCLT`
- `STCTH`

---

### f. In danh sách môn học

- In danh sách tất cả môn học.
- Danh sách được sắp xếp **tăng dần theo tên môn học**.

**Kết xuất:**

| MAMH | TENMH            | STCLT | STCTH |
| ---- | ---------------- | ----- | ----- |
| CTDL | Cấu trúc dữ liệu | 3     | 1     |
| CSDL | Cơ sở dữ liệu    | 3     | 1     |
| LTM  | Lập trình mạng   | 3     | 0     |

---

### g. Đăng ký lớp tín chỉ

- User nhập vào `MASV`.
- Chương trình tự động hiển thị thông tin sinh viên.
- User nhập:
  - `NienKhoa`
  - `HocKy`
- Chương trình tự động lọc ra các lớp tín chỉ đang mở trong niên khóa và học kỳ tương ứng.

**Thông tin hiển thị:**

| MAMH | TENMH            | NHOM | Số SV đã đăng ký | Số slot còn trống |
| ---- | ---------------- | ---- | ---------------- | ----------------- |
| CTDL | Cấu trúc dữ liệu | 1    | 35               | 15                |
| CSDL | Cơ sở dữ liệu    | 2    | 40               | 10                |

- User chọn lớp tín chỉ muốn đăng ký.
- Chương trình kiểm tra các điều kiện đăng ký trước khi thêm sinh viên vào danh sách đăng ký.

---

### h. Hủy lớp tín chỉ

- Chương trình tự động tìm các lớp tín chỉ có:

```text
Số sinh viên đăng ký < Số sinh viên tối thiểu
```

## 👥 Nhóm thực hiện (Authors)

| STT | Họ và tên         | Mã sinh viên | Vai trò     |
| :-- | :---------------- | :----------- | :---------- |
| 1   | **Ngô Quang Thọ** | N24DECE043   | Trưởng nhóm |
| 2   | **Bùi Ngọc An**   | N24DECE001   | Thành viên  |
| 3   | **Lê Thành Long** | N24DECE028   | Thành viên  |

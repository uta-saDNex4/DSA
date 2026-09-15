# ĐỀ TÀI MÔN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT
**Quản lý điểm sinh viên theo hệ tín chỉ**

---

## 1. Quy tắc phát triển (Rules)
* **Ngôn ngữ**: C/C++ (thiên về C, hạn chế thư viện/hàm có sẵn như STL vector, map, sort).
* **Minh họa**: Mọi đoạn code minh họa dùng C/C++.
* **Xác nhận**: Luôn xác nhận với người dùng trước khi triển khai.
* **Tối ưu**: Phân tích độ phức tạp thuật toán (Big-O) trước khi viết code.
* **Edge cases**: Kiểm tra và xử lý đầy đủ các trường hợp biên.
* **Clean Code**: Đặt tên biến/hàm đúng chức năng, chú thích ngắn gọn cho các đoạn xử lý phức tạp.

---

## 2. Tổ chức các danh sách (Data Structures)

### 1. Danh sách Môn học
- **Cấu trúc**: Cây nhị phân tìm kiếm (BST)
- **Thuộc tính**:
  - `MAMH` (C10)
  - `TENMH` (C50)
  - `STCLT`: Số tín chỉ lý thuyết (số nguyên)
  - `STCTH`: Số tín chỉ thực hành (số nguyên)

### 2. Danh sách Lớp
- **Cấu trúc**: Danh sách tuyến tính, mảng con trỏ tối đa 10000 lớp
- **Thuộc tính**:
  - `MALOP` (C15)
  - `TENLOP` (C50)
  - `FirstSV`: Con trỏ trỏ đến danh sách sinh viên thuộc lớp

### 3. Danh sách Sinh viên
- **Cấu trúc**: Danh sách liên kết đơn
- **Thuộc tính**:
  - `MASV` (C15)
  - `HO` (C50)
  - `TEN` (C15)
  - `PHAI` (C3)
  - `SODT` (C15)

### 4. Danh sách Lớp tín chỉ
- **Cấu trúc**: Danh sách tuyến tính, mảng con trỏ tối đa 10000 lớp
- **Thuộc tính**:
  - `MALOPTC`: Số nguyên, tự động tăng
  - `MAMH` (C10)
  - `NienKhoa` (C9)
  - `HocKy` (int)
  - `Nhom` (int)
  - `SoSVMin`: Số sinh viên tối thiểu
  - `SoSVMax`: Số sinh viên tối đa
  - `HuyLop`: Boolean, trạng thái hủy lớp
  - `FirstDK`: Con trỏ trỏ đến danh sách sinh viên đăng ký lớp tín chỉ

### 5. Danh sách Đăng ký
- **Cấu trúc**: Danh sách liên kết đơn
- **Thuộc tính**:
  - `MASV` (C15)
  - `DIEM`: Điểm thi (float)
  - `HuyDangKy`: Boolean, trạng thái hủy đăng ký

---

## 3. Các chức năng của chương trình

### a. Mở lớp tín chỉ
- Cho phép Cập nhật (Thêm / Xóa / Hiệu chỉnh) thông tin lớp tín chỉ.
- `MALOPTC` được chương trình cấp phát tự động tăng.

### b. In danh sách sinh viên đã đăng ký lớp tín chỉ
- Tham số nhập: `NienKhoa`, `HocKy`, `MAMH`, `Nhom`.
- Kết xuất: `Mã SV` | `Họ tên` | `Trạng thái (đã hủy)`.

### c. Nhập sinh viên của một lớp
- Nhập vào `MALOP` trước, sau đó nhập danh sách sinh viên.
- Danh sách sinh viên luôn được chèn theo thứ tự **tăng dần theo Tên + Họ**.
- Phải hỗ trợ đủ 3 chức năng: **Thêm / Xóa / Hiệu chỉnh**.
- Quá trình nhập dừng khi `MASV` là chuỗi rỗng.

### d. In danh sách sinh viên của một lớp
- Nhập vào `MALOP`.
- In danh sách sinh viên thuộc lớp theo **thứ tự alphabet tăng dần của Mã SV (`MASV`)**.

### e. Nhập môn học
- Cho phép Cập nhật (**Thêm / Xóa / Hiệu chỉnh**) thông tin môn học trên Cây nhị phân tìm kiếm (BST).

### f. In danh sách môn học
- In tất cả các môn học theo thứ tự **tăng dần theo Tên môn học**.

### g. Đăng ký lớp tín chỉ
- Nhập `MASV` $\rightarrow$ Tự động in thông tin sinh viên.
- Nhập `NienKhoa`, `HocKy` $\rightarrow$ Tự động lọc các lớp tín chỉ mở trong niên khóa, học kỳ đó.
- Hiển thị: `MAMH` | `TENMH` | `NHOM` | `Số SV đã đăng ký` | `Số slot còn trống`.
- Chọn lớp để đăng ký và lưu vào DSLK Đăng ký.

### h. Hủy lớp tín chỉ
- Tự động lọc các lớp tín chỉ có số sinh viên đăng ký $< SoSVMin$ trong niên khóa, học kỳ nhập vào.
- Cần sự xác nhận của người dùng trước khi hủy.

### i. Nhập điểm
- Nhập `NienKhoa`, `HocKy`, `MAMH`, `Nhom`.
- Tự động lọc và hiển thị danh sách sinh viên đã đăng ký.
- Cho phép nhập hoặc hiệu chỉnh điểm thi (0.0 đến 10.0) trực tiếp trên cột điểm.

### j. In bảng điểm môn học của 1 lớp tín chỉ
- Nhập `NienKhoa`, `HocKy`, `MAMH`, `Nhom`.
- In bảng điểm thi của môn học: `STT` | `MASV` | `HO` | `TEN` | `DIEM`.

### k. In điểm trung bình kết thúc khóa học của 1 lớp
- Nhập `MALOP`.
- In bảng thống kê điểm trung bình khóa học theo số tín chỉ (lý thuyết + thực hành):
  $$\text{ĐTB} = \frac{\sum (\text{Điểm môn} \times \text{Tổng STC})}{\sum \text{Tổng STC}}$$

### l. In bảng điểm tổng kết các môn của sinh viên thuộc 1 lớp
- Nhập `MALOP`.
- In bảng điểm tổng kết lưới: `STT` | `Mã SV` | `Họ Tên` | `Mã MH1` | `Mã MH2` | ... | `Mã MHn`.
- Mỗi môn chỉ hiển thị điểm thi lớn nhất của sinh viên.

---

## 4. Lưu trữ & Kiểm tra dữ liệu
- Cho phép lưu và đọc toàn bộ các danh sách ra/vào file.
- Kiểm tra chặt chẽ các điều kiện khi nhập liệu (không nhập trống, trùng khóa, vượt giới hạn sĩ số,...).

---

## 👥 Nhóm thực hiện (Authors)
| STT | Họ và tên | Mã sinh viên | Vai trò |
| :--- | :--- | :--- | :--- |
| 1 | **Ngô Quang Thọ** | N24DECE043 | Trưởng nhóm |
| 2 | **Bùi Ngọc An** | N24DECE001 | Thành viên |
| 3 | **Lê Thành Long** | N24DECE028 | Thành viên |

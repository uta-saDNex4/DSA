# TÀI LIỆU PHÂN TÍCH MÃ NGUỒN & KỊCH BẢN VẤN ĐÁP BẢO VỆ
## DÀNH RIÊNG CHO NGÔ QUANG THỌ (TRƯỞNG NHÓM — MSSV: N24DECE043)
**Đồ án:** Quản lý điểm sinh viên theo hệ tín chỉ — Học viện Công nghệ Bưu chính Viễn thông (PTIT)  
**Phạm vi trách nhiệm:** Phân hệ Môn học (Cây BST), Phân hệ Lớp & Sinh viên (Mảng + DSLK có thứ tự), Module Ràng buộc toàn vẹn (`rang_buoc.cpp`), Module Tiện ích (`utils.cpp`), File I/O Master Data, Tích hợp hệ thống & Qt GUI.

---

## MỤC LỤC
1. [PHẦN 1: MÔN HỌC — CÂY NHỊ PHÂN TÌM KIẾM (BST) — CÂU E, F](#phần-1-môn-học--cây-nhị-phân-tìm-kiếm-bst--câu-e-f)
   - [1.1. Cấu trúc Node & Cây BST Môn học](#11-cấu-trúc-node--cây-bst-môn-học)
   - [1.2. Thêm môn học vào Cây BST](#12-thêm-môn-học-vào-cây-bst)
   - [1.3. Xóa môn học — Kỹ thuật Node Thế Mạng 2 Con](#13-xóa-môn-học--kỹ-thuật-node-thế-mạng-2-con)
   - [1.4. In danh sách môn học tăng dần theo Tên (In-Order + QuickSort)](#14-in-danh-sách-môn-học-tăng-dần-theo-tên-in-order--quicksort)
   - [1.5. Giải phóng bộ nhớ Cây BST (Post-Order Traversal)](#15-giải-phóng-bộ-nhớ-cây-bst-post-order-traversal)
2. [PHẦN 2: LỚP HỌC & SINH VIÊN (DSLK ĐƠN CÓ THỨ TỰ) — CÂU C, D](#phần-2-lớp-học--sinh-viên-dslk-đơn-có-thứ-tự--câu-c-d)
   - [2.1. Cấu trúc Mảng đặc Lớp & Hàm Thêm/Xóa Lớp](#21-cấu-trúc-mảng-đặc-lớp--hàm-thêmxóa-lớp)
   - [2.2. So sánh 3 tầng & Chèn sinh viên có thứ tự Tên + Họ](#22-so-sánh-3-tầng--chèn-sinh-viên-có-thứ-tự-tên--họ)
   - [2.3. Hiệu chỉnh sinh viên & Xử lý lệch thứ tự Alphabet](#23-hiệu-chỉnh-sinh-viên--xử-lý-lệch-thứ-tự-alphabet)
   - [2.4. In DSSV theo Mã SV bằng Mảng con trỏ & QuickSort](#24-in-dssv-theo-mã-sv-bằng-mảng-con-trỏ--quicksort)
3. [PHẦN 3: MODULE RÀNG BUỘC TOÀN VẸN THAM CHIẾU (REFERENTIAL INTEGRITY)](#phần-3-module-ràng-buộc-toàn-vẹn-tham-chiếu-referential-integrity)
   - [3.1. Các hàm kiểm tra khóa ngoại](#31-các-hàm-kiểm-tra-khóa-ngoại)
   - [3.2. Tối ưu truyền tham chiếu `const DS_LopTC &dsltc` (Chống tràn Stack)](#32-tối-ưu-truyền-tham-chiếu-const-ds_loptc-dsltc-chống-tràn-stack)
   - [3.3. Bộ 5 kịch bản cấm xóa của hệ thống](#33-bộ-5-kịch-bản-cấm-xóa-của-hệ-thống)
4. [PHẦN 4: MODULE TIỆN ÍCH CHUẨN HÓA & VALIDATION (`utils.cpp`)](#phần-4-module-tiện-ích-chuẩn-hóa--validation-utilscpp)
4. [PHẦN 4: MODULE TIỆN ÍCH CHUẨN HÓA & VALIDATION (`utils.cpp`)](#phần-4-module-tiện-ích-chuẩn-hóa--validation-utilscpp)
   - [4.1. Chuẩn hóa tên (`ChuanHoaTen`)](#41-chuẩn-hóa-tên-chuanhoaten)
   - [4.2. Chuẩn hóa mã (`ChuanHoaMa`)](#42-chuẩn-hóa-mã-chuanhoama)
   - [4.3. Nhập số nguyên an toàn (`NhapSoNguyen`)](#43-nhập-số-nguyên-an-toàn-nhapsonguyen)
   - [4.4. Tự động tách Họ - Tên & Đảm bảo Tên chỉ có 1 từ (`TachVaChuanHoaHoTen`)](#44-tự-động-tách-họ---tên--đảm-bảo-tên-chỉ-có-1-từ-tachvachuanhoahoten)
   - [4.5. Cơ chế Tự động Viết hoa Chữ cái đầu Real-Time On-The-Fly](#45-cơ-chế-tự-động-viết-hoa-chữ-cái-đầu-real-time-on-the-fly)
5. [PHẦN 5: CƠ CHẾ GHI FILE AN TOÀN NGUYÊN TỬ (ATOMIC FILE I/O)](#phần-5-cơ-chế-ghi-file-an-toàn-nguyên-tử-atomic-file-io)
6. [PHẦN 6: BỘ ĐỀ VẤN ĐÁP THỰC CHIẾN — HỎI & ĐÁP THEO TỪNG CÂU HỎI TRONG ĐỀ BÀI](#phần-6-bộ-đề-vấn-đáp-thực-chiến--hỏi--đáp-theo-từng-câu-hỏi-trong-đề-bài)

---

## PHẦN 1: MÔN HỌC — CÂY NHỊ PHÂN TÌM KIẾM (BST) — CÂU E, F
*Mã nguồn:* [`include/monhoc.h`](file:///d:/PTIT/DSA/include/monhoc.h) và [`src/monhoc.cpp`](file:///d:/PTIT/DSA/src/monhoc.cpp)

### 1.1. Cấu trúc Node & Cây BST Môn học
```cpp
struct MonHoc {
    char MAMH[11];   // Khóa tìm kiếm phân cấp cây BST (chuỗi <= 10 ký tự)
    char TENMH[51];  // Tên môn học
    int STCLT;       // Số tín chỉ lý thuyết (>= 0)
    int STCTH;       // Số tín chỉ thực hành (>= 0)
};

struct nodeMonHoc {
    MonHoc mh;
    nodeMonHoc *left = nullptr;   // Con trỏ nhánh con trái (< MAMH)
    nodeMonHoc *right = nullptr;  // Con trỏ nhánh con phải (> MAMH)
};
typedef nodeMonHoc *TreeMonHoc;

struct DS_MonHoc {
    TreeMonHoc root = nullptr;
    int n = 0; // Biến đếm tổng số môn học trong cây
};
```

#### 🔍 Phân tích mã nguồn:
- **Tổ chức bộ nhớ:** Mỗi môn học là một `nodeMonHoc` nằm rải rác trên Heap, liên kết với nhau bằng 2 con trỏ `left` và `right`.
- **Khóa định danh cây:** `MAMH` là khóa chính. Mọi phép so sánh đều dùng hàm `stricmp` (chuẩn C, so sánh không phân biệt hoa thường).

---

### 1.2. Thêm môn học vào Cây BST
```cpp
void ThemMonHoc(TreeMonHoc &root, MonHoc mh, int &n) {
    if (root == nullptr) {
        root = new nodeMonHoc;
        root->mh = mh;
        root->left = nullptr;
        root->right = nullptr;
        n++;
        return;
    }
    
    int cmp = stricmp(mh.MAMH, root->mh.MAMH);
    if (cmp < 0) {
        ThemMonHoc(root->left, mh, n);  // Khóa nhỏ hơn -> rẽ trái
    } else if (cmp > 0) {
        ThemMonHoc(root->right, mh, n); // Khóa lớn hơn -> rẽ phải
    } else {
        cout << "[!] Loi: Ma mon hoc '" << mh.MAMH << "' da ton tai!\n";
    }
}
```

#### 🔍 Phân tích chi tiết từng dòng code:
- `TreeMonHoc &root`: Bắt buộc phải truyền **tham chiếu con trỏ (`&`)**. Nếu chỉ truyền `TreeMonHoc root`, khi đệ quy đến `root == nullptr` và gọi `root = new nodeMonHoc`, địa chỉ mới chỉ được gán cho bản sao cục bộ, khi hàm kết thúc con trỏ của node cha vẫn sẽ trỏ vào `nullptr`, làm mất node mới tạo.
- `int cmp = stricmp(mh.MAMH, root->mh.MAMH);`:
  - `cmp < 0`: Đệ quy sang nhánh trái.
  - `cmp > 0`: Đệ quy sang nhánh phải.
  - `cmp == 0`: Trùng khóa `MAMH`, hệ thống từ chối thêm để bảo toàn tính duy nhất của khóa chính.
- `n++`: Biến đếm được tăng ngay khi cấp phát node thành công, giúp hàm `InDanhSachMonHoc` biết chính xác kích thước mảng tạm cần cấp phát mà không cần phải duyệt đếm lại cây.

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Tại sao tham số `root` trong hàm `ThemMonHoc` lại có dấu `&` (`TreeMonHoc &root`)?"*  
> **Thọ trả lời:** *"Thưa thầy, kiểu `TreeMonHoc` bản chất là một con trỏ `nodeMonHoc*`. Dấu `&` biến nó thành tham chiếu con trỏ. Khi đệ quy đến điểm dừng `root == nullptr`, ta gọi `root = new nodeMonHoc`. Nhờ có dấu `&`, con trỏ `left` hoặc `right` của node cha ở tầng trước đó mới được gán trực tiếp địa chỉ của vùng nhớ mới cấp phát. Nếu không có `&`, lệnh `new` chỉ tác động lên bản sao trên stack, node mới tạo sẽ bị cô lập và gây rò rỉ bộ nhớ."*

> **Thầy hỏi:** *"Điều kiện khi nhập số tín chỉ môn học là gì?"*  
> **Thọ trả lời:** *"Thưa thầy, trong hàm `NhapMonHoc`, hệ thống kiểm tra: $STCLT \ge 0$, $STCTH \ge 0$ và bắt buộc tổng $STCLT + STCTH > 0$. Một môn học không thể có cả lý thuyết và thực hành đều bằng 0 tín chỉ."*

---

### 1.3. Xóa môn học — Kỹ thuật Node Thế Mạng 2 Con
```cpp
// Hàm phụ trợ tìm node thế mạng: Node PHẢI NHẤT của cây con TRÁI
void NodeTheMang(TreeMonHoc &p, TreeMonHoc &rightMost) {
    if (rightMost->right != nullptr) {
        NodeTheMang(p, rightMost->right); // Tiếp tục đi sang phải để tìm khóa lớn nhất
    } else {
        p->mh = rightMost->mh;         // 1. Sao chép dữ liệu từ node thế mạng vào p
        p = rightMost;                 // 2. Trỏ p tới node thế mạng để delete
        rightMost = rightMost->left;   // 3. Móc nhánh con trái của node thế mạng lên cha
    }
}

bool XoaMonHocNoiB(TreeMonHoc &root, const char MAMH[], int &n) {
    if (root == nullptr) return false;
    
    int cmp = stricmp(MAMH, root->mh.MAMH);
    if (cmp < 0) {
        return XoaMonHocNoiB(root->left, MAMH, n);
    } else if (cmp > 0) {
        return XoaMonHocNoiB(root->right, MAMH, n);
    } else {
        // ĐÃ TÌM THẤY NODE CẦN XÓA (root)
        TreeMonHoc p = root;
        
        if (root->left == nullptr) {
            root = root->right; // Trường hợp 1 & 2: Có 1 con phải hoặc node lá
        } else if (root->right == nullptr) {
            root = root->left;  // Trường hợp 2: Có 1 con trái
        } else {
            // Trường hợp 3: Có đủ 2 con -> Tìm node phải nhất cây con trái
            NodeTheMang(p, root->left);
        }
        delete p; // Giải phóng bộ nhớ của node cần xóa (hoặc node thế mạng)
        n--;
        return true;
    }
}
```

#### 🔍 Phân tích chi tiết từng dòng code:
- Khi node $P$ cần xóa có 2 con: Ta không thể ngắt liên kết của $P$ vì sẽ làm mất 2 cây con bên dưới.
- Ta gọi `NodeTheMang(p, root->left)`:
  - `root->left`: Bắt đầu đi vào cây con bên trái.
  - Vòng lặp/đệ quy `rightMost->right != nullptr`: Đi liên tục sang phải cho đến khi gặp node không còn con phải. Đây chính là **In-order Predecessor** (node có khóa lớn nhất trong tập các node nhỏ hơn $P$).
  - `p->mh = rightMost->mh`: Ghi đè dữ liệu môn học sang $P$.
  - `p = rightMost`: Lưu địa chỉ node thế mạng vào con trỏ `p`.
  - `rightMost = rightMost->left`: Móc con trái duy nhất của node thế mạng lên vị trí của nó ở node cha.
  - `delete p`: Lệnh delete ở hàm ngoài sẽ giải phóng đúng vùng nhớ của node thế mạng ban đầu. Cấu trúc cây BST được bảo toàn hoàn hảo!

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Tại sao khi xóa node có 2 con, em lại chọn node phải nhất của cây con trái? Có cách nào khác không?"*  
> **Thọ trả lời:** *"Thưa thầy, để thay thế cho một node mà không phá vỡ tính chất cây BST, ta có 2 lựa chọn:  
> 1. Chọn phần tử **phải nhất của cây con bên trái** (phần tử lớn nhất trong nhánh nhỏ hơn node cần xóa) — đây là cách em đã cài đặt trong hàm `NodeTheMang`.  
> 2. Chọn phần tử **trái nhất của cây con bên phải** (phần tử nhỏ nhất trong nhánh lớn hơn node cần xóa).  
> Cả 2 cách đều đúng chuẩn giải thuật. Node thế mạng này chắc chắn chỉ có tối đa 1 nhánh con, nên việc xóa nó rất đơn giản và không làm đứt cây."*

> **Thầy hỏi:** *"Khi nào thì hàm `XoaMonHoc` không cho phép xóa môn học?"*  
> **Thọ trả lời:** *"Thưa thầy, trước khi gọi `XoaMonHocNoiB`, hàm `XoaMonHoc` sẽ gọi `DemLopTCByMAMH(dsltc, ma)`. Nếu môn học này đã từng được mở bất kỳ lớp tín chỉ nào (kể cả lớp đã hủy), hệ thống **tuyệt đối chặn xóa** để đảm bảo tính toàn vẹn dữ liệu, đồng thời in ra màn hình danh sách các lớp tín chỉ đang ràng buộc."*

---

### 1.4. In danh sách môn học tăng dần theo Tên (In-Order + QuickSort)
```cpp
// 1. Duyệt In-Order đưa con trỏ vào mảng tạm
void InOrderTraversal(TreeMonHoc root, TreeMonHoc* arr, int &idx) {
    if (root != nullptr) {
        InOrderTraversal(root->left, arr, idx);
        arr[idx++] = root;                       // Lưu con trỏ node vào mảng
        InOrderTraversal(root->right, arr, idx);
    }
}

// 2. QuickSort mảng con trỏ theo Tên môn học
void QuickSortMH(TreeMonHoc* arr, int low, int high) {
    if (low < high) {
        // Khử trường hợp xấu nhất O(N^2) bằng cách lấy phần tử ở giữa làm pivot
        int mid = low + (high - low) / 2;
        TreeMonHoc t = arr[mid]; arr[mid] = arr[high]; arr[high] = t;
        
        char* pivot = arr[high]->mh.TENMH;
        int i = (low - 1);
        for (int j = low; j <= high - 1; j++) {
            if (stricmp(arr[j]->mh.TENMH, pivot) < 0) {
                i++;
                TreeMonHoc temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }
        }
        TreeMonHoc temp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = temp;
        int pi = i + 1;
        QuickSortMH(arr, low, pi - 1);
        QuickSortMH(arr, pi + 1, high);
    }
}

// 3. Hàm điều phối
void InDanhSachMonHoc(TreeMonHoc root, int n) {
    if (root == nullptr || n == 0) {
        cout << "[!] Danh sach mon hoc rong!\n";
        return;
    }
    TreeMonHoc* arr = new TreeMonHoc[n]; // Cấp phát mảng con trỏ tạm kích thước n
    int idx = 0;
    InOrderTraversal(root, arr, idx);    // Bước 1: Gom n con trỏ (O(N))
    
    if (n > 1) QuickSortMH(arr, 0, n - 1); // Bước 2: QuickSort theo Tên (O(N log N))
    
    // Bước 3: In bảng ra màn hình
    for (int i = 0; i < n; i++) {
        cout << setw(5) << left << (i + 1)
             << setw(15) << left << arr[i]->mh.MAMH 
             << setw(40) << left << arr[i]->mh.TENMH
             << setw(10) << left << arr[i]->mh.STCLT
             << setw(10) << left << arr[i]->mh.STCTH << endl;
    }
    delete[] arr; // Bước 4: Giải phóng mảng tạm (Cây BST không bị ảnh hưởng)
}
```

#### 🔍 Phân tích chi tiết từng dòng code:
- `TreeMonHoc* arr = new TreeMonHoc[n];`: Mảng `arr` chứa các con trỏ trỏ tới từng node trên cây. Kích thước $n \times 8 \text{ bytes}$ (với 100 môn học chỉ tốn 800 bytes RAM).
- Khi hoán vị trong QuickSort: `TreeMonHoc temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;` $\rightarrow$ Ta chỉ tráo đổi địa chỉ con trỏ trong mảng `arr`, không di chuyển hay sao chép struct `MonHoc`, và hoàn toàn **không đụng chạm đến các liên kết `left`/`right` của Cây BST gốc**.
- `delete[] arr;`: Thu hồi vùng nhớ mảng con trỏ tạm. Cây BST trên Heap vẫn tồn tại bình thường.

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Tại sao cây BST đang có sẵn mà em không in luôn, lại phải tạo mảng tạm rồi QuickSort?"*  
> **Thọ trả lời:** *"Thưa thầy, cây BST được tổ chức sắp xếp theo `MAMH`. Nếu duyệt cây trực tiếp (In-Order), ta chỉ thu được danh sách tăng dần theo Mã môn học. Đề bài câu f yêu cầu in tăng dần theo **Tên môn học (`TENMH`)**. Cây BST theo mã không thể tự động xuất ra thứ tự theo tên được. Vì vậy, giải pháp tối ưu nhất là dùng In-Order đưa các con trỏ node vào mảng tạm mất $O(N)$, rồi dùng QuickSort sắp xếp theo `TENMH` mất $O(N \log N)$. Cách này chạy cực nhanh và không làm đảo lộn cấu trúc cây gốc."*

---

### 1.5. Giải phóng bộ nhớ Cây BST (Post-Order Traversal)
```cpp
void GiaiPhongCay(TreeMonHoc &root) {
    if (root != nullptr) {
        GiaiPhongCay(root->left);  // 1. Giải phóng toàn bộ cây con trái
        GiaiPhongCay(root->right); // 2. Giải phóng toàn bộ cây con phải
        delete root;               // 3. Giải phóng node cha
        root = nullptr;
    }
}
```

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Tại sao giải phóng cây BST lại phải duyệt Post-Order (Hậu thứ tự) mà không dùng Pre-Order?"*  
> **Thọ trả lời:** *"Thưa thầy, nếu dùng Pre-Order (giải phóng cha trước rồi mới giải phóng con), khi gọi `delete root`, vùng nhớ chứa 2 con trỏ `root->left` và `root->right` đã bị hủy, ta sẽ không còn địa chỉ hợp lệ để duyệt tiếp sang 2 cây con, dẫn đến lỗi truy cập bộ nhớ bất hợp pháp hoặc rò rỉ bộ nhớ. Duyệt Post-Order đảm bảo giải phóng đệ quy sạch sẽ toàn bộ 2 cây con trước rồi mới thu hồi node cha."*

---

## PHẦN 2: LỚP HỌC & SINH VIÊN (DSLK ĐƠN CÓ THỨ TỰ) — CÂU C, D
*Mã nguồn:* [`include/lop.h`](file:///d:/PTIT/DSA/include/lop.h), [`src/lop.cpp`](file:///d:/PTIT/DSA/src/lop.cpp), [`include/sinhvien.h`](file:///d:/PTIT/DSA/include/sinhvien.h), [`src/sinhvien.cpp`](file:///d:/PTIT/DSA/src/sinhvien.cpp)

### 2.1. Cấu trúc Mảng đặc Lớp & Hàm Thêm/Xóa Lớp
```cpp
struct LOPSV {
    char MALOP[16];
    char TENLOP[51];
    PTRSV FirstSV = nullptr; // Con trỏ đầu DSLK sinh viên của lớp
};

struct DS_LOPSV {
    int n = 0;
    LOPSV nodes[MAX_LOPSV]; // Mảng đặc tĩnh 10.000 phần tử (Rule đề tài)
};
```

#### Xóa Lớp Sinh Viên (`XoaLop`):
```cpp
bool XoaLop(DS_LOPSV &ds, const char MALOP[], const DS_LopTC &dsltc) {
    int idx = TimLop(ds, MALOP);
    if (idx == -1) return false;
    
    // RÀNG BUỘC 1: Lớp còn sinh viên không?
    if (ds.nodes[idx].FirstSV != nullptr) {
        cout << "[!] Khong the xoa! Lop dang co sinh vien. Phai xoa het SV truoc.\n";
        return false;
    }
    
    // RÀNG BUỘC 2: Sinh viên trong lớp có ai đang đăng ký LTC không?
    if (DemDangKyByLop(dsltc, ds.nodes[idx].FirstSV) > 0) return false;
    
    // DỒN MẢNG ĐẶC KHI XÓA
    for (int i = idx; i < ds.n - 1; i++) {
        ds.nodes[i] = ds.nodes[i + 1];
    }
    ds.nodes[ds.n - 1].FirstSV = nullptr;
    ds.n--;
    return true;
}
```

---

### 2.2. So sánh 3 tầng & Chèn sinh viên có thứ tự Tên + Họ
```cpp
int SoSanhTen(SinhVien a, SinhVien b) {
    int cmpTen = stricmp(a.TEN, b.TEN);
    if (cmpTen != 0) return cmpTen; // Ưu tiên 1: Tên (A-Z)
    
    int cmpHo = stricmp(a.HO, b.HO);
    if (cmpHo != 0) return cmpHo;   // Ưu tiên 2: Họ và tên đệm
    
    return stricmp(a.MASV, b.MASV); // Ưu tiên 3: Mã sinh viên
}

void InsertOrderSV(PTRSV &First, SinhVien sv) {
    PTRSV p = new nodeSV;
    p->sv = sv;
    p->next = nullptr;
    
    // Trường hợp 1: DSLK rỗng HOẶC node mới nhỏ hơn node đầu -> chèn vào đầu
    if (First == nullptr || SoSanhTen(First->sv, p->sv) > 0) {
        p->next = First;
        First = p;
        return;
    }
    
    // Trường hợp 2: Chèn vào giữa hoặc cuối danh sách
    PTRSV curr = First;
    while (curr->next != nullptr && SoSanhTen(curr->next->sv, p->sv) < 0) {
        curr = curr->next;
    }
    p->next = curr->next;
    curr->next = p;
}
```

#### 🔍 Phân tích chi tiết từng dòng code:
- `while (curr->next != nullptr && SoSanhTen(curr->next->sv, p->sv) < 0)`: Con trỏ `curr` đứng lại ngay tại vị trí node mà node đứng sau nó (`curr->next`) lớn hơn hoặc bằng node mới `p`.
- `p->next = curr->next; curr->next = p;`: Phép móc nối 2 bước kinh điển của Danh sách liên kết đơn, không làm đứt mạch liên kết.

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Nếu có 2 sinh viên cùng tên 'An', ví dụ 'Nguyễn Văn An' và 'Trần Văn An', hàm chèn xử lý thế nào?"*  
> **Thọ trả lời:** *"Thưa thầy, trong hàm `SoSanhTen`, khi so sánh Tên `stricmp(a.TEN, b.TEN) == 0`, hàm sẽ tự động kích hoạt so sánh tầng thứ hai là Họ `stricmp(a.HO, b.HO)`. Chuỗi 'Nguyen Van' có mã ASCII nhỏ hơn 'Tran Van' nên 'Nguyễn Văn An' sẽ được chèn đứng trước 'Trần Văn An'. Danh sách liên kết luôn đảm bảo trật tự từ điển tuyệt đối."*

---

### 2.3. Hiệu chỉnh sinh viên & Xử lý lệch thứ tự Alphabet
```cpp
void SuaSV(PTRSV &First, const char MASV[]) {
    PTRSV p = TimSV(First, MASV);
    if (p == nullptr) return;
    
    SinhVien temp = p->sv;
    // ... nhập Họ mới, Tên mới, Phái mới, SĐT mới vào biến temp ...
    
    // BẢO TOÀN TRẬT TỰ ALPHABET
    if (stricmp(temp.TEN, p->sv.TEN) != 0 || stricmp(temp.HO, p->sv.HO) != 0) {
        XoaSVNoiB(First, MASV);     // 1. Tách và xóa node cũ khỏi DSLK
        InsertOrderSV(First, temp);  // 2. Chèn lại dữ liệu mới vào đúng vị trí alphabet
    } else {
        p->sv = temp; // Nếu chỉ đổi Phái hoặc SĐT thì ghi đè tại chỗ
    }
}
```

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Nếu một sinh viên đang đứng ở đầu danh sách (vần A), người dùng đổi tên sinh viên đó thành vần V, chương trình của em có bị sai thứ tự không?"*  
> **Thọ trả lời:** *"Thưa thầy, em đã lường trước trường hợp này trong hàm `SuaSV`. Nếu người dùng thay đổi Tên hoặc Họ, hệ thống sẽ gọi `XoaSVNoiB` để tách node cũ ra, sau đó gọi `InsertOrderSV` chèn lại với thông tin mới. Thuật toán chèn có thứ tự sẽ duyệt tìm lại vị trí mới (vần V) để móc nối vào, đảm bảo danh sách không bao giờ bị xáo trộn trật tự."*

---

### 2.4. In DSSV theo Mã SV bằng Mảng con trỏ & QuickSort
```cpp
void InDSSVTheoMa(PTRSV First) {
    if (First == nullptr) return;
    
    int count = DemSV(First);
    PTRSV* arr = new PTRSV[count]; // Cấp phát mảng con trỏ tạm
    int i = 0;
    for (PTRSV p = First; p != nullptr; p = p->next) {
        arr[i++] = p; // Đưa địa chỉ node vào mảng
    }
    
    // QuickSort mảng con trỏ theo MASV
    if (count > 1) QuickSortSV(arr, 0, count - 1);
    
    // In bảng
    for (int k = 0; k < count; k++) {
        cout << setw(5) << left << (k + 1)
             << setw(15) << left << arr[k]->sv.MASV 
             << setw(35) << left << (string(arr[k]->sv.HO) + " " + arr[k]->sv.TEN)
             << setw(10) << left << arr[k]->sv.PHAI
             << setw(15) << left << arr[k]->sv.SODT << endl;
    }
    delete[] arr; // Giải phóng mảng con trỏ tạm
}
```

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Tại sao ở câu d (in theo Mã SV), em lại dùng mảng con trỏ `PTRSV* arr` mà không sắp xếp trực tiếp trên danh sách liên kết?"*  
> **Thọ trả lời:** *"Thưa thầy, vì đề bài câu c quy định Danh sách liên kết sinh viên của lớp phải luôn luôn được duy trì ở trạng thái có thứ tự theo Tên + Họ. Nếu ta sắp xếp trực tiếp DSLK theo Mã SV thì trật tự theo Tên + Họ sẽ bị phá vỡ hoàn toàn, lần sau muốn in theo Tên lại phải sắp xếp lại.  
> Do đó, em dùng mảng con trỏ tạm `arr` để gom địa chỉ các node rồi QuickSort mảng tạm. Danh sách liên kết gốc `First` hoàn toàn không bị ảnh hưởng, thời gian chạy chỉ mất $O(K \log K)$."*

---

## PHẦN 3: MODULE RÀNG BUỘC TOÀN VẸN THAM CHIẾU (REFERENTIAL INTEGRITY)
*Mã nguồn:* [`include/rang_buoc.h`](file:///d:/PTIT/DSA/include/rang_buoc.h) và [`src/rang_buoc.cpp`](file:///d:/PTIT/DSA/src/rang_buoc.cpp)

### 3.1. Các hàm kiểm tra khóa ngoại
```cpp
// 1. Kiểm tra sinh viên có đang đăng ký lớp tín chỉ nào chưa hủy không
int DemDangKyByMASV(const DS_LopTC &dsltc, const char MASV[]);

// 2. Kiểm tra sinh viên đã có điểm thi ở bất kỳ môn nào chưa (DIEM >= 0)
bool SVDaCoDiem(const DS_LopTC &dsltc, const char MASV[]);

// 3. Kiểm tra môn học đã mở lớp tín chỉ nào chưa
int DemLopTCByMAMH(const DS_LopTC &dsltc, const char MAMH[]);

// 4. Kiểm tra lớp học có sinh viên nào đang đăng ký tín chỉ không
int DemDangKyByLop(const DS_LopTC &dsltc, PTRSV FirstSV);
```

### 3.2. Tối ưu truyền tham chiếu `const DS_LopTC &dsltc` (Chống tràn Stack)
#### 🔍 Phân tích kỹ thuật:
- Kiểu `DS_LopTC` chứa mảng `LopTinChi* nodes[10000]`. Dung lượng bộ nhớ:
  $$\text{Size} = 10000 \times 8 \text{ bytes} + 4 \text{ bytes (int n)} \approx 80.004 \text{ bytes} \approx 80 \text{ KB}$$
- Nếu viết `bool SVDaCoDiem(DS_LopTC dsltc, ...)`: Mỗi lần hàm được gọi, hệ điều hành phải copy toàn bộ 80 KB vào Call Stack. Khi kiểm tra một lớp có 100 sinh viên, hệ thống phải copy $100 \times 80 \text{ KB} = 8 \text{ MB}$ vào ngăn xếp $\rightarrow$ Dễ gây chậm và nguy cơ **Tràn bộ nhớ ngăn xếp (Stack Overflow)**.
- Khi Thọ tối ưu sang `const DS_LopTC &dsltc`:
  - Dấu `&`: Truyền bằng địa chỉ tham chiếu, chỉ tốn **8 bytes** trên stack (kích thước con trỏ 64-bit).
  - Từ khóa `const`: Ngăn chặn tuyệt đối việc hàm vô tình sửa đổi dữ liệu danh sách lớp tín chỉ.

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Tại sao trong module ràng buộc, tất cả các hàm em đều truyền `const DS_LopTC &dsltc`?"*  
> **Thọ trả lời:** *"Thưa thầy, `DS_LopTC` chứa mảng 10.000 con trỏ, nặng hơn 80 KB. Nếu truyền tham trị, chương trình sẽ phải nhân bản 80 KB này vào Stack trong mỗi lần gọi hàm, gây lãng phí bộ nhớ và giảm hiệu năng. Em sử dụng `const &` để truyền tham chiếu chỉ tốn đúng 8 bytes địa chỉ, đồng thời từ khóa `const` bảo đảm an toàn dữ liệu chỉ đọc, tối ưu tốc độ kiểm tra ràng buộc lên mức tối đa."*

### 3.3. Bộ 5 kịch bản cấm xóa của hệ thống
1. **Môn học:** Đã có lớp tín chỉ tham chiếu (`DemLopTCByMAMH > 0`) $\rightarrow$ Cấm xóa.
2. **Sinh viên:** Đang đăng ký lớp tín chỉ (`DemDangKyByMASV > 0`) HOẶC đã có điểm thi (`SVDaCoDiem == true`) $\rightarrow$ Cấm xóa.
3. **Lớp sinh viên:** Lớp còn sinh viên (`FirstSV != nullptr`) HOẶC SV trong lớp đang đăng ký tín chỉ $\rightarrow$ Cấm xóa.
4. **Lớp tín chỉ:** Đã có sinh viên đăng ký (`DemTongSVDangKy > 0`) $\rightarrow$ Cấm xóa (yêu cầu dùng Hủy lớp).
5. **Đăng ký tín chỉ:** Sinh viên đã có điểm thi (`DIEM >= 0`) $\rightarrow$ Cấm hủy đăng ký.

---

## PHẦN 4: MODULE TIỆN ÍCH CHUẨN HÓA & VALIDATION (`utils.cpp`)
*Mã nguồn:* [`include/utils.h`](file:///d:/PTIT/DSA/include/utils.h) và [`src/utils.cpp`](file:///d:/PTIT/DSA/src/utils.cpp)

### 4.1. Chuẩn hóa tên (`ChuanHoaTen`)
- Cắt bỏ toàn bộ khoảng trắng ở đầu và cuối chuỗi.
- Giữa các từ chỉ giữ lại đúng 1 dấu cách.
- Chữ cái đầu mỗi từ viết hoa, các chữ cái sau viết thường (vd: `"  ngUYen   vAn   aN  "` $\rightarrow$ `"Nguyen Van An"`).

### 4.2. Chuẩn hóa mã (`ChuanHoaMa`)
- Xóa sạch toàn bộ khoảng trắng thừa.
- Chuyển toàn bộ ký tự sang chữ in hoa (vd: `" d24cqcn01 "` $\rightarrow$ `"D24CQCN01"`).

### 4.3. Nhập số nguyên an toàn (`NhapSoNguyen`)
```cpp
int NhapSoNguyen(const char thongBao[]) {
    char input[51];
    int val;
    while (true) {
        cout << thongBao;
        cin.getline(input, 51);
        ChuanHoaMa(input);
        if (KiemTraRong(input)) continue;
        
        // Kiểm tra từng ký tự: Chỉ cho phép chữ số (và dấu trừ ở đầu)
        bool hopLe = true;
        for (int i = 0; i < (int)strlen(input); i++) {
            if (i == 0 && input[i] == '-') continue;
            if (!isdigit((unsigned char)input[i])) { hopLe = false; break; }
        }
        if (!hopLe) { cout << "Loi: Vui long nhap so nguyen hop le!\n"; continue; }
        val = atoi(input);
        break;
    }
    return val;
}
```

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Tại sao em không dùng lệnh `cin >> x` thông thường để nhập số nguyên mà phải viết hàm `NhapSoNguyen`?"*  
> **Thọ trả lời:** *"Thưa thầy, lệnh `cin >> int` có một nhược điểm chí mạng trong C++: Nếu người dùng lỡ tay nhập vào một chữ cái (ví dụ gõ nhầm 'abc'), cờ lỗi `failbit` của stream `cin` sẽ bị bật, các lệnh nhập tiếp theo bị bỏ qua và chương trình sẽ rơi vào vòng lặp vô tận (Infinite Loop) làm treo ứng dụng.  
> Hàm `NhapSoNguyen` của em đọc toàn bộ dòng dưới dạng chuỗi qua `cin.getline`, sau đó duyệt kiểm tra từng ký tự bằng hàm `isdigit`. Nếu hợp lệ mới chuyển thành số qua `atoi`. Cách này chống crash và chống treo chương trình 100%."*

---


### 4.4. Tự động tách Họ - Tên & Đảm bảo Tên chỉ có 1 từ (`TachVaChuanHoaHoTen`)
*Mã nguồn:* [`src/utils.cpp`](file:///d:/PTIT/DSA/src/utils.cpp) và [`include/utils.h`](file:///d:/PTIT/DSA/include/utils.h)

```cpp
void TachVaChuanHoaHoTen(char ho[], char ten[], int maxHoLen, int maxTenLen) {
    if (ho == nullptr || ten == nullptr) return;
    
    // 1. Chuẩn hóa sơ bộ để loại bỏ khoảng trắng dư thừa
    ChuanHoaTen(ho);
    ChuanHoaTen(ten);
    
    int lenTen = strlen(ten);
    if (lenTen == 0) return;
    
    // 2. Tìm khoảng trắng cuối cùng trong chuỗi TEN
    int lastSpace = -1;
    for (int i = lenTen - 1; i >= 0; i--) {
        if (ten[i] == ' ') {
            lastSpace = i;
            break;
        }
    }
    
    // 3. Nếu TEN có chứa khoảng trắng (nhiều hơn 1 từ)
    // Ví dụ: Người dùng nhập HO = "Ngo", TEN = "Quang Tho"
    if (lastSpace != -1) {
        char phanDem[100];
        char tenCuoi[50];
        
        // Cắt phần đệm trước dấu cách cuối cùng ("Quang")
        int idxDem = 0;
        for (int i = 0; i < lastSpace; i++) {
            phanDem[idxDem++] = ten[i];
        }
        phanDem[idxDem] = ' ';
        
        // Cắt từ cuối cùng làm TÊN chính thức ("Tho")
        int idxTen = 0;
        for (int i = lastSpace + 1; i < lenTen; i++) {
            tenCuoi[idxTen++] = ten[i];
        }
        tenCuoi[idxTen] = ' ';
        
        // Đẩy phanDem sang ho: HO trở thành "Ngo Quang"
        int curLenHo = strlen(ho);
        if (curLenHo > 0 && curLenHo < maxHoLen - 1) {
            strncat(ho, " ", maxHoLen - 1 - curLenHo);
        }
        curLenHo = strlen(ho);
        if (curLenHo < maxHoLen - 1) {
            strncat(ho, phanDem, maxHoLen - 1 - curLenHo);
        }
        ho[maxHoLen - 1] = ' ';
        
        // Gán lại tên cuối vào ten: TEN trở thành "Tho"
        strncpy(ten, tenCuoi, maxTenLen - 1);
        ten[maxTenLen - 1] = ' ';
        
        // 4. Chuẩn hóa lại cả hai chuỗi
        ChuanHoaTen(ho);
        ChuanHoaTen(ten);
    }
}
```

#### 🔍 Phân tích chi tiết thuật toán:
- **Mục đích:** Theo quy định quản lý sinh viên của PTIT, danh sách sinh viên phải sắp xếp theo thứ tự `TEN` + `HO`. `TEN` chỉ được phép chứa duy nhất **1 từ** (tên gọi cuối cùng). Nếu người dùng quen tay nhập vào ô Tên cả chữ đệm (ví dụ `HO = "Ngô"`, `TEN = "Quang Thọ"` hoặc người dùng để trống ô Họ và nhập cả họ tên `"Ngô Quang Thọ"` vào ô Tên):
  - Thuật toán duyệt từ cuối chuỗi `ten` ngược về đầu để tìm vị trí dấu cách cuối cùng (`lastSpace`).
  - Toàn bộ chuỗi nằm bên phải `lastSpace` được trích xuất làm tên chính thức (`tenCuoi = "Thọ"`).
  - Toàn bộ chuỗi nằm bên trái `lastSpace` (`phanDem = "Quang"`) được tự động ghép nối vào sau chuỗi `ho` (biến `ho` thành `"Ngô Quang"`).
- **Kết quả:** `HO` = `"Ngô Quang"`, `TEN` = `"Thọ"`. Thuật toán đảm bảo `TEN` không bao giờ chứa dấu cách và luôn có đúng 1 từ, giúp hàm `SoSanhTen` và `InsertOrderSV` hoạt động chuẩn xác 100%.

---

### 4.5. Cơ chế Tự động Viết hoa Chữ cái đầu Real-Time (On-The-Fly)
*Mã nguồn:* [`src/utils.cpp`](file:///d:/PTIT/DSA/src/utils.cpp) và [`src/gui/TabLopSinhVien.cpp`](file:///d:/PTIT/DSA/src/gui/TabLopSinhVien.cpp)

#### 1. Trên giao diện Console (`NhapChuoiTuDongHoa`):
```cpp
void NhapChuoiTuDongHoa(char buffer[], int maxLen, bool vietHoaChuDau) {
    if (buffer == nullptr || maxLen <= 0) return;
#ifdef _WIN32
    if (!_isatty(_fileno(stdin))) { // Chống lỗi khi stdin bị pipe/redirect
        cin.getline(buffer, maxLen);
        if (vietHoaChuDau) ChuanHoaTen(buffer);
        return;
    }

    int idx = 0;
    while (idx < maxLen - 1) {
        int ch = _getch(); // Đọc từng phím nhấn ngay lập tức, không đợi Enter
        if (ch == 0 || ch == 224) { _getch(); continue; } // Phím mũi tên / Function key
        if (ch == '\r' || ch == '\n') { cout << "\n"; break; } // Nhấn Enter kết thúc
        if (ch == '\b' || ch == 127) { // Phím Backspace xóa ký tự
            if (idx > 0) { idx--; cout << "\b \b"; }
            continue;
        }
        if (isprint((unsigned char)ch)) {
            char c = (char)ch;
            // VIẾT HOA NGAY KHI GÕ: Nếu là ký tự đầu tiên hoặc ký tự đứng ngay sau dấu cách
            if (vietHoaChuDau && (idx == 0 || buffer[idx - 1] == ' ')) {
                c = (char)toupper((unsigned char)c);
            }
            buffer[idx++] = c;
            cout << c; // In ngay ký tự đã viết hoa ra màn hình Console
        }
    }
    buffer[idx] = '\0';
#else
    cin.getline(buffer, maxLen);
    if (vietHoaChuDau) ChuanHoaTen(buffer);
#endif
}
```

#### 2. Trên giao diện Đồ họa Qt GUI (`TabLopSinhVien.cpp`):
```cpp
// 1. Tự động viết hoa chữ cái đầu ngay trong khi đang gõ phím
auto setupRealtimeCap = [](QLineEdit *edt) {
    QObject::connect(edt, &QLineEdit::textEdited, [edt](const QString &text) {
        int cursor = edt->cursorPosition(); // Lưu lại vị trí con trỏ bàn phím
        QString res;
        res.reserve(text.length());
        bool capNext = true;
        for (int k = 0; k < text.length(); ++k) {
            QChar ch = text[k];
            if (ch.isSpace()) {
                capNext = true;
                res += ch;
            } else {
                if (capNext) {
                    res += ch.toUpper(); // Viết hoa ký tự đầu của từng từ
                    capNext = false;
                } else {
                    res += ch;
                }
            }
        }
        if (res != text) {
            edt->setText(res);
            edt->setCursorPosition(cursor); // Khôi phục lại đúng vị trí con trỏ
        }
    });
};
setupRealtimeCap(edtHo);
setupRealtimeCap(edtTen);

// 2. Tự động đẩy từ đệm về ô Họ ngay khi người dùng hoàn tất nhập ô Tên
auto autoSplitHoTen = [edtHo, edtTen]() {
    QString ho = edtHo->text().trimmed();
    QString ten = edtTen->text().trimmed();
    int lastSpace = ten.lastIndexOf(' ');
    if (lastSpace != -1) {
        QString dem = ten.left(lastSpace).trimmed();
        QString tenCuoi = ten.mid(lastSpace + 1).trimmed();
        edtHo->setText(ho.isEmpty() ? dem : ho + " " + dem);
        edtTen->setText(tenCuoi);
    }
};
connect(edtTen, &QLineEdit::editingFinished, autoSplitHoTen);
```

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Làm sao em hiện thực được việc người dùng vừa gõ phím là chữ cái đầu lập tức in hoa luôn chứ không phải đợi nhấn Enter mới thấy in hoa?"*  
> **Thọ trả lời:** *"Thưa thầy, hệ thống của em giải quyết trọn vẹn trên cả 2 môi trường:  
> 1. **Trên Console:** Em không dùng `cin.getline` thông thường (vì hàm này chặn chờ phím Enter mới bắt đầu xử lý). Em dùng hàm `_getch()` của thư viện `<conio.h>` để bắt tức thời từng mã phím người dùng gõ. Biến cờ kiểm tra nếu ký tự nằm ở đầu chuỗi (`idx == 0`) hoặc ngay sau dấu cách (`buffer[idx-1] == ' '`), em lập tức dùng `toupper` đổi thành chữ hoa rồi mới `cout` ra màn hình. Đồng thời xử lý Backspace bằng chuỗi điều khiển `\b \b` để xóa mượt mà.  
> 2. **Trên Qt GUI:** Em kết nối tín hiệu `QLineEdit::textEdited`. Mỗi khi một ký tự được gõ vào ô nhập liệu, hàm lambda duyệt chuỗi viết hoa chữ cái đầu và gán lại `setText`. Đặc biệt em lưu lại và khôi phục `cursorPosition` để con trỏ chuột không bị nhảy về cuối ô nhập liệu."*

> **Thầy hỏi:** *"Nếu người dùng nhập vào ô Tên là 'Quang Thọ' thì hệ thống xử lý thế nào để không làm hỏng thứ tự sắp xếp alphabet?"*  
> **Thọ trả lời:** *"Thưa thầy, đề bài yêu cầu sắp xếp theo Tên rồi đến Họ. Nếu ô Tên chứa cả chữ đệm 'Quang Thọ', khi so sánh nó sẽ bị so sánh theo vần Q thay vì vần T (Thọ), làm sai quy tắc nghiệp vụ.  
> Do đó, em xây dựng hàm `TachVaChuanHoaHoTen`. Khi người dùng gõ xong ô Tên hoặc bấm Thêm/Sửa, hàm quét tìm dấu cách cuối cùng trong ô Tên, tự động cắt chữ 'Quang' đẩy về nối tiếp vào ô Họ (thành 'Ngô Quang'), và chỉ giữ lại chữ 'Thọ' trong ô Tên. Nhờ vậy dữ liệu đưa vào danh sách liên kết luôn chuẩn và hàm `InsertOrderSV` luôn sắp đúng vị trí."*

---


---

## PHẦN 5: CƠ CHẾ GHI FILE AN TOÀN NGUYÊN TỬ (ATOMIC FILE I/O)
*Mã nguồn:* [`src/file_io.cpp`](file:///d:/PTIT/DSA/src/file_io.cpp)

```cpp
static bool HoanTatGhiAnToan(const char* filepath, const string& tempPath) {
    error_code ec;
    fs::path target(filepath);
    if (fs::exists(target, ec)) {
        fs::remove(target, ec); // 1. Xóa file cũ
    }
    fs::rename(tempPath, target, ec); // 2. Đổi tên file tạm (.tmp) thành file chính (.txt)
    if (ec) {
        fs::remove(tempPath, ec);
        return false;
    }
    return true;
}
```

#### 🎙️ KỊCH BẢN VẤN ĐÁP:
> **Thầy hỏi:** *"Trình bày cơ chế lưu trữ file của nhóm? Nếu đang lưu file mà máy tính bị mất điện đột ngột thì dữ liệu có bị mất trắng không?"*  
> **Thọ trả lời:** *"Thưa thầy, nhóm em áp dụng mô hình **Atomic File I/O Pattern** bằng thư viện `std::filesystem`:  
> Dữ liệu không bao giờ được ghi đè trực tiếp lên các file chính `data/*.txt`. Thay vào đó, toàn bộ dữ liệu từ bộ nhớ RAM được ghi ra một file tạm `data/*.txt.tmp`.  
> Chỉ khi toàn bộ quá trình ghi file tạm thành công và luồng stream được đóng an toàn (`outFile.close()`), hàm `HoanTatGhiAnToan` mới thực hiện lệnh `fs::rename` để tráo đổi tên file tạm thành file chính thức.  
> Do thao tác rename của hệ điều hành là thao tác nguyên tử (Atomic), nếu có xảy ra mất điện giữa chừng khi đang ghi, file tạm bị lỗi nhưng **file chính cũ vẫn còn nguyên vẹn**, hoàn toàn không bao giờ xảy ra hiện tượng file rỗng 0 bytes hay hỏng dữ liệu."*

---

## PHẦN 6: BỘ ĐỀ VẤN ĐÁP THỰC CHIẾN — HỎI & ĐÁP THEO TỪNG CÂU HỎI TRONG ĐỀ BÀI
*(Tập trung vào toàn bộ các câu hỏi thuộc phân hệ của Thọ)*

### 🎯 CÂU C: NHẬP LỚP (MÃ LỚP, TÊN LỚP)
- **Cấu trúc dữ liệu:** Mảng đặc `DS_LOPSV` gồm mảng tuyến tính tĩnh `LOPSV nodes[10000]` và số lượng `n`.
- **Câu hỏi của thầy:** *"Khi thêm một lớp mới, em xử lý kiểm tra trùng mã thế nào? Khi xóa một lớp, thao tác trong mảng đặc diễn ra ra sao?"*  
- **Thọ trả lời:**  
  1. **Khi thêm lớp:** Em gọi hàm `TimLop(dsLop, cMa)`. Hàm duyệt tuần tự từ $0$ đến $n-1$ và so sánh `stricmp(ds.nodes[i].MALOP, cMa) == 0`. Nếu tìm thấy trả về vị trí $ightarrow$ Báo lỗi trùng mã lớp và từ chối thêm.  
  2. **Khi xóa lớp:** Đầu tiên kiểm tra 2 ràng buộc: lớp phải không còn sinh viên (`FirstSV == nullptr`) và không có sinh viên nào của lớp đang có dữ liệu trong danh sách lớp tín chỉ (`DemDangKyByLop == 0`). Sau khi thỏa mãn, em tiến hành **dồn mảng đặc**: từ vị trí `idx` cần xóa, copy phần tử thứ `i+1` đè lên phần tử thứ `i` cho đến cuối mảng, sau đó giảm $n$ đi 1 (`ds.n--`).

---

### 🎯 CÂU D: NHẬP SINH VIÊN CỦA LỚP
- **Cấu trúc dữ liệu:** Danh sách liên kết đơn `PTRSV FirstSV` có thứ tự tăng dần theo `TEN` + `HO`.
- **Câu hỏi của thầy:** *"Mã sinh viên có được phép trùng giữa hai lớp khác nhau không? Và khi chèn sinh viên, thuật toán hoạt động như thế nào?"*  
- **Thọ trả lời:**  
  1. **Kiểm tra trùng toàn trường:** Đề bài quy định mỗi sinh viên là duy nhất trong toàn bộ học viện. Vì vậy em xây dựng hàm `TimSVToanTruong(dsLop, maSV)`. Hàm duyệt qua toàn bộ các lớp trong mảng `DS_LOPSV`, tại mỗi lớp duyệt danh sách liên kết sinh viên. Nếu trùng ở bất kỳ lớp nào trong trường đều bị từ chối thêm.  
  2. **Thuật toán chèn có thứ tự (`InsertOrderSV`):**  
     - Dùng hàm `SoSanhTen(a, b)` so sánh 3 tầng: Tầng 1 theo `TEN`, nếu trùng xét Tầng 2 theo `HO`, nếu trùng xét Tầng 3 theo `MASV`.  
     - Nếu danh sách rỗng hoặc sinh viên mới nhỏ hơn phần tử đầu tiên (`FirstSV`), chèn ngay vào đầu danh sách ($O(1)$).  
     - Ngược lại, duyệt tìm con trỏ `curr` sao cho `curr->next` có tên lớn hơn sinh viên mới (hoặc đến cuối danh sách), rồi thực hiện nối con trỏ: `p->next = curr->next; curr->next = p;`. Độ phức tạp trung bình $O(K)$ với $K$ là số sinh viên của lớp.

---

### 🎯 CÂU E: NHẬP MÔN HỌC (MÃ MH, TÊN MH, STCLT, STCTH)
- **Cấu trúc dữ liệu:** Cây nhị phân tìm kiếm (BST) `TreeMonHoc root`.
- **Câu hỏi của thầy:** *"Em hãy trình bày chi tiết thuật toán xóa một môn học trên Cây BST khi node cần xóa có 2 con?"*  
- **Thọ trả lời:**  
  *"Thưa thầy, khi node cần xóa có đủ 2 con, ta không thể xóa node trực tiếp vì sẽ làm đứt gãy 2 cây con. Thuật toán của em sử dụng kỹ thuật tìm **Node thế mạng (In-order Successor)**:  
  1. Ta tìm node nhỏ nhất bên cây con phải (tức là node nằm ở vị trí tận cùng bên trái của nhánh con phải `root->right`). Đây là node có giá trị khóa nhỏ nhất nhưng vẫn lớn hơn toàn bộ cây con trái.  
  2. Ta copy toàn bộ thông tin môn học của node thế mạng này đè vào node cần xóa.  
  3. Sau đó, ta tiến hành gọi hàm xóa đệ quy chính node thế mạng đó ra khỏi nhánh con phải (lúc này node thế mạng chỉ có tối đa 1 con phải hoặc là node lá, việc xóa trở về trường hợp đơn giản 0 hoặc 1 con)."*

---

### 🎯 CÂU F: IN DANH SÁCH MÔN HỌC TĂNG DẦN THEO TÊN
- **Câu hỏi của thầy:** *"Tại sao em không duyệt cây rồi in luôn mà phải dùng mảng tạm và QuickSort?"*  
- **Thọ trả lời:**  
  *"Thưa thầy, Cây BST môn học được xây dựng theo khóa `MAMH`. Do đó, nếu ta duyệt cây theo thứ tự LNR (In-Order), ta chỉ thu được danh sách tăng dần theo **Mã môn học**.  
  Yêu cầu câu f của đề bài là in danh sách môn học tăng dần theo **Tên môn học (`TENMH`)**.  
  Để đáp ứng mà không làm xáo trộn cây BST gốc, giải thuật tối ưu nhất là:  
  - Bước 1: Dùng In-Order duyệt cây để gom $N$ địa chỉ node vào một mảng con trỏ tạm `TreeMonHoc* arr = new TreeMonHoc[n]`. Thao tác này mất thời gian tuyến tính $O(N)$.  
  - Bước 2: Dùng thuật toán QuickSort sắp xếp mảng con trỏ này theo trường `TENMH`. Thao tác này mất $O(N \log N)$. Khi hoán vị ta chỉ tráo đổi địa chỉ con trỏ trong mảng tạm, hoàn toàn không sao chép dữ liệu struct và không thay đổi liên kết của cây.  
  - Bước 3: In danh sách đã sắp xếp ra màn hình và giải phóng mảng tạm bằng `delete[] arr`."*

---

### 🎯 CÂU L: IN DANH SÁCH SINH VIÊN CỦA 1 LỚP
- **Câu hỏi của thầy:** *"Khi in danh sách sinh viên của lớp, làm sao để in theo thứ tự tăng dần của Họ Tên, và làm sao để in theo Mã sinh viên?"*  
- **Thọ trả lời:**  
  *"Thưa thầy:  
  1. **In theo Họ Tên:** Vì danh sách liên kết của lớp đã được chèn theo thứ tự alphabet `TEN` + `HO` ngay từ lúc nhập bằng hàm `InsertOrderSV`, nên khi in ta chỉ cần duyệt tuần tự từ `FirstSV` đến `nullptr` là danh sách đã tự động tăng dần theo Họ Tên mà không cần phải tốn bất kỳ bước sắp xếp nào ($O(N)$).  
  2. **In theo Mã Sinh Viên:** Nếu người dùng chọn chế độ sắp xếp theo Mã SV, ta cấp phát một mảng con trỏ `PTRSV* arr = new PTRSV[soSV]`, đưa các node vào mảng và áp dụng thuật toán QuickSort so sánh theo `MASV`, sau đó in ra bảng."*

---

### 🎯 XỬ LÝ QUÊN LƯU FILE & ĐÓNG ỨNG DỤNG ĐỘT NGỘT
- **Câu hỏi của thầy:** *"Nếu người dùng đang làm việc mà quên bấm nút lưu file, hoặc bị cúp điện, hoặc nhấn nút X tắt ứng dụng thì hệ thống xử lý ra sao?"*  
- **Thọ trả lời:**  
  *"Thưa thầy, hệ thống được trang bị 2 tầng phòng ngự:  
  1. **Tự động lưu khi có thay đổi:** Trong Qt GUI, mỗi khi người dùng Thêm/Sửa/Xóa thành công một lớp, sinh viên hay môn học, hệ thống lập tức gọi `AppData::instance().save()` để ghi dữ liệu xuống đĩa ngay tức thì.  
  2. **Sự kiện đóng cửa sổ (`closeEvent`):** Trong cửa sổ chính `MainWindow`, em ghi đè phương thức `closeEvent`. Khi người dùng bấm nút [X], hệ thống sẽ tự động gọi lưu toàn bộ dữ liệu trước khi giải phóng bộ nhớ.  
  3. **Cơ chế ghi file nguyên tử (`Atomic File I/O`):** Mọi thao tác lưu đều ghi ra file tạm `.tmp` trước khi đổi tên đè lên file chính, bảo đảm nếu đang ghi mà mất điện thì file cũ vẫn nguyên vẹn 100%, không bao giờ bị mất trắng."*

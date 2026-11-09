#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#include <io.h>
#endif

using namespace std;

void ChuanHoaTen(char str[]) {
    int n = strlen(str);
    if (n == 0) return;
    
    int i = 0;
    while (i < n && isspace((unsigned char)str[i])) i++;
    
    int j = 0;
    bool spaceFound = false;
    
    while (i < n) {
        if (!isspace((unsigned char)str[i])) {
            if (spaceFound && j > 0) {
                str[j++] = ' ';
            }
            if (j == 0 || str[j - 1] == ' ') {
                str[j++] = toupper((unsigned char)str[i]);
            } else {
                str[j++] = tolower((unsigned char)str[i]);
            }
            spaceFound = false;
        } else {
            spaceFound = true;
        }
        i++;
    }
    
    str[j] = '\0';
}

void ChuanHoaMa(char str[]) {
    int n = strlen(str);
    if (n == 0) return;
    
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = toupper((unsigned char)str[i]);
        }
    }
    str[j] = '\0';
}

bool KiemTraRong(const char str[]) {
    if (str == nullptr) return true;
    int n = strlen(str);
    if (n == 0) return true;
    for (int i = 0; i < n; i++) {
        if (!isspace((unsigned char)str[i])) return false;
    }
    return true;
}

int NhapSoNguyen(const char thongBao[]) {
    char input[51];
    int val;
    while (true) {
        cout << thongBao;
        cin.getline(input, 51);
        ChuanHoaMa(input); // xóa khoảng trắng
        if (KiemTraRong(input)) {
            cout << "Loi: Khong duoc de trong!\n";
            continue;
        }
        // Kiểm tra từng ký tự: chỉ cho phép số và dấu trừ ở đầu
        bool hopLe = true;
        for (int i = 0; i < (int)strlen(input); i++) {
            if (i == 0 && input[i] == '-') continue;
            if (!isdigit((unsigned char)input[i])) {
                hopLe = false;
                break;
            }
        }
        if (!hopLe || strlen(input) == 0 || (strlen(input) == 1 && input[0] == '-')) {
            cout << "Loi: Vui long nhap so nguyen hop le!\n";
            continue;
        }
        val = atoi(input);
        break;
    }
    return val;
}

bool KiemTraNienKhoaHopLe(const char nk[]) {
    if (nk == nullptr) return false;
    if (strlen(nk) != 9) return false;
    if (nk[4] != '-') return false;
    
    for (int i = 0; i < 9; i++) {
        if (i == 4) continue;
        if (!isdigit((unsigned char)nk[i])) return false;
    }
    
    int y1 = (nk[0] - '0') * 1000 + (nk[1] - '0') * 100 + (nk[2] - '0') * 10 + (nk[3] - '0');
    int y2 = (nk[5] - '0') * 1000 + (nk[6] - '0') * 100 + (nk[7] - '0') * 10 + (nk[8] - '0');
    
    if (y1 < 1900 || y1 > 2100) return false;
    if (y2 != y1 + 1) return false;
    
    return true;
}

void TachVaChuanHoaHoTen(char ho[], char ten[], int maxHoLen, int maxTenLen) {
    if (ho == nullptr || ten == nullptr) return;
    
    // 1. Chuẩn hóa sơ bộ để loại bỏ khoảng trắng dư thừa
    ChuanHoaTen(ho);
    ChuanHoaTen(ten);
    
    int lenTen = strlen(ten);
    // Trường hợp: TÊN rỗng nhưng HỌ có nhiều hơn 1 từ (người dùng nhập cả họ và tên vào ô HỌ)
    if (lenTen == 0) {
        int lenHo = strlen(ho);
        if (lenHo > 0) {
            int lastSpaceHo = -1;
            for (int i = lenHo - 1; i >= 0; i--) {
                if (ho[i] == ' ') {
                    lastSpaceHo = i;
                    break;
                }
            }
            if (lastSpaceHo != -1) {
                strncpy(ten, ho + lastSpaceHo + 1, maxTenLen - 1);
                ten[maxTenLen - 1] = '\0';
                ho[lastSpaceHo] = '\0';
                ChuanHoaTen(ho);
                ChuanHoaTen(ten);
            }
        }
        return;
    }
    
    // 2. Tìm khoảng trắng cuối cùng trong chuỗi TEN
    int lastSpace = -1;
    for (int i = lenTen - 1; i >= 0; i--) {
        if (ten[i] == ' ') {
            lastSpace = i;
            break;
        }
    }
    
    // 3. Nếu TEN có chứa khoảng trắng (nhiều hơn 1 từ)
    // Ví dụ: Nhập TEN = "Văn An" -> Tách "Văn" ghép vào HO, giữ "An" cho TEN
    // Ví dụ: Nhập HỌ = "", TEN = "Nguyễn Văn An" -> "Nguyễn Văn" ghép vào HỌ, giữ "An" cho TEN
    if (lastSpace != -1) {
        char phanDem[128];
        char tenCuoi[64];
        
        int idxDem = 0;
        for (int i = 0; i < lastSpace && idxDem < (int)sizeof(phanDem) - 1; i++) {
            phanDem[idxDem++] = ten[i];
        }
        phanDem[idxDem] = '\0';
        
        int idxTen = 0;
        for (int i = lastSpace + 1; i < lenTen && idxTen < (int)sizeof(tenCuoi) - 1; i++) {
            tenCuoi[idxTen++] = ten[i];
        }
        tenCuoi[idxTen] = '\0';
        
        // Đẩy phanDem sang ho (nếu ho đã có nội dung thì thêm dấu cách ngăn cách)
        int curLenHo = strlen(ho);
        if (curLenHo > 0 && curLenHo < maxHoLen - 1) {
            strncat(ho, " ", maxHoLen - 1 - curLenHo);
        }
        curLenHo = strlen(ho);
        if (curLenHo < maxHoLen - 1) {
            strncat(ho, phanDem, maxHoLen - 1 - curLenHo);
        }
        ho[maxHoLen - 1] = '\0';
        
        // Gán lại tên cuối vào ten
        strncpy(ten, tenCuoi, maxTenLen - 1);
        ten[maxTenLen - 1] = '\0';
        
        // 4. Chuẩn hóa lại cả hai để xóa khoảng trắng thừa và viết hoa chuẩn mực
        ChuanHoaTen(ho);
        ChuanHoaTen(ten);
    }
}

void NhapChuoiTuDongHoa(char buffer[], int maxLen, bool vietHoaChuDau) {
    if (buffer == nullptr || maxLen <= 0) return;

#ifdef _WIN32
    // Nếu stdin bị redirect (chạy test pipe tự động), fallback sang cin.getline
    if (!_isatty(_fileno(stdin))) {
        cin.getline(buffer, maxLen);
        if (vietHoaChuDau) ChuanHoaTen(buffer);
        return;
    }

    int idx = 0;
    while (idx < maxLen - 1) {
        int ch = _getch();
        if (ch == 0 || ch == 224) { // Phím chức năng / mũi tên (2 byte)
            _getch(); // bỏ qua byte thứ hai
            continue;
        }
        
        // Nhấn Enter để kết thúc nhập
        if (ch == '\r' || ch == '\n') {
            cout << "\n";
            break;
        }
        
        // Nhấn Backspace để xóa ký tự
        if (ch == '\b' || ch == 127) {
            if (idx > 0) {
                idx--;
                cout << "\b \b";
            }
            continue;
        }
        
        // Ký tự thông thường có thể in được
        if (isprint((unsigned char)ch)) {
            char c = (char)ch;
            if (vietHoaChuDau) {
                // Viết hoa chữ cái đầu tiên hoặc chữ cái ngay sau dấu cách ngay lúc gõ (Real-time)
                if (idx == 0 || buffer[idx - 1] == ' ') {
                    c = (char)toupper((unsigned char)c);
                }
            }
            buffer[idx++] = c;
            cout << c;
        }
    }
    buffer[idx] = '\0';
#else
    cin.getline(buffer, maxLen);
    if (vietHoaChuDau) ChuanHoaTen(buffer);
#endif
}

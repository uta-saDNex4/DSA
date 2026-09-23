#include "utils.h"
#include <string.h>
#include <ctype.h>

void ChuanHoaTen(char str[]) {
    int n = strlen(str);
    if (n == 0) return;
    
    // Xóa khoảng trắng ở đầu
    int i = 0;
    while (str[i] == ' ') i++;
    
    int j = 0;
    bool spaceFound = false;
    
    // Xóa khoảng trắng thừa ở giữa và copy đè lên
    while (i < n) {
        if (str[i] != ' ') {
            if (spaceFound && j > 0) {
                str[j++] = ' ';
            }
            // Chuyển ký tự đầu tiên của mỗi từ thành chữ hoa, các ký tự khác thành chữ thường
            if (j == 0 || str[j - 1] == ' ') {
                str[j++] = toupper(str[i]);
            } else {
                str[j++] = tolower(str[i]);
            }
            spaceFound = false;
        } else {
            spaceFound = true;
        }
        i++;
    }
    
    str[j] = '\0'; // Kết thúc chuỗi
}

void ChuanHoaMa(char str[]) {
    int n = strlen(str);
    if (n == 0) return;
    
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (str[i] != ' ') {
            str[j++] = toupper(str[i]); // Chuyển thành chữ hoa và bỏ qua mọi khoảng trắng
        }
    }
    str[j] = '\0';
}

bool KiemTraRong(const char str[]) {
    if (str == nullptr || strlen(str) == 0) return true;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != ' ') return false;
    }
    return true; // Chỉ toàn khoảng trắng
}

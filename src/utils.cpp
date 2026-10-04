#include "utils.h"
#include <string.h>
#include <ctype.h>
#include <iostream>
#include <stdlib.h>

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

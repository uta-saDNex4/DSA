#include "sinhvien.h"
#include <string.h>

void KhoiTaoDanhSachSV(PTRSV &First){
    First = nullptr;
}


PTRSV TimSV(PTRSV First, char MASV[]){
    if (First == nullptr) return nullptr;

    PTRSV p;
    for (p = First; p != nullptr; p = p -> next){
        // if (p -> sv.MASV == MASV) return p;
        if (strcmp(p -> sv.MASV ,MASV) == 0) return p;
    }
    return nullptr;
        
}
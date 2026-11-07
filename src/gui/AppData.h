#ifndef APP_DATA_H
#define APP_DATA_H

#include "struct.h"
#include "monhoc.h"
#include "lop.h"
#include "sinhvien.h"
#include "loptc.h"
#include "dangky.h"
#include "file_io.h"
#include "diem.h"
#include "rang_buoc.h"
#include "utils.h"

class AppData {
public:
    static AppData& instance() {
        static AppData inst;
        return inst;
    }

    DS_MonHoc dsMH;
    DS_LOPSV dsLop;
    DS_LopTC dsLTC;

    void init() {
        KhoiTaoCayMonHoc(dsMH.root);
        KhoiTaoDanhSachLop(dsLop);
        KhoiTaoDS_LopTC(dsLTC);
        load();
    }

    bool save() {
        return LuuToanBoDuLieu(dsMH.root, dsLop, dsLTC);
    }

    bool saveAll() {
        return save();
    }

    bool load() {
        return DocToanBoDuLieu(dsMH, dsLop, dsLTC);
    }

    bool loadAll() {
        return load();
    }

private:
    AppData() = default;
    ~AppData() {
        save();
        GiaiPhongCay(dsMH.root);
        GiaiPhongDSLop(dsLop);
        GiaiPhongDS_LopTC(dsLTC);
    }
};

#endif // APP_DATA_H

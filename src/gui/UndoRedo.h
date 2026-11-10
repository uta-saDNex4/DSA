#ifndef UNDO_REDO_H
#define UNDO_REDO_H

#include "struct.h"
#include "monhoc.h"
#include "sinhvien.h"
#include "lop.h"
#include "AppData.h"
#include <QString>
#include <cstring>

enum ActionType {
    ACTION_THEM_MON_HOC,
    ACTION_XOA_MON_HOC,
    ACTION_SUA_MON_HOC,
    ACTION_THEM_SINH_VIEN,
    ACTION_XOA_SINH_VIEN,
    ACTION_SUA_SINH_VIEN
};

struct UndoAction {
    ActionType type;
    char description[128]; // Mô tả hiển thị (VD: "Thêm môn CTDL")

    // Dữ liệu cho Môn học
    MonHoc mhData;
    MonHoc mhOldData;

    // Dữ liệu cho Sinh viên & Lớp
    char maLop[16];
    SinhVien svData;
    SinhVien svOldData;
};

// Node Ngăn xếp (Stack DSLK đơn tự cài đặt theo Rule 1 & 7 của môn CTDL & GT)
struct StackNode {
    UndoAction action;
    StackNode* next;
};

class UndoRedoManager {
private:
    StackNode* undoTop;
    StackNode* redoTop;
    int maxHistory;
    int currentUndoCount;

    void clearStack(StackNode*& top) {
        while (top != nullptr) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
    }

    UndoRedoManager() : undoTop(nullptr), redoTop(nullptr), maxHistory(50), currentUndoCount(0) {}

public:
    static UndoRedoManager& instance() {
        static UndoRedoManager s_instance;
        return s_instance;
    }

    ~UndoRedoManager() {
        clearStack(undoTop);
        clearStack(redoTop);
    }

    bool canUndo() const { return undoTop != nullptr; }
    bool canRedo() const { return redoTop != nullptr; }

    void recordAction(const UndoAction& act) {
        // Mỗi khi có thao tác mới, xóa rỗng RedoStack
        clearStack(redoTop);

        StackNode* newNode = new StackNode;
        newNode->action = act;
        newNode->next = undoTop;
        undoTop = newNode;
        currentUndoCount++;

        // Giới hạn 50 bước gần nhất để giải phóng bộ nhớ
        if (currentUndoCount > maxHistory) {
            StackNode* curr = undoTop;
            while (curr->next != nullptr && curr->next->next != nullptr) {
                curr = curr->next;
            }
            if (curr->next != nullptr) {
                delete curr->next;
                curr->next = nullptr;
                currentUndoCount--;
            }
        }
    }

    bool undo(QString& outDesc) {
        if (undoTop == nullptr) return false;

        StackNode* node = undoTop;
        undoTop = undoTop->next;
        UndoAction act = node->action;
        delete node;
        currentUndoCount--;

        DS_MonHoc& dsMH = AppData::instance().dsMH;
        DS_LOPSV& dsLop = AppData::instance().dsLop;

        switch (act.type) {
            case ACTION_THEM_MON_HOC:
                // Nghịch đảo của Thêm là Xóa
                XoaMonHocNoiB(dsMH.root, act.mhData.MAMH, dsMH.n);
                break;
            case ACTION_XOA_MON_HOC:
                // Nghịch đảo của Xóa là Thêm lại
                ThemMonHoc(dsMH.root, act.mhOldData, dsMH.n);
                break;
            case ACTION_SUA_MON_HOC: {
                // Nghịch đảo của Sửa là khôi phục thông tin cũ
                TreeMonHoc p = TimMonHoc(dsMH.root, act.mhData.MAMH);
                if (p != nullptr) {
                    strcpy(p->mh.TENMH, act.mhOldData.TENMH);
                    p->mh.STCLT = act.mhOldData.STCLT;
                    p->mh.STCTH = act.mhOldData.STCTH;
                }
                break;
            }
            case ACTION_THEM_SINH_VIEN: {
                // Nghịch đảo của Thêm là Xóa
                int idxLop = TimLop(dsLop, act.maLop);
                if (idxLop >= 0) {
                    XoaSVNoiB(dsLop.nodes[idxLop].FirstSV, act.svData.MASV);
                }
                break;
            }
            case ACTION_XOA_SINH_VIEN: {
                // Nghịch đảo của Xóa là Chèn lại
                int idxLop = TimLop(dsLop, act.maLop);
                if (idxLop >= 0) {
                    InsertOrderSV(dsLop.nodes[idxLop].FirstSV, act.svOldData);
                }
                break;
            }
            case ACTION_SUA_SINH_VIEN: {
                // Nghịch đảo của Sửa là khôi phục thông tin cũ
                int idxLop = TimLop(dsLop, act.maLop);
                if (idxLop >= 0) {
                    for (PTRSV p = dsLop.nodes[idxLop].FirstSV; p != nullptr; p = p->next) {
                        if (strcmp(p->sv.MASV, act.svData.MASV) == 0) {
                            p->sv = act.svOldData;
                            break;
                        }
                    }
                }
                break;
            }
        }

        // Đưa vào RedoStack
        StackNode* rNode = new StackNode;
        rNode->action = act;
        rNode->next = redoTop;
        redoTop = rNode;

        AppData::instance().save();
        outDesc = QString::fromUtf8(act.description);
        return true;
    }

    bool redo(QString& outDesc) {
        if (redoTop == nullptr) return false;

        StackNode* node = redoTop;
        redoTop = redoTop->next;
        UndoAction act = node->action;
        delete node;

        DS_MonHoc& dsMH = AppData::instance().dsMH;
        DS_LOPSV& dsLop = AppData::instance().dsLop;

        switch (act.type) {
            case ACTION_THEM_MON_HOC:
                // Làm lại: Thêm môn
                ThemMonHoc(dsMH.root, act.mhData, dsMH.n);
                break;
            case ACTION_XOA_MON_HOC:
                // Làm lại: Xóa môn
                XoaMonHocNoiB(dsMH.root, act.mhOldData.MAMH, dsMH.n);
                break;
            case ACTION_SUA_MON_HOC: {
                // Làm lại: Áp dụng thông tin mới
                TreeMonHoc p = TimMonHoc(dsMH.root, act.mhData.MAMH);
                if (p != nullptr) {
                    strcpy(p->mh.TENMH, act.mhData.TENMH);
                    p->mh.STCLT = act.mhData.STCLT;
                    p->mh.STCTH = act.mhData.STCTH;
                }
                break;
            }
            case ACTION_THEM_SINH_VIEN: {
                // Làm lại: Thêm SV
                int idxLop = TimLop(dsLop, act.maLop);
                if (idxLop >= 0) {
                    InsertOrderSV(dsLop.nodes[idxLop].FirstSV, act.svData);
                }
                break;
            }
            case ACTION_XOA_SINH_VIEN: {
                // Làm lại: Xóa SV
                int idxLop = TimLop(dsLop, act.maLop);
                if (idxLop >= 0) {
                    XoaSVNoiB(dsLop.nodes[idxLop].FirstSV, act.svOldData.MASV);
                }
                break;
            }
            case ACTION_SUA_SINH_VIEN: {
                // Làm lại: Áp dụng thông tin SV mới
                int idxLop = TimLop(dsLop, act.maLop);
                if (idxLop >= 0) {
                    for (PTRSV p = dsLop.nodes[idxLop].FirstSV; p != nullptr; p = p->next) {
                        if (strcmp(p->sv.MASV, act.svData.MASV) == 0) {
                            p->sv = act.svData;
                            break;
                        }
                    }
                }
                break;
            }
        }

        // Đưa lại vào UndoStack
        StackNode* uNode = new StackNode;
        uNode->action = act;
        uNode->next = undoTop;
        undoTop = uNode;
        currentUndoCount++;

        AppData::instance().save();
        outDesc = QString::fromUtf8(act.description);
        return true;
    }
};

#endif // UNDO_REDO_H

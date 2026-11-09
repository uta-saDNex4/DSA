#ifndef TAB_DIEM_H
#define TAB_DIEM_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "AppData.h"
#include "diem.h"

class TabDiem : public QWidget {
    Q_OBJECT
public:
    explicit TabDiem(QWidget *parent = nullptr);
    void loadData();

private slots:
    // Sub-tab 1: Nhập điểm (Câu i)
    void onLuuDiemThi();
    void onTaiDanhSachNhapDiem();
    void onMHNhapChanged();
    void onNKNhapChanged();
    void onHKNhapChanged();

    // Sub-tab 2: Bảng điểm môn học (Câu j)
    void onXemBangDiemMonHoc();
    void onMHBdChanged();
    void onNKBdChanged();
    void onHKBdChanged();

    // Sub-tab 3: Điểm trung bình khóa học (Câu k)
    void onXemDiemTrungBinhKhoaHoc();

    // Sub-tab 4: Bảng điểm tổng kết ma trận (Câu l)
    void onXemBangDiemTongKet();

private:
    QTabWidget *subTabs;

    // --- Sub-tab 1: Nhập điểm ---
    QComboBox *cboMH_Nhap;
    QComboBox *cboNK_Nhap;
    QComboBox *cboHK_Nhap;
    QComboBox *cboNhom_Nhap;
    QTableWidget *tblNhapDiem;
    QLabel *lblInfoLTC_Nhap;
    int currentMaLTC_Nhap;

    // --- Sub-tab 2: Bảng điểm môn học ---
    QComboBox *cboMH_BD;
    QComboBox *cboNK_BD;
    QComboBox *cboHK_BD;
    QComboBox *cboNhom_BD;
    QTableWidget *tblBangDiemMH;
    QLabel *lblInfo_BD;

    // --- Sub-tab 3: ĐTB Khóa học ---
    QComboBox *cboLop_DTB;
    QTableWidget *tblDTB;
    QLabel *lblInfo_DTB;

    // --- Sub-tab 4: Tổng kết ma trận ---
    QComboBox *cboLop_TK;
    QTableWidget *tblTongKet;
    QLabel *lblInfo_TK;

    void setupUI();
    QWidget* createSubTabNhapDiem();
    QWidget* createSubTabBangDiemMH();
    QWidget* createSubTabDTBKhoaHoc();
    QWidget* createSubTabTongKet();

    void capNhatComboMonHoc(QComboBox *cbo);
    void capNhatComboLop(QComboBox *cbo);
};

#endif // TAB_DIEM_H

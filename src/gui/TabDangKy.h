#ifndef TAB_DANG_KY_H
#define TAB_DANG_KY_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QGroupBox>
#include "AppData.h"

class TabDangKy : public QWidget {
    Q_OBJECT
public:
    explicit TabDangKy(QWidget *parent = nullptr);
    void loadData();

private slots:
    void onSearchSinhVien();
    void onFilterChanged();
    void onDangKyLTC();
    void onHuyDangKyLTC();

private:
    QLineEdit *txtMaSV;
    QPushButton *btnTimSV;
    QLabel *lblThongTinSV;

    QComboBox *cboNienKhoa;
    QComboBox *cboHocKy;

    QTableWidget *tableLopMo;
    QTableWidget *tableLopDaDK;

    QLabel *lblTongTinChi;

    char currentMaSV[16];
    bool svFound;

    void setupUI();
    void capNhatDanhSachLopMo();
    void capNhatDanhSachDaDangKy();
    void capNhatDanhSachNienKhoa();
};

#endif // TAB_DANG_KY_H
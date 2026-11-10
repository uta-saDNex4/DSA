#ifndef TAB_DANG_KY_H
#define TAB_DANG_KY_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QGroupBox>
#include <QStackedWidget>
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
    void onSVSelectedFromTable(int row, int col);
    void onChonSVKhac();
    void onFilterSVChanged(const QString &text);

private:
    QLineEdit *txtMaSV;
    QPushButton *btnTimSV;
    QLabel *lblThongTinSV;

    QComboBox *cboNienKhoa;
    QComboBox *cboHocKy;

    QTableWidget *tableLopMo;

    // Bottom Stack: Page 0 = Chọn SV, Page 1 = Môn đã ĐK
    QStackedWidget *stackBottom;
    QWidget *pnlChonSV;
    QWidget *pnlDaDK;

    // Components of Page 0 (Chọn SV)
    QTableWidget *tableChonSV;
    QLineEdit *txtFilterSV;
    QPushButton *btnChonSVSubmit;

    // Components of Page 1 (Môn đã ĐK)
    QTableWidget *tableLopDaDK;
    QLabel *lblTongTinChi;
    QPushButton *btnChonSVKhac;
    QPushButton *btnHuyDK;

    char currentMaSV[16];
    bool svFound;

    void setupUI();
    void capNhatDanhSachLopMo();
    void capNhatDanhSachDaDangKy();
    void capNhatDanhSachNienKhoa();
    void capNhatDanhSachSinhVienChon();
};

#endif // TAB_DANG_KY_H

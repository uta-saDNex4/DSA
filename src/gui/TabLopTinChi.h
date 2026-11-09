#ifndef TAB_LOP_TIN_CHI_H
#define TAB_LOP_TIN_CHI_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include "AppData.h"

class TabLopTinChi : public QWidget {
    Q_OBJECT
public:
    explicit TabLopTinChi(QWidget *parent = nullptr);
    void loadData();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void onFilterChanged();
    void onMoLopTC();
    void onHieuChinhLopTC();
    void onXoaLopTC();
    void onHuyLopThieuSiSo();
    void onXemDSSVDangKy();

private:
    QComboBox *cboNienKhoa;
    QComboBox *cboHocKy;
    QTableWidget *tableLTC;
    QLabel *statusLabel;

    void setupUI();
    void capNhatDanhSachNienKhoa();
};

#endif // TAB_LOP_TIN_CHI_H

#ifndef TAB_LOP_SINH_VIEN_H
#define TAB_LOP_SINH_VIEN_H

#include <QWidget>
#include <QListWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QRadioButton>
#include "AppData.h"

class TabLopSinhVien : public QWidget {
    Q_OBJECT
public:
    explicit TabLopSinhVien(QWidget *parent = nullptr);
    void loadData();

private slots:
    void onClassSelected(int row);
    void onThemLop();
    void onSuaLop();
    void onXoaLop();
    void onThemSV();
    void onSuaSV();
    void onXoaSV();
    void onSearchClass(const QString &text);
    void onSearchSV(const QString &text);
    void onSortModeChanged();

private:
    QListWidget *listLop;
    QLineEdit *searchLopEdit;
    QLabel *classHeaderLabel;
    QRadioButton *radSortTenHo;
    QRadioButton *radSortMaSV;
    QLineEdit *searchSVEdit;
    QTableWidget *tableSV;
    QLabel *statusSVLabel;

    int currentClassIdx = -1;

    void setupUI();
    void loadSinhVienCuaLop(int classIdx);
};

#endif // TAB_LOP_SINH_VIEN_H

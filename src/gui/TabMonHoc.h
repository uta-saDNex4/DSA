#ifndef TAB_MON_HOC_H
#define TAB_MON_HOC_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "AppData.h"

class TabMonHoc : public QWidget {
    Q_OBJECT
public:
    explicit TabMonHoc(QWidget *parent = nullptr);
    void loadData();

private slots:
    void onSearchChanged(const QString &text);
    void onThemMonHoc();
    void onSuaMonHoc();
    void onXoaMonHoc();

private:
    QLineEdit *searchEdit;
    QTableWidget *table;
    QLabel *statusLabel;

    void setupUI();
    void hienThiDanhSach(TreeMonHoc* arr, int count, const QString &keyword = "");
};

#endif // TAB_MON_HOC_H

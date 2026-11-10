#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>
#include "TabMonHoc.h"
#include "TabLopSinhVien.h"
#include "TabLopTinChi.h"
#include "TabDangKy.h"
#include "TabDiem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onSaveAllData();
    void onReloadData();
    void onAboutDialog();
    void onTabChanged(int index);
    void onUndo();
    void onRedo();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QTabWidget *tabWidget;
    TabMonHoc *tabMonHoc;
    TabLopSinhVien *tabLopSinhVien;
    TabLopTinChi *tabLopTinChi;
    TabDangKy *tabDangKy;
    TabDiem *tabDiem;

    QLabel *statusMsgLabel;
    QLabel *statusStatsLabel;

    void setupUI();
    void setupShortcuts();
    void applyTheme();
    void updateStatusBarStats();
};

#endif // MAIN_WINDOW_H
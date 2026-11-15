#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLabel>
#include <QStackedWidget>
#include "TabMonHoc.h"
#include "TabLopSinhVien.h"
#include "TabLopTinChi.h"
#include "TabDangKy.h"
#include "TabDiem.h"
#include "LoginView.h"
#include "StudentDashboardView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLoginAsStudent(const QString &maSV, const QString &hoTen, const QString &maLop, const QString &phai, const QString &soDT);
    void onLoginAsTeacher();
    void onLogout();
    void onSaveAllData();
    void onReloadData();
    void onAboutDialog();
    void onTabChanged(int index);
    void onUndo();
    void onRedo();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QStackedWidget *stackedWidget;

    // View 0: Login View
    LoginView *loginView;

    // View 1: Teacher / Admin Dashboard
    QWidget *teacherContainer;
    QTabWidget *tabWidget;
    TabMonHoc *tabMonHoc;
    TabLopSinhVien *tabLopSinhVien;
    TabLopTinChi *tabLopTinChi;
    TabDangKy *tabDangKy;
    TabDiem *tabDiem;

    // View 2: Student Dashboard
    StudentDashboardView *studentDashboardView;

    QLabel *statusMsgLabel;
    QLabel *statusStatsLabel;

    void setupUI();
    void setupTeacherUI();
    void setupShortcuts();
    void applyTheme();
    void updateStatusBarStats();
};

#endif // MAIN_WINDOW_H

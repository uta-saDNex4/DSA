#ifndef STUDENT_DASHBOARD_VIEW_H
#define STUDENT_DASHBOARD_VIEW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTabWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QString>
#include "TabDangKy.h"

class StudentDashboardView : public QWidget {
    Q_OBJECT
public:
    explicit StudentDashboardView(QWidget *parent = nullptr);
    void setStudentInfo(const QString &maSV, const QString &hoTen, const QString &maLop, const QString &phai, const QString &soDT);

signals:
    void logoutRequested();

private slots:
    void onTabChanged(int index);
    void onSearchMonHocChanged(const QString &text);

private:
    QString m_maSV;
    QString m_hoTen;
    QString m_maLop;
    QString m_phai;
    QString m_soDT;

    // Header Profile Card (To & Rõ Nét)
    QLabel *lblAvatar;
    QLabel *lblProfileName;
    QLabel *lblBadgeMaSV;
    QLabel *lblBadgeLop;
    QLabel *lblBadgePhai;
    QLabel *lblBadgeSDT;
    QPushButton *btnLogout;

    // Tab Widget
    QTabWidget *tabWidget;
    TabDangKy *tabDangKy;

    // Tab Điểm Cá Nhân
    QWidget *tabDiemCaNhan;
    QTableWidget *tblDiem;
    QLabel *lblSummaryStats;

    // Tab Tra Cứu Môn Học
    QWidget *tabTraCuuMH;
    QLineEdit *edtSearchMH;
    QTableWidget *tblMonHoc;

    void setupUI();
    void applyTheme();
    void loadBangDiemCaNhan();
    void loadDanhSachMonHoc();
};

#endif // STUDENT_DASHBOARD_VIEW_H

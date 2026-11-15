#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QString>

class LoginView : public QWidget {
    Q_OBJECT
public:
    explicit LoginView(QWidget *parent = nullptr);
    void loadData();

signals:
    void loginAsStudent(const QString &maSV, const QString &hoTen, const QString &maLop, const QString &phai, const QString &soDT);
    void loginAsTeacher();

private slots:
    void onStudentTableRowClicked(int row, int col);
    void onBtnLoginStudentClicked();
    void onSearchStudentChanged(const QString &text);

private:
    QLineEdit *edtUsername;
    QLineEdit *edtPassword;
    QPushButton *btnLoginStudent;
    QPushButton *btnRoleTeacher;

    QLineEdit *edtSearch;
    QTableWidget *tblStudents;
    QLabel *lblStatus;

    void setupUI();
    void applyTheme();
};

#endif // LOGIN_VIEW_H

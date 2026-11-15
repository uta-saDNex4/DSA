#include "LoginView.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <cstring>
#include "AppData.h"

LoginView::LoginView(QWidget *parent) : QWidget(parent) {
    setupUI();
    applyTheme();
    loadData();
}

void LoginView::setupUI() {
    QHBoxLayout *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(32, 28, 32, 28);
    rootLayout->setSpacing(28);

    // ==========================================
    // CỘT TRÁI: FORM ĐĂNG NHẬP & ROLE GIÁO VIÊN
    // ==========================================
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(16);

    // School Header Box
    QLabel *lblSchool = new QLabel("HỌC VIỆN CÔNG NGHỆ BƯU CHÍNH VIỄN THÔNG", leftPanel);
    lblSchool->setObjectName("lblLoginSchool");
    QLabel *lblAppTitle = new QLabel("HỆ THỐNG QUẢN LÝ ĐIỂM TÍN CHỈ (DSA)", leftPanel);
    lblAppTitle->setObjectName("lblLoginTitle");
    QLabel *lblAppSub = new QLabel("Cổng thông tin đào tạo và đăng ký học phần", leftPanel);
    lblAppSub->setObjectName("lblLoginSub");

    leftLayout->addWidget(lblSchool);
    leftLayout->addWidget(lblAppTitle);
    leftLayout->addWidget(lblAppSub);
    leftLayout->addSpacing(8);

    // GroupBox: Đăng nhập Sinh viên
    QGroupBox *grpStudent = new QGroupBox("🎓 ĐĂNG NHẬP DÀNH CHO SINH VIÊN", leftPanel);
    grpStudent->setObjectName("grpLoginBox");
    QVBoxLayout *studentLayout = new QVBoxLayout(grpStudent);
    studentLayout->setContentsMargins(18, 20, 18, 18);
    studentLayout->setSpacing(12);

    QLabel *lblUser = new QLabel("Tài khoản (Mã sinh viên):", grpStudent);
    lblUser->setStyleSheet("font-weight: bold; color: #1e293b;");
    edtUsername = new QLineEdit(grpStudent);
    edtUsername->setPlaceholderText("VD: N24DCCN001");
    edtUsername->setMinimumHeight(40);

    QLabel *lblPass = new QLabel("Mật khẩu (Số điện thoại):", grpStudent);
    lblPass->setStyleSheet("font-weight: bold; color: #1e293b;");
    edtPassword = new QLineEdit(grpStudent);
    edtPassword->setEchoMode(QLineEdit::Password);
    edtPassword->setPlaceholderText("Nhập số điện thoại của bạn");
    edtPassword->setMinimumHeight(40);

    btnLoginStudent = new QPushButton("🚀 ĐĂNG NHẬP SINH VIÊN", grpStudent);
    btnLoginStudent->setObjectName("btnLoginPrimary");
    btnLoginStudent->setMinimumHeight(44);
    btnLoginStudent->setCursor(Qt::PointingHandCursor);

    studentLayout->addWidget(lblUser);
    studentLayout->addWidget(edtUsername);
    studentLayout->addWidget(lblPass);
    studentLayout->addWidget(edtPassword);
    studentLayout->addSpacing(6);
    studentLayout->addWidget(btnLoginStudent);

    leftLayout->addWidget(grpStudent);

    // Divider
    QHBoxLayout *divLayout = new QHBoxLayout();
    QFrame *line1 = new QFrame(leftPanel);
    line1->setFrameShape(QFrame::HLine);
    line1->setStyleSheet("color: #cbd5e1;");
    QLabel *lblOr = new QLabel("HOẶC TRUY CẬP TRỰC TIẾP", leftPanel);
    lblOr->setStyleSheet("color: #64748b; font-weight: bold; font-size: 11px;");
    QFrame *line2 = new QFrame(leftPanel);
    line2->setFrameShape(QFrame::HLine);
    line2->setStyleSheet("color: #cbd5e1;");
    divLayout->addWidget(line1);
    divLayout->addWidget(lblOr);
    divLayout->addWidget(line2);
    leftLayout->addLayout(divLayout);

    // Highlight Button Role Giáo Viên
    btnRoleTeacher = new QPushButton("👑 VÀO VAI TRÒ GIÁO VIÊN (ADMIN)", leftPanel);
    btnRoleTeacher->setObjectName("btnRoleTeacherHighlight");
    btnRoleTeacher->setMinimumHeight(52);
    btnRoleTeacher->setCursor(Qt::PointingHandCursor);
    leftLayout->addWidget(btnRoleTeacher);

    lblStatus = new QLabel("<div style='background-color: #f8fafc; border: 1.5px dashed #cbd5e1; border-radius: 8px; padding: 10px 14px; color: #64748b; font-size: 13px;'>"
                           "💡 Vui lòng nhập tài khoản hoặc nhấp vào một sinh viên ở bảng bên phải để tự động điền.</div>", leftPanel);
    lblStatus->setWordWrap(true);
    leftLayout->addWidget(lblStatus);

    leftLayout->addStretch();
    leftPanel->setFixedWidth(380);
    rootLayout->addWidget(leftPanel);

    // ==========================================
    // CỘT PHẢI: BẢNG DANH SÁCH SINH VIÊN (AUTO-FILL)
    // ==========================================
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(12);

    QLabel *lblTableTitle = new QLabel("📋 DANH SÁCH TÀI KHOẢN SINH VIÊN (AUTO-FILL)", rightPanel);
    lblTableTitle->setStyleSheet("font-size: 15px; font-weight: bold; color: #740d18;");
    QLabel *lblTableHint = new QLabel("💡 Nhấp vào bất kỳ dòng nào trong bảng để tự động điền Mã SV và Mật khẩu vào form đăng nhập.", rightPanel);
    lblTableHint->setStyleSheet("font-size: 12px; color: #475569;");

    rightLayout->addWidget(lblTableTitle);
    rightLayout->addWidget(lblTableHint);

    edtSearch = new QLineEdit(rightPanel);
    edtSearch->setPlaceholderText("🔍 Tìm nhanh theo Mã SV, Họ tên hoặc Lớp...");
    edtSearch->setClearButtonEnabled(true);
    edtSearch->setMinimumHeight(36);
    rightLayout->addWidget(edtSearch);

    tblStudents = new QTableWidget(rightPanel);
    tblStudents->setColumnCount(5);
    tblStudents->setHorizontalHeaderLabels({"Mã SV", "Họ và Tên", "Lớp", "Phái", "Mật Khẩu (SĐT)"});
    tblStudents->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblStudents->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tblStudents->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    tblStudents->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tblStudents->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tblStudents->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblStudents->setSelectionMode(QAbstractItemView::SingleSelection);
    tblStudents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblStudents->setAlternatingRowColors(true);
    tblStudents->verticalHeader()->setVisible(false);
    tblStudents->verticalHeader()->setDefaultSectionSize(34);
    rightLayout->addWidget(tblStudents, 1);

    rootLayout->addWidget(rightPanel, 1);

    // Connections
    connect(tblStudents, &QTableWidget::cellClicked, this, &LoginView::onStudentTableRowClicked);
    connect(btnLoginStudent, &QPushButton::clicked, this, &LoginView::onBtnLoginStudentClicked);
    connect(btnRoleTeacher, &QPushButton::clicked, this, &LoginView::loginAsTeacher);
    connect(edtSearch, &QLineEdit::textChanged, this, &LoginView::onSearchStudentChanged);
    connect(edtPassword, &QLineEdit::returnPressed, this, &LoginView::onBtnLoginStudentClicked);
    connect(edtUsername, &QLineEdit::returnPressed, [this]() { edtPassword->setFocus(); });
}

void LoginView::applyTheme() {
    QString qss = R"(
        #lblLoginSchool {
            color: #be3142;
            font-size: 12px;
            font-weight: 800;
            letter-spacing: 1px;
        }
        #lblLoginTitle {
            color: #1e293b;
            font-size: 20px;
            font-weight: 900;
        }
        #lblLoginSub {
            color: #64748b;
            font-size: 13px;
        }
        #grpLoginBox {
            background-color: #ffffff;
            border: 1px solid #cbd5e1;
            border-radius: 10px;
            font-size: 13px;
            font-weight: bold;
            color: #740d18;
            margin-top: 10px;
            padding-top: 10px;
        }
        #btnLoginPrimary {
            background-color: #be3142;
            color: #ffffff;
            font-weight: bold;
            font-size: 14px;
            border-radius: 6px;
            border: 1px solid #991b1b;
        }
        #btnLoginPrimary:hover {
            background-color: #991b1b;
        }
        #btnRoleTeacherHighlight {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #fef08a, stop:1 #fde047);
            color: #740d18;
            font-weight: 900;
            font-size: 14px;
            border: 2px solid #eab308;
            border-radius: 8px;
            letter-spacing: 0.5px;
        }
        #btnRoleTeacherHighlight:hover {
            background: #fde047;
            border-color: #ca8a04;
        }
    )";
    setStyleSheet(qss);
}

void LoginView::loadData() {
    tblStudents->setRowCount(0);
    const DS_LOPSV &dsLop = AppData::instance().dsLop;

    int totalSV = 0;
    for (int i = 0; i < dsLop.n; i++) {
        for (PTRSV p = dsLop.nodes[i].FirstSV; p != nullptr; p = p->next) {
            int row = tblStudents->rowCount();
            tblStudents->insertRow(row);

            QString maSV = QString::fromUtf8(p->sv.MASV);
            QString hoTen = QString("%1 %2").arg(QString::fromUtf8(p->sv.HO)).arg(QString::fromUtf8(p->sv.TEN));
            QString maLop = QString::fromUtf8(dsLop.nodes[i].MALOP);
            QString phai = QString::fromUtf8(p->sv.PHAI);
            QString soDT = QString::fromUtf8(p->sv.SODT);

            auto itMa = new QTableWidgetItem(maSV);
            auto itTen = new QTableWidgetItem(hoTen);
            auto itLop = new QTableWidgetItem(maLop);
            auto itPhai = new QTableWidgetItem(phai);
            auto itDT = new QTableWidgetItem(soDT);

            itMa->setTextAlignment(Qt::AlignCenter);
            itLop->setTextAlignment(Qt::AlignCenter);
            itPhai->setTextAlignment(Qt::AlignCenter);
            itDT->setTextAlignment(Qt::AlignCenter);

            itMa->setFont(QFont("Segoe UI", 9, QFont::Bold));

            tblStudents->setItem(row, 0, itMa);
            tblStudents->setItem(row, 1, itTen);
            tblStudents->setItem(row, 2, itLop);
            tblStudents->setItem(row, 3, itPhai);
            tblStudents->setItem(row, 4, itDT);

            totalSV++;
        }
    }

    lblStatus->setText(QString("Hệ thống hiện có %1 sinh viên sẵn sàng đăng nhập.").arg(totalSV));
}

void LoginView::onStudentTableRowClicked(int row, int /*col*/) {
    if (row < 0 || row >= tblStudents->rowCount()) return;

    QString maSV = tblStudents->item(row, 0)->text();
    QString hoTen = tblStudents->item(row, 1)->text();
    QString soDT = tblStudents->item(row, 4)->text();

    edtUsername->setText(maSV);
    edtPassword->setText(soDT);

    lblStatus->setText(QString("<div style='background-color: #f0fdf4; border: 1.5px solid #86efac; border-radius: 8px; padding: 10px 14px; color: #166534; font-size: 13px; line-height: 1.5;'>"
                               "✅ Đã chọn sinh viên: <b style='font-size: 15px;'>%1</b><br>"
                               "🏷️ Mã SV: <b style='color: #740d18;'>%2</b> &nbsp;|&nbsp; 📞 Mật khẩu: <b>%3</b><br>"
                               "👉 Bấm <b>[🚀 ĐĂNG NHẬP SINH VIÊN]</b> để tiếp tục.</div>")
                           .arg(hoTen).arg(maSV).arg(soDT));
}

void LoginView::onBtnLoginStudentClicked() {
    QString username = edtUsername->text().trimmed().toUpper();
    QString password = edtPassword->text().trimmed();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập Mã sinh viên!");
        edtUsername->setFocus();
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập Mật khẩu (Số điện thoại)!");
        edtPassword->setFocus();
        return;
    }

    const DS_LOPSV &dsLop = AppData::instance().dsLop;
    char cMa[16];
    strncpy(cMa, username.toUtf8().constData(), sizeof(cMa) - 1);
    cMa[sizeof(cMa) - 1] = '\0';

    LOPSV *lopFound = nullptr;
    PTRSV svNode = TimSVToanTruong(dsLop, cMa, lopFound);

    if (svNode == nullptr) {
        QMessageBox::critical(this, "Đăng nhập thất bại",
                              QString("Không tìm thấy sinh viên có Mã SV: '%1' trong hệ thống!").arg(username));
        edtUsername->selectAll();
        edtUsername->setFocus();
        return;
    }

    // Kiểm tra mật khẩu (SĐT)
    QString actualPhone = QString::fromUtf8(svNode->sv.SODT).trimmed();
    if (password != actualPhone) {
        QMessageBox::critical(this, "Đăng nhập thất bại",
                              "Mật khẩu (Số điện thoại) không chính xác! Vui lòng thử lại.");
        edtPassword->selectAll();
        edtPassword->setFocus();
        return;
    }

    // Đăng nhập thành công
    QString hoTen = QString("%1 %2").arg(QString::fromUtf8(svNode->sv.HO)).arg(QString::fromUtf8(svNode->sv.TEN));
    QString maLop = lopFound ? QString::fromUtf8(lopFound->MALOP) : "";
    QString phai = QString::fromUtf8(svNode->sv.PHAI);

    emit loginAsStudent(username, hoTen, maLop, phai, actualPhone);
}

void LoginView::onSearchStudentChanged(const QString &text) {
    QString q = text.trimmed();
    for (int r = 0; r < tblStudents->rowCount(); r++) {
        if (q.isEmpty()) {
            tblStudents->setRowHidden(r, false);
            continue;
        }
        bool match = false;
        for (int c = 0; c < tblStudents->columnCount(); c++) {
            if (tblStudents->item(r, c) && tblStudents->item(r, c)->text().contains(q, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        tblStudents->setRowHidden(r, !match);
    }
}

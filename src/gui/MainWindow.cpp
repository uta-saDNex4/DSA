#include "MainWindow.h"
#include "Toast.h"
#include "UndoRedo.h"
#include <QDateTime>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStatusBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QShortcut>
#include <QKeySequence>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    applyTheme();
    setupShortcuts();
    updateStatusBarStats();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUI() {
    setWindowTitle("HỆ THỐNG QUẢN LÝ ĐIỂM SINH VIÊN THEO HỆ TÍN CHỈ - PTIT DSA 2026");
    resize(1280, 800);
    setMinimumSize(1024, 680);

    stackedWidget = new QStackedWidget(this);

    // ==========================================
    // 0. LOGIN VIEW (TRANG CHỦ ĐĂNG NHẬP)
    // ==========================================
    loginView = new LoginView(this);
    connect(loginView, &LoginView::loginAsStudent, this, &MainWindow::onLoginAsStudent);
    connect(loginView, &LoginView::loginAsTeacher, this, &MainWindow::onLoginAsTeacher);
    stackedWidget->addWidget(loginView);

    // ==========================================
    // 1. TEACHER CONTAINER (DASHBOARD GIÁO VIÊN)
    // ==========================================
    setupTeacherUI();
    stackedWidget->addWidget(teacherContainer);

    // ==========================================
    // 2. STUDENT DASHBOARD VIEW (DASHBOARD SINH VIÊN)
    // ==========================================
    studentDashboardView = new StudentDashboardView(this);
    connect(studentDashboardView, &StudentDashboardView::logoutRequested, this, &MainWindow::onLogout);
    stackedWidget->addWidget(studentDashboardView);

    setCentralWidget(stackedWidget);

    // Bắt đầu ở màn hình đăng nhập
    stackedWidget->setCurrentIndex(0);

    // ==========================================
    // STATUS BAR
    // ==========================================
    QStatusBar *bar = statusBar();
    bar->setObjectName("statusBar");

    statusMsgLabel = new QLabel("Sẵn sàng. (Dữ liệu tự động lưu an toàn sau mỗi thao tác)", this);
    statusStatsLabel = new QLabel(this);
    statusStatsLabel->setStyleSheet("color: #475569; font-weight: 500; padding-right: 12px;");

    bar->addWidget(statusMsgLabel, 1);
    bar->addPermanentWidget(statusStatsLabel);
}

void MainWindow::setupTeacherUI() {
    teacherContainer = new QWidget(this);
    QVBoxLayout *rootLayout = new QVBoxLayout(teacherContainer);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ==========================================
    // TOP HEADER BANNER (ROLE GIÁO VIÊN / ADMIN)
    // ==========================================
    QWidget *headerWidget = new QWidget(teacherContainer);
    headerWidget->setObjectName("headerWidget");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 12, 20, 12);
    headerLayout->setSpacing(14);

    QVBoxLayout *titleLayout = new QVBoxLayout();
    QLabel *lblSchool = new QLabel("HỌC VIỆN CÔNG NGHỆ BƯU CHÍNH VIỄN THÔNG", headerWidget);
    lblSchool->setObjectName("lblSchool");
    QLabel *lblTitle = new QLabel("HỆ THỐNG QUẢN LÝ ĐIỂM TÍN CHỈ - [VAI TRÒ: GIÁO VIÊN / ADMIN]", headerWidget);
    lblTitle->setObjectName("lblTitle");

    titleLayout->addWidget(lblSchool);
    titleLayout->addWidget(lblTitle);
    headerLayout->addLayout(titleLayout);

    headerLayout->addStretch();

    QPushButton *btnUndo = new QPushButton("↩️ [Ctrl+Z] Hoàn Tác", headerWidget);
    btnUndo->setObjectName("btnHeaderAction");
    btnUndo->setMinimumHeight(38);
    btnUndo->setCursor(Qt::PointingHandCursor);
    connect(btnUndo, &QPushButton::clicked, this, &MainWindow::onUndo);

    QPushButton *btnRedo = new QPushButton("↪️ [Ctrl+Y] Làm Lại", headerWidget);
    btnRedo->setObjectName("btnHeaderAction");
    btnRedo->setMinimumHeight(38);
    btnRedo->setCursor(Qt::PointingHandCursor);
    connect(btnRedo, &QPushButton::clicked, this, &MainWindow::onRedo);

    QPushButton *btnSave = new QPushButton("💾 [F2] Lưu Toàn Bộ", headerWidget);
    btnSave->setObjectName("btnSave");
    btnSave->setMinimumHeight(38);
    btnSave->setCursor(Qt::PointingHandCursor);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveAllData);

    QPushButton *btnReload = new QPushButton("🔄 [F5] Tải Lại", headerWidget);
    btnReload->setObjectName("btnHeaderAction");
    btnReload->setMinimumHeight(38);
    btnReload->setCursor(Qt::PointingHandCursor);
    connect(btnReload, &QPushButton::clicked, this, &MainWindow::onReloadData);

    QPushButton *btnAbout = new QPushButton("ℹ️ Giới Thiệu", headerWidget);
    btnAbout->setObjectName("btnHeaderAction");
    btnAbout->setMinimumHeight(38);
    btnAbout->setCursor(Qt::PointingHandCursor);
    connect(btnAbout, &QPushButton::clicked, this, &MainWindow::onAboutDialog);

    QPushButton *btnLogout = new QPushButton("🚪 Đăng Xuất", headerWidget);
    btnLogout->setObjectName("btnHeaderAction");
    btnLogout->setMinimumHeight(38);
    btnLogout->setCursor(Qt::PointingHandCursor);
    connect(btnLogout, &QPushButton::clicked, this, &MainWindow::onLogout);

    headerLayout->addWidget(btnUndo);
    headerLayout->addWidget(btnRedo);
    headerLayout->addWidget(btnSave);
    headerLayout->addWidget(btnReload);
    headerLayout->addWidget(btnAbout);
    headerLayout->addWidget(btnLogout);

    rootLayout->addWidget(headerWidget);

    // ==========================================
    // MAIN TAB WIDGET FOR TEACHER
    // ==========================================
    tabWidget = new QTabWidget(teacherContainer);
    tabWidget->setObjectName("mainTabWidget");

    tabMonHoc = new TabMonHoc(tabWidget);
    tabLopSinhVien = new TabLopSinhVien(tabWidget);
    tabLopTinChi = new TabLopTinChi(tabWidget);
    tabDangKy = new TabDangKy(tabWidget);
    tabDiem = new TabDiem(tabWidget);

    tabWidget->addTab(tabMonHoc, "📚 1. Môn Học");
    tabWidget->addTab(tabLopSinhVien, "🎓 2. Lớp & Sinh Viên");
    tabWidget->addTab(tabLopTinChi, "📋 3. Lớp Tín Chỉ");
    tabWidget->addTab(tabDangKy, "📝 4. Hỗ Trợ Đăng Ký");
    tabWidget->addTab(tabDiem, "📊 5. Điểm & Báo Cáo");

    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    rootLayout->addWidget(tabWidget, 1);
}

void MainWindow::applyTheme() {
    QString qss = R"(
        QMainWindow {
            background-color: #f8fafc;
        }

        #headerWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #740d18, stop:1 #be3142);
            border-bottom: 2px solid #500911;
        }

        #lblSchool {
            color: #fde047;
            font-size: 11px;
            font-weight: 800;
            letter-spacing: 1.5px;
        }

        #lblTitle {
            color: #ffffff;
            font-size: 17px;
            font-weight: 900;
        }

        #btnSave {
            background-color: #fde047;
            color: #740d18;
            font-weight: bold;
            font-size: 13px;
            border-radius: 6px;
            padding: 0 16px;
            border: 1px solid #ca8a04;
        }
        #btnSave:hover {
            background-color: #fef08a;
        }

        #btnHeaderAction {
            background-color: rgba(255, 255, 255, 0.15);
            color: white;
            font-weight: bold;
            font-size: 13px;
            border-radius: 6px;
            padding: 0 14px;
            border: 1px solid rgba(255, 255, 255, 0.35);
        }
        #btnHeaderAction:hover {
            background-color: rgba(255, 255, 255, 0.25);
        }

        #mainTabWidget::pane {
            border: none;
            background: #f8fafc;
        }

        QTabBar::tab {
            background: #e2e8f0;
            color: #334155;
            font-size: 14px;
            font-weight: bold;
            padding: 12px 24px;
            border-top-left-radius: 8px;
            border-top-right-radius: 8px;
            margin-right: 4px;
            margin-top: 6px;
        }

        QTabBar::tab:selected {
            background: #ffffff;
            color: #740d18;
            border-top: 3px solid #be3142;
        }

        /* ========================================================
           BẢNG VÀ DANH SÁCH TOÀN HỆ THỐNG (UNIFIED DESIGN SYSTEM)
           ======================================================== */
        QTableWidget, QTableView {
            background-color: #ffffff;
            alternate-background-color: #f8fafc;
            color: #0f172a;
            gridline-color: #e2e8f0;
            border: 1px solid #cbd5e1;
            border-radius: 8px;
            font-size: 13px;
            selection-background-color: #fee2e2;
            selection-color: #740d18;
        }

        QTableWidget::item, QTableView::item {
            padding: 5px 8px;
            border: none;
        }

        QTableWidget::item:alternate, QTableView::item:alternate {
            background-color: #f8fafc;
        }

        QTableWidget::item:selected:active, QTableView::item:selected:active {
            background-color: #fee2e2;
            color: #740d18;
            font-weight: bold;
        }

        QTableWidget::item:selected:!active, QTableView::item:selected:!active {
            background-color: #e2e8f0;
            color: #0f172a;
            font-weight: normal;
        }

        QTableWidget::item:hover, QTableView::item:hover {
            background-color: #f1f5f9;
        }

        /* HEADER CỦA TẤT CẢ CÁC BẢNG */
        QHeaderView::section {
            background-color: #f1f5f9;
            color: #1e293b;
            font-weight: bold;
            font-size: 13px;
            padding: 8px 6px;
            border: none;
            border-bottom: 2px solid #cbd5e1;
            border-right: 1px solid #e2e8f0;
        }

        /* DANH SÁCH (QLISTWIDGET) */
        QListWidget {
            background-color: #ffffff;
            color: #0f172a;
            border: 1px solid #cbd5e1;
            border-radius: 8px;
            font-size: 13px;
        }

        QListWidget::item {
            color: #0f172a;
            padding: 10px 12px;
            border-bottom: 1px solid #e2e8f0;
        }

        QListWidget::item:alternate {
            background-color: #f8fafc;
        }

        QListWidget::item:hover {
            background-color: #f1f5f9;
        }

        QListWidget::item:selected:active {
            background-color: #fee2e2;
            color: #740d18;
            font-weight: bold;
        }

        QListWidget::item:selected:!active {
            background-color: #e2e8f0;
            color: #0f172a;
            font-weight: normal;
        }

        QLineEdit, QComboBox, QSpinBox {
            background-color: #ffffff;
            color: #1e293b;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            padding: 5px 8px;
            font-size: 13px;
        }

        QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
            border: 1.5px solid #be3142;
        }

        QDialog {
            background-color: #ffffff;
            color: #1e293b;
        }

        /* THANH CUỘN HIỆN ĐẠI */
        QScrollBar:vertical {
            background: #f8fafc;
            width: 10px;
            margin: 0px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical {
            background: #cbd5e1;
            min-height: 24px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical:hover {
            background: #94a3b8;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }

        QScrollBar:horizontal {
            background: #f8fafc;
            height: 10px;
            margin: 0px;
            border-radius: 5px;
        }
        QScrollBar::handle:horizontal {
            background: #cbd5e1;
            min-width: 24px;
            border-radius: 5px;
        }
        QScrollBar::handle:horizontal:hover {
            background: #94a3b8;
        }
        QScrollBar::add-line:horizontal, QScrollBar::sub-line:horizontal {
            width: 0px;
        }

        QStatusBar {
            background-color: #ffffff;
            border-top: 1px solid #e2e8f0;
            color: #334155;
            font-size: 12px;
        }
    )";

    if (qobject_cast<QApplication*>(QApplication::instance())) qobject_cast<QApplication*>(QApplication::instance())->setStyleSheet(qss);
    setStyleSheet(qss);
}

void MainWindow::setupShortcuts() {
    new QShortcut(QKeySequence(Qt::Key_F2), this, SLOT(onSaveAllData()));
    new QShortcut(QKeySequence(Qt::Key_F5), this, SLOT(onReloadData()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z), this, SLOT(onUndo()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y), this, SLOT(onRedo()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::SHIFT | Qt::Key_Z), this, SLOT(onRedo()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q), this, SLOT(close()));
}

void MainWindow::updateStatusBarStats() {
    int countMH = AppData::instance().dsMH.n;
    int countLop = AppData::instance().dsLop.n;
    int countLTC = AppData::instance().dsLTC.n;

    statusStatsLabel->setText(
        QString("📚 %1 Môn Học | 🏫 %2 Lớp Học | 📋 %3 Lớp Tín Chỉ | ⚡ MinGW Qt6")
            .arg(countMH).arg(countLop).arg(countLTC));
}

void MainWindow::onLoginAsStudent(const QString &maSV, const QString &hoTen, const QString &maLop, const QString &phai, const QString &soDT) {
    studentDashboardView->setStudentInfo(maSV, hoTen, maLop, phai, soDT);
    stackedWidget->setCurrentIndex(2);
    statusMsgLabel->setText(QString("👤 Đang đăng nhập: Sinh viên %1 (%2)").arg(hoTen).arg(maSV));
    updateStatusBarStats();
}

void MainWindow::onLoginAsTeacher() {
    tabDangKy->setAdminMode();
    tabMonHoc->loadData();
    stackedWidget->setCurrentIndex(1);
    statusMsgLabel->setText("👑 Đang đăng nhập: Vai trò Giáo Viên / Quản Trị Viên");
    updateStatusBarStats();
}

void MainWindow::onLogout() {
    AppData::instance().saveAll();
    loginView->loadData();
    stackedWidget->setCurrentIndex(0);
    statusMsgLabel->setText("Đã đăng xuất. Vui lòng chọn tài khoản hoặc vai trò.");
    updateStatusBarStats();
}

void MainWindow::onTabChanged(int index) {
    switch (index) {
        case 0: tabMonHoc->loadData(); break;
        case 1: tabLopSinhVien->loadData(); break;
        case 2: tabLopTinChi->loadData(); break;
        case 3: tabDangKy->loadData(); break;
        case 4: tabDiem->loadData(); break;
        default: break;
    }
    updateStatusBarStats();
}

void MainWindow::onSaveAllData() {
    AppData::instance().saveAll();
    statusMsgLabel->setText("✅ Đã lưu toàn bộ dữ liệu thành công!");
    updateStatusBarStats();
    QMessageBox::information(this, "Thông báo", "Đã lưu toàn bộ dữ liệu thành công!");
}

void MainWindow::onReloadData() {
    // Chống spam F5 hoặc spam click nút Tải Lại gây chớp nháy toàn trang
    static qint64 lastReloadTime = 0;
    qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (now - lastReloadTime < 600) {
        return; // Bỏ qua các lần kích hoạt quá nhanh liên tiếp trong 600ms
    }
    lastReloadTime = now;

    // Khóa vẽ lại giao diện tạm thời để cập nhật mượt mà không chớp nháy
    tabWidget->setUpdatesEnabled(false);
    AppData::instance().loadAll();
    if (stackedWidget->currentIndex() == 1) {
        onTabChanged(tabWidget->currentIndex());
    } else if (stackedWidget->currentIndex() == 0) {
        loginView->loadData();
    }
    tabWidget->setUpdatesEnabled(true);

    Toast::showSuccess(this, "Đã nạp lại toàn bộ dữ liệu từ ổ đĩa!");
    statusMsgLabel->setText("🔄 Đã nạp lại dữ liệu từ ổ đĩa.");
    updateStatusBarStats();
}

void MainWindow::onAboutDialog() {
    QMessageBox::about(this, "Giới thiệu Đồ Án CTDL & GT (DSA)",
                       "<h2>HỌC VIỆN CÔNG NGHỆ BƯU CHÍNH VIỄN THÔNG</h2>"
                       "<h3>ĐỒ ÁN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT (2026)</h3>"
                       "<b>Đề tài:</b> Quản lý điểm sinh viên theo hệ tín chỉ<br><br>"
                       "<b>Hệ thống phân quyền 2 Role:</b><br>"
                       "• <b>Giáo viên (Admin):</b> Quản trị danh mục môn học, lớp, lớp tín chỉ, nhập điểm, báo cáo.<br>"
                       "• <b>Sinh viên:</b> Đăng ký tín chỉ, tra cứu bảng điểm cá nhân & ĐTB tích lũy.<br><br>"
                       "<b>Kiến trúc cấu trúc dữ liệu thuần C/C++:</b><br>"
                       "• <b>Môn học:</b> Cây nhị phân tìm kiếm (Binary Search Tree)<br>"
                       "• <b>Lớp sinh viên:</b> Mảng tuyến tính trực tiếp (Static Array 10000 phần tử)<br>"
                       "• <b>Sinh viên:</b> Danh sách liên kết đơn có thứ tự theo Tên + Họ<br>"
                       "• <b>Lớp tín chỉ:</b> Danh sách tuyến tính mảng con trỏ (Pointer Array 10000 phần tử)<br>"
                       "• <b>Đăng ký & Điểm:</b> Danh sách liên kết đơn<br><br>"
                       "<b>Giao diện:</b> Modern Qt6 GUI C++ (QStackedWidget, QTableWidget, Custom Stylesheets)<br>"
                       "<b>Công nghệ build:</b> CMake + MinGW GCC 16 + Ninja");
}

void MainWindow::closeEvent(QCloseEvent *event) {
    auto ret = QMessageBox::question(this, "Xác nhận thoát",
                                     "Bạn có muốn lưu toàn bộ dữ liệu trước khi thoát ứng dụng không?",
                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (ret == QMessageBox::Yes) {
        AppData::instance().saveAll();
        event->accept();
    } else if (ret == QMessageBox::No) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::onUndo() {
    QString desc;
    if (UndoRedoManager::instance().undo(desc)) {
        onTabChanged(tabWidget->currentIndex());
        Toast::showSuccess(this, "↩️ Đã hoàn tác: " + desc);
        statusMsgLabel->setText("↩️ Đã hoàn tác: " + desc);
        updateStatusBarStats();
    } else {
        Toast::showInfo(this, "Không còn thao tác nào để hoàn tác!");
    }
}

void MainWindow::onRedo() {
    QString desc;
    if (UndoRedoManager::instance().redo(desc)) {
        onTabChanged(tabWidget->currentIndex());
        Toast::showSuccess(this, "↪️ Đã làm lại: " + desc);
        statusMsgLabel->setText("↪️ Đã làm lại: " + desc);
        updateStatusBarStats();
    } else {
        Toast::showInfo(this, "Không còn thao tác nào để làm lại!");
    }
}

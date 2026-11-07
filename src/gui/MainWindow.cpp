#include "MainWindow.h"
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

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *rootLayout = new QVBoxLayout(centralWidget);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ==========================================
    // TOP HEADER BANNER (PTIT STYLE)
    // ==========================================
    QWidget *headerWidget = new QWidget(centralWidget);
    headerWidget->setObjectName("headerWidget");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(20, 12, 20, 12);
    headerLayout->setSpacing(16);

    QVBoxLayout *titleLayout = new QVBoxLayout();
    QLabel *lblSchool = new QLabel("HỌC VIỆN CÔNG NGHỆ BƯU CHÍNH VIỄN THÔNG", headerWidget);
    lblSchool->setObjectName("lblSchool");
    QLabel *lblTitle = new QLabel("HỆ THỐNG QUẢN LÝ ĐIỂM SINH VIÊN THEO HỆ TÍN CHỈ (DSA)", headerWidget);
    lblTitle->setObjectName("lblTitle");

    titleLayout->addWidget(lblSchool);
    titleLayout->addWidget(lblTitle);
    headerLayout->addLayout(titleLayout);

    headerLayout->addStretch();

    QPushButton *btnSave = new QPushButton("💾 [F2] Lưu Toàn Bộ", headerWidget);
    btnSave->setObjectName("btnSave");
    btnSave->setMinimumHeight(38);
    connect(btnSave, &QPushButton::clicked, this, &MainWindow::onSaveAllData);

    QPushButton *btnReload = new QPushButton("🔄 [F5] Tải Lại", headerWidget);
    btnReload->setObjectName("btnHeaderAction");
    btnReload->setMinimumHeight(38);
    connect(btnReload, &QPushButton::clicked, this, &MainWindow::onReloadData);

    QPushButton *btnAbout = new QPushButton("ℹ️ Giới Thiệu", headerWidget);
    btnAbout->setObjectName("btnHeaderAction");
    btnAbout->setMinimumHeight(38);
    connect(btnAbout, &QPushButton::clicked, this, &MainWindow::onAboutDialog);

    headerLayout->addWidget(btnSave);
    headerLayout->addWidget(btnReload);
    headerLayout->addWidget(btnAbout);

    rootLayout->addWidget(headerWidget);

    // ==========================================
    // MAIN TAB WIDGET
    // ==========================================
    tabWidget = new QTabWidget(centralWidget);
    tabWidget->setObjectName("mainTabWidget");

    tabMonHoc = new TabMonHoc(tabWidget);
    tabLopSinhVien = new TabLopSinhVien(tabWidget);
    tabLopTinChi = new TabLopTinChi(tabWidget);
    tabDangKy = new TabDangKy(tabWidget);
    tabDiem = new TabDiem(tabWidget);

    tabWidget->addTab(tabMonHoc, "📚 1. Môn Học");
    tabWidget->addTab(tabLopSinhVien, "🎓 2. Lớp & Sinh Viên");
    tabWidget->addTab(tabLopTinChi, "📋 3. Lớp Tín Chỉ");
    tabWidget->addTab(tabDangKy, "📝 4. Đăng Ký Tín Chỉ");
    tabWidget->addTab(tabDiem, "📊 5. Điểm & Báo Cáo");

    connect(tabWidget, &QTabWidget::currentChanged, this, &MainWindow::onTabChanged);

    rootLayout->addWidget(tabWidget, 1);

    setCentralWidget(centralWidget);

    // ==========================================
    // STATUS BAR
    // ==========================================
    QStatusBar *bar = statusBar();
    bar->setObjectName("statusBar");

    statusMsgLabel = new QLabel("Sẵn sàng.", this);
    statusStatsLabel = new QLabel(this);
    statusStatsLabel->setStyleSheet("color: #475569; font-weight: 500; padding-right: 12px;");

    bar->addWidget(statusMsgLabel, 1);
    bar->addPermanentWidget(statusStatsLabel);
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
            font-size: 18px;
            font-weight: 900;
        }

        #btnSave {
            background-color: #fde047;
            color: #740d18;
            font-weight: bold;
            font-size: 13px;
            border-radius: 6px;
            padding: 0 16px;
            border: none;
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
            border: 1px solid rgba(255, 255, 255, 0.3);
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

        QTableWidget {
            background-color: #ffffff;
            gridline-color: #f1f5f9;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            font-size: 13px;
            selection-background-color: #fee2e2;
            selection-color: #740d18;
        }

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

        QStatusBar {
            background-color: #ffffff;
            border-top: 1px solid #e2e8f0;
            color: #334155;
            font-size: 12px;
        }
    )";

    setStyleSheet(qss);
}

void MainWindow::setupShortcuts() {
    new QShortcut(QKeySequence(Qt::Key_F2), this, SLOT(onSaveAllData()));
    new QShortcut(QKeySequence(Qt::Key_F5), this, SLOT(onReloadData()));
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
    AppData::instance().loadAll();
    onTabChanged(tabWidget->currentIndex());
    statusMsgLabel->setText("🔄 Đã nạp lại toàn bộ dữ liệu từ ổ đĩa!");
    updateStatusBarStats();
}

void MainWindow::onAboutDialog() {
    QMessageBox::about(this, "Giới thiệu Đồ Án CTDL & GT (DSA)",
                       "<h2>HỌC VIỆN CÔNG NGHỆ BƯU CHÍNH VIỄN THÔNG</h2>"
                       "<h3>ĐỒ ÁN CẤU TRÚC DỮ LIỆU & GIẢI THUẬT (2026)</h3>"
                       "<b>Đề tài:</b> Quản lý điểm sinh viên theo hệ tín chỉ<br><br>"
                       "<b>Kiến trúc cấu trúc dữ liệu thuần C/C++:</b><br>"
                       "• <b>Môn học:</b> Cây nhị phân tìm kiếm (Binary Search Tree)<br>"
                       "• <b>Lớp sinh viên:</b> Mảng tuyến tính trực tiếp (Static Array 10000 phần tử)<br>"
                       "• <b>Sinh viên:</b> Danh sách liên kết đơn có thứ tự theo Tên + Họ<br>"
                       "• <b>Lớp tín chỉ:</b> Danh sách tuyến tính mảng con trỏ (Pointer Array 10000 phần tử)<br>"
                       "• <b>Đăng ký & Điểm:</b> Danh sách liên kết đơn<br><br>"
                       "<b>Giao diện:</b> Modern Qt6 GUI C++ (QTableWidget, QSplitter, Custom Stylesheets)<br>"
                       "<b>Công nghệ build:</b> CMake + MinGW GCC 15.2 + Ninja");
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

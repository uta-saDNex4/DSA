#include "StudentDashboardView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QGroupBox>
#include <QFrame>
#include <vector>
#include <map>
#include <string>
#include <iomanip>
#include <sstream>
#include "AppData.h"

StudentDashboardView::StudentDashboardView(QWidget *parent) : QWidget(parent) {
    setupUI();
    applyTheme();
}

void StudentDashboardView::setupUI() {
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // ==========================================
    // TOP HEADER PROFILE CARD (TO & RÕ NÉT)
    // ==========================================
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setObjectName("studentHeaderWidget");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(28, 16, 28, 16);
    headerLayout->setSpacing(18);

    lblAvatar = new QLabel("🎓", headerWidget);
    lblAvatar->setObjectName("lblStudentAvatar");
    lblAvatar->setAlignment(Qt::AlignCenter);
    lblAvatar->setFixedSize(58, 58);
    headerLayout->addWidget(lblAvatar);

    QVBoxLayout *infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(6);

    QLabel *lblSub = new QLabel("HỌC VIỆN CÔNG NGHỆ BƯU CHÍNH VIỄN THÔNG - CỔNG THÔNG TIN SINH VIÊN", headerWidget);
    lblSub->setStyleSheet("color: #fde047; font-size: 11px; font-weight: 800; letter-spacing: 1.2px;");
    
    lblProfileName = new QLabel("SINH VIÊN: ĐANG TẢI...", headerWidget);
    lblProfileName->setObjectName("lblStudentProfileName");

    infoLayout->addWidget(lblSub);
    infoLayout->addWidget(lblProfileName);

    QHBoxLayout *badgesLayout = new QHBoxLayout();
    badgesLayout->setSpacing(12);

    lblBadgeMaSV = new QLabel("🏷️ Mã SV: ---", headerWidget);
    lblBadgeLop = new QLabel("🏫 Lớp: ---", headerWidget);
    lblBadgePhai = new QLabel("⚧️ Phái: ---", headerWidget);
    lblBadgeSDT = new QLabel("📞 SĐT: ---", headerWidget);

    lblBadgeMaSV->setObjectName("lblStudentBadge");
    lblBadgeLop->setObjectName("lblStudentBadge");
    lblBadgePhai->setObjectName("lblStudentBadge");
    lblBadgeSDT->setObjectName("lblStudentBadge");

    badgesLayout->addWidget(lblBadgeMaSV);
    badgesLayout->addWidget(lblBadgeLop);
    badgesLayout->addWidget(lblBadgePhai);
    badgesLayout->addWidget(lblBadgeSDT);
    badgesLayout->addStretch();

    infoLayout->addLayout(badgesLayout);
    headerLayout->addLayout(infoLayout, 1);

    btnLogout = new QPushButton("🚪 Đăng Xuất", headerWidget);
    btnLogout->setObjectName("btnStudentLogout");
    btnLogout->setMinimumHeight(42);
    btnLogout->setMinimumWidth(120);
    btnLogout->setCursor(Qt::PointingHandCursor);
    connect(btnLogout, &QPushButton::clicked, this, &StudentDashboardView::logoutRequested);
    headerLayout->addWidget(btnLogout);

    rootLayout->addWidget(headerWidget);

    // ==========================================
    // MAIN TAB WIDGET
    // ==========================================
    tabWidget = new QTabWidget(this);
    tabWidget->setObjectName("studentMainTabWidget");

    // 1. Tab Đăng Ký Tín Chỉ
    tabDangKy = new TabDangKy(tabWidget);
    tabWidget->addTab(tabDangKy, "📝 1. Đăng Ký Tín Chỉ (Câu g)");

    // 2. Tab Kết Quả Học Tập
    tabDiemCaNhan = new QWidget(tabWidget);
    QVBoxLayout *layoutDiem = new QVBoxLayout(tabDiemCaNhan);
    layoutDiem->setContentsMargins(18, 16, 18, 16);
    layoutDiem->setSpacing(12);

    QLabel *lblTitleDiem = new QLabel("📊 BẢNG ĐIỂM & TIẾN ĐỘ HỌC TẬP CÁ NHÂN", tabDiemCaNhan);
    lblTitleDiem->setStyleSheet("font-size: 15px; font-weight: bold; color: #740d18;");
    layoutDiem->addWidget(lblTitleDiem);

    tblDiem = new QTableWidget(tabDiemCaNhan);
    tblDiem->setColumnCount(9);
    tblDiem->setHorizontalHeaderLabels({"STT", "Mã MH", "Tên Môn Học", "Số TC", "Niên Khóa", "Học Kỳ", "Nhóm", "Điểm Thi", "Đánh Giá"});
    tblDiem->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblDiem->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tblDiem->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tblDiem->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tblDiem->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tblDiem->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tblDiem->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tblDiem->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tblDiem->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    tblDiem->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblDiem->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblDiem->setAlternatingRowColors(true);
    tblDiem->verticalHeader()->setVisible(false);
    tblDiem->verticalHeader()->setDefaultSectionSize(34);
    layoutDiem->addWidget(tblDiem, 1);

    lblSummaryStats = new QLabel("Tổng tín chỉ tích lũy: 0 | Điểm TB tích lũy: 0.0", tabDiemCaNhan);
    lblSummaryStats->setObjectName("lblStudentSummaryStats");
    layoutDiem->addWidget(lblSummaryStats);

    tabWidget->addTab(tabDiemCaNhan, "📊 2. Bảng Điểm Cá Nhân");

    // 3. Tab Danh Mục Môn Học
    tabTraCuuMH = new QWidget(tabWidget);
    QVBoxLayout *layoutMH = new QVBoxLayout(tabTraCuuMH);
    layoutMH->setContentsMargins(18, 16, 18, 16);
    layoutMH->setSpacing(12);

    QLabel *lblTitleMH = new QLabel("📚 DANH MỤC TẤT CẢ MÔN HỌC (CÂU F - DUYỆT CÂY BST)", tabTraCuuMH);
    lblTitleMH->setStyleSheet("font-size: 15px; font-weight: bold; color: #740d18;");
    layoutMH->addWidget(lblTitleMH);

    edtSearchMH = new QLineEdit(tabTraCuuMH);
    edtSearchMH->setPlaceholderText("🔍 Tìm nhanh môn học theo mã hoặc tên...");
    edtSearchMH->setClearButtonEnabled(true);
    edtSearchMH->setMinimumHeight(36);
    connect(edtSearchMH, &QLineEdit::textChanged, this, &StudentDashboardView::onSearchMonHocChanged);
    layoutMH->addWidget(edtSearchMH);

    tblMonHoc = new QTableWidget(tabTraCuuMH);
    tblMonHoc->setColumnCount(6);
    tblMonHoc->setHorizontalHeaderLabels({"STT", "Mã MH", "Tên Môn Học", "Tín Chỉ LT", "Tín Chỉ TH", "Tổng Tín Chỉ"});
    tblMonHoc->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblMonHoc->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tblMonHoc->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tblMonHoc->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tblMonHoc->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tblMonHoc->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tblMonHoc->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblMonHoc->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblMonHoc->setAlternatingRowColors(true);
    tblMonHoc->verticalHeader()->setVisible(false);
    tblMonHoc->verticalHeader()->setDefaultSectionSize(34);
    layoutMH->addWidget(tblMonHoc, 1);

    tabWidget->addTab(tabTraCuuMH, "📚 3. Danh Mục Môn Học");

    rootLayout->addWidget(tabWidget, 1);

    connect(tabWidget, &QTabWidget::currentChanged, this, &StudentDashboardView::onTabChanged);
}

void StudentDashboardView::applyTheme() {
    QString qss = R"(
        #studentHeaderWidget {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #740d18, stop:1 #be3142);
            border-bottom: 2px solid #500911;
        }
        #lblStudentAvatar {
            font-size: 30px;
            background: rgba(255, 255, 255, 0.18);
            border: 2px solid rgba(255, 255, 255, 0.4);
            border-radius: 12px;
        }
        #lblStudentProfileName {
            color: #ffffff;
            font-size: 22px;
            font-weight: 900;
            letter-spacing: 0.5px;
        }
        #lblStudentBadge {
            background: rgba(0, 0, 0, 0.28);
            border: 1px solid rgba(255, 255, 255, 0.25);
            border-radius: 6px;
            padding: 5px 14px;
            font-size: 13px;
            color: #f1f5f9;
            font-weight: 600;
        }
        #btnStudentLogout {
            background-color: rgba(255, 255, 255, 0.15);
            color: white;
            font-weight: bold;
            font-size: 13px;
            border-radius: 6px;
            padding: 0 16px;
            border: 1px solid rgba(255, 255, 255, 0.35);
        }
        #btnStudentLogout:hover {
            background-color: rgba(255, 255, 255, 0.25);
        }
        #lblStudentSummaryStats {
            background-color: #f1f5f9;
            color: #1e293b;
            border: 1px solid #cbd5e1;
            border-left: 5px solid #be3142;
            border-radius: 6px;
            padding: 14px 18px;
            font-size: 15px;
            font-weight: bold;
        }
    )";
    setStyleSheet(qss);
}

void StudentDashboardView::setStudentInfo(const QString &maSV, const QString &hoTen, const QString &maLop, const QString &phai, const QString &soDT) {
    m_maSV = maSV;
    m_hoTen = hoTen;
    m_maLop = maLop;
    m_phai = phai;
    m_soDT = soDT;

    lblProfileName->setText(m_hoTen.toUpper());
    lblBadgeMaSV->setText(QString("🏷️ <b>Mã SV:</b> <span style='color:#fde047; font-size:16px;'>%1</span>").arg(m_maSV));
    lblBadgeLop->setText(QString("🏫 <b>Lớp:</b> <span style='color:#ffffff; font-size:15px;'>%1</span>").arg(m_maLop));
    lblBadgePhai->setText(QString("⚧️ <b>Phái:</b> <span style='color:#ffffff; font-size:15px;'>%1</span>").arg(m_phai));
    lblBadgeSDT->setText(QString("📞 <b>SĐT:</b> <span style='color:#ffffff; font-size:15px;'>%1</span>").arg(m_soDT));

    // Khóa mã SV ở tab đăng ký tín chỉ
    tabDangKy->setStudentMode(m_maSV);

    // Nạp dữ liệu các tab
    loadBangDiemCaNhan();
    loadDanhSachMonHoc();
}

void StudentDashboardView::onTabChanged(int index) {
    if (index == 0) {
        tabDangKy->setStudentMode(m_maSV);
    } else if (index == 1) {
        loadBangDiemCaNhan();
    } else if (index == 2) {
        loadDanhSachMonHoc();
    }
}

void StudentDashboardView::loadBangDiemCaNhan() {
    tblDiem->setRowCount(0);
    const DS_LopTC &dsltc = AppData::instance().dsLTC;
    const DS_MonHoc &dsMH = AppData::instance().dsMH;

    struct MonRecord {
        QString maMH;
        QString tenMH;
        int tongSTC;
        QString nienKhoa;
        int hocKy;
        int nhom;
        float diem;
    };

    std::vector<MonRecord> listDangKy;
    // Để tính ĐTB tích lũy: lấy điểm thi lớn nhất của từng môn
    std::map<std::string, float> maxDiemTheoMon;
    std::map<std::string, int> tcTheoMon;

    for (int i = 0; i < dsltc.n; i++) {
        LopTinChi *ltc = dsltc.nodes[i];
        if (!ltc || ltc->HuyLop) continue;

        for (PTR_DangKy p = ltc->FirstDK; p != nullptr; p = p->next) {
            if (p->dk.HuyDangKy) continue;
            if (m_maSV.compare(QString::fromUtf8(p->dk.MASV), Qt::CaseInsensitive) == 0) {
                TreeMonHoc nodeMH = TimMonHoc(dsMH.root, ltc->MAMH);
                QString tenMH = nodeMH ? QString::fromUtf8(nodeMH->mh.TENMH) : QString::fromUtf8(ltc->MAMH);
                int stc = nodeMH ? (nodeMH->mh.STCLT + nodeMH->mh.STCTH) : 0;

                MonRecord rec;
                rec.maMH = QString::fromUtf8(ltc->MAMH);
                rec.tenMH = tenMH;
                rec.tongSTC = stc;
                rec.nienKhoa = QString::fromUtf8(ltc->NienKhoa);
                rec.hocKy = ltc->HocKy;
                rec.nhom = ltc->Nhom;
                rec.diem = p->dk.DIEM;

                listDangKy.push_back(rec);

                if (p->dk.DIEM >= 0.0f) {
                    std::string smh = ltc->MAMH;
                    if (maxDiemTheoMon.find(smh) == maxDiemTheoMon.end() || p->dk.DIEM > maxDiemTheoMon[smh]) {
                        maxDiemTheoMon[smh] = p->dk.DIEM;
                        tcTheoMon[smh] = stc;
                    }
                }
            }
        }
    }

    int tongTCDaDK = 0;
    int tongTCTichLuy = 0;

    for (size_t i = 0; i < listDangKy.size(); i++) {
        const auto &rec = listDangKy[i];
        int row = tblDiem->rowCount();
        tblDiem->insertRow(row);

        tongTCDaDK += rec.tongSTC;
        if (rec.diem >= 4.0f) {
            tongTCTichLuy += rec.tongSTC;
        }

        auto itSTT = new QTableWidgetItem(QString::number(i + 1));
        auto itMa = new QTableWidgetItem(rec.maMH);
        auto itTen = new QTableWidgetItem(rec.tenMH);
        auto itSTC = new QTableWidgetItem(QString::number(rec.tongSTC));
        auto itNK = new QTableWidgetItem(rec.nienKhoa);
        auto itHK = new QTableWidgetItem(QString::number(rec.hocKy));
        auto itNhom = new QTableWidgetItem(QString::number(rec.nhom));

        QString strDiem = (rec.diem < 0.0f) ? "Chưa có" : QString::number(rec.diem, 'f', 1);
        auto itDiem = new QTableWidgetItem(strDiem);

        QString strDanhGia;
        if (rec.diem < 0.0f) {
            strDanhGia = "⏳ Đang học";
        } else if (rec.diem >= 4.0f) {
            strDanhGia = "✅ Đạt";
        } else {
            strDanhGia = "❌ Học lại";
        }
        auto itDanhGia = new QTableWidgetItem(strDanhGia);

        itSTT->setTextAlignment(Qt::AlignCenter);
        itMa->setTextAlignment(Qt::AlignCenter);
        itSTC->setTextAlignment(Qt::AlignCenter);
        itNK->setTextAlignment(Qt::AlignCenter);
        itHK->setTextAlignment(Qt::AlignCenter);
        itNhom->setTextAlignment(Qt::AlignCenter);
        itDiem->setTextAlignment(Qt::AlignCenter);
        itDanhGia->setTextAlignment(Qt::AlignCenter);

        itMa->setFont(QFont("Segoe UI", 9, QFont::Bold));
        itDiem->setFont(QFont("Segoe UI", 9, QFont::Bold));

        if (rec.diem >= 4.0f) {
            itDiem->setForeground(QBrush(QColor("#15803d")));
            itDanhGia->setForeground(QBrush(QColor("#15803d")));
        } else if (rec.diem >= 0.0f) {
            itDiem->setForeground(QBrush(QColor("#b91c1c")));
            itDanhGia->setForeground(QBrush(QColor("#b91c1c")));
        } else {
            itDiem->setForeground(QBrush(QColor("#475569")));
            itDanhGia->setForeground(QBrush(QColor("#475569")));
        }

        tblDiem->setItem(row, 0, itSTT);
        tblDiem->setItem(row, 1, itMa);
        tblDiem->setItem(row, 2, itTen);
        tblDiem->setItem(row, 3, itSTC);
        tblDiem->setItem(row, 4, itNK);
        tblDiem->setItem(row, 5, itHK);
        tblDiem->setItem(row, 6, itNhom);
        tblDiem->setItem(row, 7, itDiem);
        tblDiem->setItem(row, 8, itDanhGia);
    }

    // Tính điểm trung bình tích lũy theo số tín chỉ
    float tongDiemNhanTinChi = 0.0f;
    int tongTCHopLe = 0;
    for (const auto &pair : maxDiemTheoMon) {
        int stc = tcTheoMon[pair.first];
        tongDiemNhanTinChi += (pair.second * stc);
        tongTCHopLe += stc;
    }

    float gpa = (tongTCHopLe > 0) ? (tongDiemNhanTinChi / tongTCHopLe) : 0.0f;

    lblSummaryStats->setText(
        QString("📋 Đã đăng ký: <b>%1</b> môn (%2 tín chỉ) &nbsp;&nbsp;|&nbsp;&nbsp; "
                "🏆 Tích lũy (Điểm >= 4.0): <b style='color:#15803d;'>%3</b> tín chỉ &nbsp;&nbsp;|&nbsp;&nbsp; "
                "🎯 Điểm trung bình tích lũy toàn khóa (GPA Hệ 10): <b style='color:#740d18; font-size:16px;'>%4</b>")
            .arg(listDangKy.size())
            .arg(tongTCDaDK)
            .arg(tongTCTichLuy)
            .arg(QString::number(gpa, 'f', 2)));
}

void StudentDashboardView::loadDanhSachMonHoc() {
    tblMonHoc->setRowCount(0);
    const DS_MonHoc &dsMH = AppData::instance().dsMH;

    if (dsMH.n <= 0 || dsMH.root == nullptr) return;

    TreeMonHoc *arr = new TreeMonHoc[dsMH.n];
    int count = 0;
    InOrderTraversal(dsMH.root, arr, count);

    // Sắp xếp tăng dần theo Tên môn học (Câu f)
    QuickSortMH(arr, 0, count - 1);

    for (int i = 0; i < count; i++) {
        if (!arr[i]) continue;
        int row = tblMonHoc->rowCount();
        tblMonHoc->insertRow(row);

        QString maMH = QString::fromUtf8(arr[i]->mh.MAMH);
        QString tenMH = QString::fromUtf8(arr[i]->mh.TENMH);
        int lt = arr[i]->mh.STCLT;
        int th = arr[i]->mh.STCTH;
        int tong = lt + th;

        auto itSTT = new QTableWidgetItem(QString::number(row + 1));
        auto itMa = new QTableWidgetItem(maMH);
        auto itTen = new QTableWidgetItem(tenMH);
        auto itLT = new QTableWidgetItem(QString::number(lt));
        auto itTH = new QTableWidgetItem(QString::number(th));
        auto itTong = new QTableWidgetItem(QString::number(tong));

        itSTT->setTextAlignment(Qt::AlignCenter);
        itMa->setTextAlignment(Qt::AlignCenter);
        itLT->setTextAlignment(Qt::AlignCenter);
        itTH->setTextAlignment(Qt::AlignCenter);
        itTong->setTextAlignment(Qt::AlignCenter);

        itMa->setFont(QFont("Segoe UI", 9, QFont::Bold));
        itTong->setFont(QFont("Segoe UI", 9, QFont::Bold));

        tblMonHoc->setItem(row, 0, itSTT);
        tblMonHoc->setItem(row, 1, itMa);
        tblMonHoc->setItem(row, 2, itTen);
        tblMonHoc->setItem(row, 3, itLT);
        tblMonHoc->setItem(row, 4, itTH);
        tblMonHoc->setItem(row, 5, itTong);
    }

    delete[] arr;
}

void StudentDashboardView::onSearchMonHocChanged(const QString &text) {
    QString q = text.trimmed();
    for (int r = 0; r < tblMonHoc->rowCount(); r++) {
        if (q.isEmpty()) {
            tblMonHoc->setRowHidden(r, false);
            continue;
        }
        bool match = false;
        for (int c = 1; c <= 2; c++) {
            if (tblMonHoc->item(r, c) && tblMonHoc->item(r, c)->text().contains(q, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        tblMonHoc->setRowHidden(r, !match);
    }
}

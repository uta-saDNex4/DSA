#include "TabDangKy.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>
#include <QShortcut>
#include <cstring>
#include "Toast.h"
#include "lop.h"
#include "sinhvien.h"
#include "monhoc.h"
#include "loptc.h"
#include "dangky.h"

TabDangKy::TabDangKy(QWidget *parent) : QWidget(parent), svFound(false) {
    std::memset(currentMaSV, 0, sizeof(currentMaSV));
    setupUI();
    loadData();
}

void TabDangKy::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // ==========================================
    // TOP CARD: TRA CỨU SINH VIÊN & HỌC KỲ
    // ==========================================
    QGroupBox *grpTop = new QGroupBox("🔍 TRA CỨU SINH VIÊN & CHỌN HỌC KỲ ĐĂNG KÝ", this);
    grpTop->setStyleSheet("QGroupBox { font-weight: bold; color: #740d18; border: 1px solid #cbd5e1; border-radius: 8px; margin-top: 8px; padding-top: 14px; } "
                          "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }");
    QVBoxLayout *grpLayout = new QVBoxLayout(grpTop);
    grpLayout->setSpacing(10);

    QHBoxLayout *searchRow = new QHBoxLayout();
    QLabel *lblMa = new QLabel("Mã Sinh Viên:", grpTop);
    lblMa->setStyleSheet("font-weight: bold; color: #1e293b;");
    txtMaSV = new QLineEdit(grpTop);
    txtMaSV->setPlaceholderText("Nhập mã SV (VD: N21DCCN001)...");
    txtMaSV->setMinimumHeight(34);
    txtMaSV->setMaximumWidth(220);
    connect(txtMaSV, &QLineEdit::returnPressed, this, &TabDangKy::onSearchSinhVien);

    btnTimSV = new QPushButton("🔍 Tìm Sinh Viên", grpTop);
    btnTimSV->setMinimumHeight(34);
    btnTimSV->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 5px; padding: 0 16px;");
    connect(btnTimSV, &QPushButton::clicked, this, &TabDangKy::onSearchSinhVien);

    QLabel *lblNK = new QLabel("Niên khóa:", grpTop);
    lblNK->setStyleSheet("font-weight: bold; color: #1e293b;");
    cboNienKhoa = new QComboBox(grpTop);
    cboNienKhoa->setMinimumHeight(34);
    cboNienKhoa->setMinimumWidth(130);
    connect(cboNienKhoa, &QComboBox::currentTextChanged, this, &TabDangKy::onFilterChanged);

    QLabel *lblHK = new QLabel("Học kỳ:", grpTop);
    lblHK->setStyleSheet("font-weight: bold; color: #1e293b;");
    cboHocKy = new QComboBox(grpTop);
    cboHocKy->setMinimumHeight(34);
    cboHocKy->setMinimumWidth(90);
    cboHocKy->addItem("Học kỳ 1", 1);
    cboHocKy->addItem("Học kỳ 2", 2);
    cboHocKy->addItem("Học kỳ 3", 3);
    connect(cboHocKy, &QComboBox::currentIndexChanged, this, &TabDangKy::onFilterChanged);

    searchRow->addWidget(lblMa);
    searchRow->addWidget(txtMaSV);
    searchRow->addWidget(btnTimSV);
    searchRow->addSpacing(20);
    searchRow->addWidget(lblNK);
    searchRow->addWidget(cboNienKhoa);
    searchRow->addWidget(lblHK);
    searchRow->addWidget(cboHocKy);
    searchRow->addStretch();

    grpLayout->addLayout(searchRow);

    // Student Info Display Card
    lblThongTinSV = new QLabel("💡 Vui lòng nhập Mã Sinh Viên hoặc nhấp chọn sinh viên từ danh sách bên dưới để bắt đầu đăng ký tín chỉ.", grpTop);
    lblThongTinSV->setStyleSheet("background-color: #f8fafc; border: 1px dashed #cbd5e1; border-radius: 6px; padding: 8px 14px; font-size: 13px; color: #475569;");
    grpLayout->addWidget(lblThongTinSV);

    mainLayout->addWidget(grpTop);

    // ==========================================
    // CENTER SPLIT: LỚP ĐANG MỞ & BẢNG BÊN DƯỚI
    // ==========================================
    QSplitter *splitter = new QSplitter(Qt::Vertical, this);

    // TOP TABLE: Lớp tín chỉ đang mở
    QWidget *pnlLopMo = new QWidget(this);
    QVBoxLayout *layoutLopMo = new QVBoxLayout(pnlLopMo);
    layoutLopMo->setContentsMargins(0, 0, 0, 0);
    layoutLopMo->setSpacing(6);

    QHBoxLayout *hdrMo = new QHBoxLayout();
    QLabel *lblTitleMo = new QLabel("📋 DANH SÁCH CÁC LỚP TÍN CHỈ ĐANG MỞ TRONG HỌC KỲ", pnlLopMo);
    lblTitleMo->setStyleSheet("font-weight: bold; font-size: 13px; color: #1e293b;");
    QPushButton *btnDK = new QPushButton("✅ Đăng Ký Lớp Đã Chọn", pnlLopMo);
    btnDK->setMinimumHeight(32);
    btnDK->setStyleSheet("background-color: #059669; color: white; font-weight: bold; border-radius: 5px; padding: 0 16px;");
    btnDK->setToolTip("Nhấn nút này, hoặc nhấn phím Enter / Nhấp đúp chuột vào lớp để đăng ký");
    connect(btnDK, &QPushButton::clicked, this, &TabDangKy::onDangKyLTC);

    hdrMo->addWidget(lblTitleMo);
    hdrMo->addStretch();
    hdrMo->addWidget(btnDK);
    layoutLopMo->addLayout(hdrMo);

    tableLopMo = new QTableWidget(pnlLopMo);
    tableLopMo->setColumnCount(9);
    tableLopMo->setHorizontalHeaderLabels({
        "Mã LTC", "Mã MH", "Tên Môn Học", "STCLT", "STCTH", "Nhóm", "SV Max", "Đã ĐK", "Còn Trống"
    });
    tableLopMo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableLopMo->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableLopMo->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableLopMo->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableLopMo->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableLopMo->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableLopMo->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableLopMo->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tableLopMo->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    tableLopMo->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLopMo->setSelectionMode(QAbstractItemView::SingleSelection);
    tableLopMo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLopMo->setAlternatingRowColors(true);
    tableLopMo->verticalHeader()->setVisible(false);
    connect(tableLopMo, &QTableWidget::cellDoubleClicked, this, &TabDangKy::onDangKyLTC);

    QShortcut *scEnter1 = new QShortcut(QKeySequence(Qt::Key_Return), tableLopMo);
    connect(scEnter1, &QShortcut::activated, this, &TabDangKy::onDangKyLTC);
    QShortcut *scEnter2 = new QShortcut(QKeySequence(Qt::Key_Enter), tableLopMo);
    connect(scEnter2, &QShortcut::activated, this, &TabDangKy::onDangKyLTC);

    layoutLopMo->addWidget(tableLopMo);
    splitter->addWidget(pnlLopMo);

    // ==========================================
    // BOTTOM STACK: PAGE 0 = CHỌN SV | PAGE 1 = LỚP ĐÃ ĐĂNG KÝ
    // ==========================================
    stackBottom = new QStackedWidget(this);

    // PAGE 0: BẢNG GỢI Ý CHỌN SINH VIÊN KHI CHƯA TRA CỨU
    pnlChonSV = new QWidget(this);
    QVBoxLayout *layoutChonSV = new QVBoxLayout(pnlChonSV);
    layoutChonSV->setContentsMargins(0, 0, 0, 0);
    layoutChonSV->setSpacing(6);

    QHBoxLayout *hdrChonSV = new QHBoxLayout();
    QLabel *lblTitleChonSV = new QLabel("👥 DANH SÁCH SINH VIÊN (Nhấp đúp hoặc chọn dòng để bắt đầu đăng ký)", pnlChonSV);
    lblTitleChonSV->setStyleSheet("font-weight: bold; font-size: 13px; color: #1e293b;");

    txtFilterSV = new QLineEdit(pnlChonSV);
    txtFilterSV->setPlaceholderText("🔍 Lọc nhanh theo Mã SV, Họ Tên, Lớp...");
    txtFilterSV->setMinimumHeight(32);
    txtFilterSV->setMaximumWidth(280);
    connect(txtFilterSV, &QLineEdit::textChanged, this, &TabDangKy::onFilterSVChanged);

    btnChonSVSubmit = new QPushButton("👉 Chọn Sinh Viên Này", pnlChonSV);
    btnChonSVSubmit->setMinimumHeight(32);
    btnChonSVSubmit->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 5px; padding: 0 16px;");
    btnChonSVSubmit->setToolTip("Chọn sinh viên đang bôi đen để bắt đầu đăng ký tín chỉ");
    connect(btnChonSVSubmit, &QPushButton::clicked, this, [this]() {
        onSVSelectedFromTable(tableChonSV->currentRow(), 0);
    });

    hdrChonSV->addWidget(lblTitleChonSV);
    hdrChonSV->addStretch();
    hdrChonSV->addWidget(txtFilterSV);
    hdrChonSV->addWidget(btnChonSVSubmit);
    layoutChonSV->addLayout(hdrChonSV);

    tableChonSV = new QTableWidget(pnlChonSV);
    tableChonSV->setColumnCount(7);
    tableChonSV->setHorizontalHeaderLabels({
        "STT", "Mã SV", "Họ & Tên Đệm", "Tên", "Phái", "Lớp", "Số Điện Thoại"
    });
    tableChonSV->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableChonSV->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableChonSV->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableChonSV->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableChonSV->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableChonSV->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableChonSV->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableChonSV->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableChonSV->setSelectionMode(QAbstractItemView::SingleSelection);
    tableChonSV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableChonSV->setAlternatingRowColors(true);
    tableChonSV->verticalHeader()->setVisible(false);
    connect(tableChonSV, &QTableWidget::cellDoubleClicked, this, &TabDangKy::onSVSelectedFromTable);

    QShortcut *scEnterSV1 = new QShortcut(QKeySequence(Qt::Key_Return), tableChonSV);
    connect(scEnterSV1, &QShortcut::activated, this, [this]() {
        onSVSelectedFromTable(tableChonSV->currentRow(), 0);
    });
    QShortcut *scEnterSV2 = new QShortcut(QKeySequence(Qt::Key_Enter), tableChonSV);
    connect(scEnterSV2, &QShortcut::activated, this, [this]() {
        onSVSelectedFromTable(tableChonSV->currentRow(), 0);
    });

    layoutChonSV->addWidget(tableChonSV);
    stackBottom->addWidget(pnlChonSV);

    // PAGE 1: BẢNG LỚP SINH VIÊN ĐÃ ĐĂNG KÝ
    pnlDaDK = new QWidget(this);
    QVBoxLayout *layoutDaDK = new QVBoxLayout(pnlDaDK);
    layoutDaDK->setContentsMargins(0, 0, 0, 0);
    layoutDaDK->setSpacing(6);

    QHBoxLayout *hdrDaDK = new QHBoxLayout();
    QLabel *lblTitleDaDK = new QLabel("🎓 DANH SÁCH CÁC MÔN SINH VIÊN ĐÃ ĐĂNG KÝ TRONG KỲ NÀY", pnlDaDK);
    lblTitleDaDK->setStyleSheet("font-weight: bold; font-size: 13px; color: #1e293b;");
    
    lblTongTinChi = new QLabel("Tổng tín chỉ: <b>0</b>", pnlDaDK);
    lblTongTinChi->setStyleSheet("background-color: #eff6ff; color: #1d4ed8; border: 1px solid #bfdbfe; border-radius: 4px; padding: 4px 10px; font-size: 13px;");

    btnChonSVKhac = new QPushButton("🔄 Chọn SV Khác", pnlDaDK);
    btnChonSVKhac->setMinimumHeight(32);
    btnChonSVKhac->setStyleSheet("background-color: #f1f5f9; color: #334155; border: 1px solid #cbd5e1; font-weight: bold; border-radius: 5px; padding: 0 14px;");
    btnChonSVKhac->setToolTip("Quay lại danh sách sinh viên để chọn sinh viên khác");
    connect(btnChonSVKhac, &QPushButton::clicked, this, &TabDangKy::onChonSVKhac);

    btnHuyDK = new QPushButton("🗑️ Hủy Đăng Ký Môn Này", pnlDaDK);
    btnHuyDK->setMinimumHeight(32);
    btnHuyDK->setStyleSheet("background-color: #dc2626; color: white; font-weight: bold; border-radius: 5px; padding: 0 16px;");
    btnHuyDK->setToolTip("Nhấn nút này, hoặc nhấn phím Delete / Nhấp đúp chuột vào môn để hủy đăng ký");
    connect(btnHuyDK, &QPushButton::clicked, this, &TabDangKy::onHuyDangKyLTC);

    hdrDaDK->addWidget(lblTitleDaDK);
    hdrDaDK->addSpacing(16);
    hdrDaDK->addWidget(lblTongTinChi);
    hdrDaDK->addStretch();
    hdrDaDK->addWidget(btnChonSVKhac);
    hdrDaDK->addWidget(btnHuyDK);
    layoutDaDK->addLayout(hdrDaDK);

    tableLopDaDK = new QTableWidget(pnlDaDK);
    tableLopDaDK->setColumnCount(7);
    tableLopDaDK->setHorizontalHeaderLabels({
        "Mã LTC", "Mã MH", "Tên Môn Học", "STC LT", "STC TH", "Nhóm", "Điểm Thi"
    });
    tableLopDaDK->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableLopDaDK->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableLopDaDK->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableLopDaDK->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableLopDaDK->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableLopDaDK->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableLopDaDK->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableLopDaDK->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLopDaDK->setSelectionMode(QAbstractItemView::SingleSelection);
    tableLopDaDK->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLopDaDK->setAlternatingRowColors(true);
    tableLopDaDK->verticalHeader()->setVisible(false);
    connect(tableLopDaDK, &QTableWidget::cellDoubleClicked, this, &TabDangKy::onHuyDangKyLTC);

    QShortcut *scDel = new QShortcut(QKeySequence(Qt::Key_Delete), tableLopDaDK);
    connect(scDel, &QShortcut::activated, this, &TabDangKy::onHuyDangKyLTC);

    layoutDaDK->addWidget(tableLopDaDK);
    stackBottom->addWidget(pnlDaDK);

    stackBottom->setCurrentIndex(0); // Mặc định hiển thị Bảng Chọn SV
    splitter->addWidget(stackBottom);

    mainLayout->addWidget(splitter, 1);
}

void TabDangKy::capNhatDanhSachSinhVienChon() {
    if (!tableChonSV) return;
    tableChonSV->setUpdatesEnabled(false);
    tableChonSV->setRowCount(0);

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    QString filter = txtFilterSV ? txtFilterSV->text().trimmed().toLower() : "";

    int row = 0;
    int stt = 1;

    for (int i = 0; i < dsLop.n; ++i) {
        const char *malop = dsLop.nodes[i].MALOP;
        for (PTRSV p = dsLop.nodes[i].FirstSV; p != nullptr; p = p->next) {
            QString masv = QString::fromUtf8(p->sv.MASV);
            QString ho = QString::fromUtf8(p->sv.HO);
            QString ten = QString::fromUtf8(p->sv.TEN);
            QString phai = QString::fromUtf8(p->sv.PHAI);
            QString lop = QString::fromUtf8(malop);
            QString sodt = QString::fromUtf8(p->sv.SODT);

            if (!filter.isEmpty()) {
                QString fullSearch = masv + " " + ho + " " + ten + " " + lop + " " + sodt;
                if (!fullSearch.toLower().contains(filter)) {
                    continue;
                }
            }

            tableChonSV->insertRow(row);

            QTableWidgetItem *itemSTT = new QTableWidgetItem(QString::number(stt++));
            itemSTT->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *itemMa = new QTableWidgetItem(masv);
            itemMa->setTextAlignment(Qt::AlignCenter);
            itemMa->setData(Qt::UserRole, masv);

            QTableWidgetItem *itemHo = new QTableWidgetItem(ho);
            QTableWidgetItem *itemTen = new QTableWidgetItem(ten);
            itemTen->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *itemPhai = new QTableWidgetItem(phai);
            itemPhai->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *itemLop = new QTableWidgetItem(lop);
            itemLop->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *itemSDT = new QTableWidgetItem(sodt);
            itemSDT->setTextAlignment(Qt::AlignCenter);

            tableChonSV->setItem(row, 0, itemSTT);
            tableChonSV->setItem(row, 1, itemMa);
            tableChonSV->setItem(row, 2, itemHo);
            tableChonSV->setItem(row, 3, itemTen);
            tableChonSV->setItem(row, 4, itemPhai);
            tableChonSV->setItem(row, 5, itemLop);
            tableChonSV->setItem(row, 6, itemSDT);

            row++;
        }
    }
    tableChonSV->setUpdatesEnabled(true);
}

void TabDangKy::onSVSelectedFromTable(int row, int /*col*/) {
    if (row < 0) {
        row = tableChonSV->currentRow();
    }
    if (row < 0 || row >= tableChonSV->rowCount()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một sinh viên từ danh sách!");
        return;
    }
    QTableWidgetItem *itemMa = tableChonSV->item(row, 1);
    if (!itemMa) return;
    QString masv = itemMa->data(Qt::UserRole).toString();
    txtMaSV->setText(masv);
    onSearchSinhVien();
}

void TabDangKy::onChonSVKhac() {
    svFound = false;
    std::memset(currentMaSV, 0, sizeof(currentMaSV));
    txtMaSV->clear();
    lblThongTinSV->setText("💡 Vui lòng nhập Mã Sinh Viên hoặc nhấp chọn sinh viên từ danh sách bên dưới để bắt đầu đăng ký tín chỉ.");
    lblThongTinSV->setStyleSheet("background-color: #f8fafc; border: 1px dashed #cbd5e1; border-radius: 6px; padding: 8px 14px; font-size: 13px; color: #475569;");
    stackBottom->setCurrentIndex(0);
    capNhatDanhSachSinhVienChon();
    capNhatDanhSachDaDangKy();
    if (txtFilterSV) {
        txtFilterSV->clear();
        txtFilterSV->setFocus();
    }
}

void TabDangKy::onFilterSVChanged(const QString &/*text*/) {
    capNhatDanhSachSinhVienChon();
}

void TabDangKy::capNhatDanhSachNienKhoa() {
    QString current = cboNienKhoa->currentText();
    cboNienKhoa->blockSignals(true);
    cboNienKhoa->clear();

    DS_LopTC &dsltc = AppData::instance().dsLTC;
    for (int i = 0; i < dsltc.n; ++i) {
        if (dsltc.nodes[i] != nullptr) {
            QString nk = QString::fromUtf8(dsltc.nodes[i]->NienKhoa);
            if (cboNienKhoa->findText(nk) == -1) {
                cboNienKhoa->addItem(nk);
            }
        }
    }

    if (cboNienKhoa->count() == 0) {
        cboNienKhoa->addItem("2025-2026");
    }

    int idx = cboNienKhoa->findText(current);
    if (idx >= 0) cboNienKhoa->setCurrentIndex(idx);
    else cboNienKhoa->setCurrentIndex(0);

    cboNienKhoa->blockSignals(false);
}

void TabDangKy::loadData() {
    capNhatDanhSachNienKhoa();
    capNhatDanhSachSinhVienChon();
    capNhatDanhSachLopMo();
    capNhatDanhSachDaDangKy();

    if (svFound) {
        stackBottom->setCurrentIndex(1);
    } else {
        stackBottom->setCurrentIndex(0);
    }

    if (svFound && tableLopMo->rowCount() > 0) {
        tableLopMo->selectRow(0);
        tableLopMo->setFocus();
    }
}

void TabDangKy::onFilterChanged() {
    capNhatDanhSachLopMo();
    capNhatDanhSachDaDangKy();

    if (svFound && tableLopMo->rowCount() > 0) {
        tableLopMo->selectRow(0);
        tableLopMo->setFocus();
    }
}

void TabDangKy::onSearchSinhVien() {
    QString masvInput = txtMaSV->text().trimmed().toUpper();
    txtMaSV->setText(masvInput);

    if (masvInput.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập Mã Sinh Viên!");
        stackBottom->setCurrentIndex(0);
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    LOPSV *lopChuaSV = nullptr;
    PTRSV pSV = TimSVToanTruong(dsLop, masvInput.toUtf8().constData(), lopChuaSV);

    if (pSV != nullptr && lopChuaSV != nullptr) {
        std::strcpy(currentMaSV, pSV->sv.MASV);
        svFound = true;
        lblThongTinSV->setText(
            QString("👤 <b>Mã SV:</b> <span style='color:#be3142; font-size:14px;'>%1</span> &nbsp;&nbsp;|&nbsp;&nbsp; "
                    "<b>Họ Tên:</b> %2 %3 &nbsp;&nbsp;|&nbsp;&nbsp; "
                    "<b>Phái:</b> %4 &nbsp;&nbsp;|&nbsp;&nbsp; "
                    "<b>Lớp:</b> %5 &nbsp;&nbsp;|&nbsp;&nbsp; "
                    "<b>SĐT:</b> %6")
                .arg(pSV->sv.MASV)
                .arg(QString::fromUtf8(pSV->sv.HO))
                .arg(QString::fromUtf8(pSV->sv.TEN))
                .arg(QString::fromUtf8(pSV->sv.PHAI))
                .arg(QString::fromUtf8(lopChuaSV->MALOP))
                .arg(pSV->sv.SODT)
        );
        lblThongTinSV->setStyleSheet("background-color: #f0fdf4; border: 1px solid #86efac; border-radius: 6px; padding: 8px 14px; font-size: 13px; color: #166534;");
        stackBottom->setCurrentIndex(1); // Chuyển sang bảng môn đã đăng ký
    } else {
        svFound = false;
        std::memset(currentMaSV, 0, sizeof(currentMaSV));
        lblThongTinSV->setText(QString("❌ Không tìm thấy sinh viên có mã: <b>%1</b> trong hệ thống!").arg(masvInput));
        lblThongTinSV->setStyleSheet("background-color: #fef2f2; border: 1px solid #fca5a5; border-radius: 6px; padding: 8px 14px; font-size: 13px; color: #991b1b;");
        stackBottom->setCurrentIndex(0); // Trở lại bảng chọn sinh viên
    }

    capNhatDanhSachLopMo();
    capNhatDanhSachDaDangKy();

    if (svFound && tableLopMo->rowCount() > 0) {
        tableLopMo->selectRow(0);
        tableLopMo->setFocus();
    }
}

void TabDangKy::capNhatDanhSachLopMo() {
    tableLopMo->setRowCount(0);

    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_MonHoc &dsMH = AppData::instance().dsMH;

    QString nk = cboNienKhoa->currentText();
    int hk = cboHocKy->currentData().toInt();

    int row = 0;
    for (int i = 0; i < dsltc.n; ++i) {
        if (dsltc.nodes[i] == nullptr) continue;
        LopTinChi *ltc = dsltc.nodes[i];

        if (ltc->HuyLop) continue; // Lớp đã hủy không hiển thị để đăng ký
        if (strcmp(ltc->NienKhoa, nk.toUtf8().constData()) != 0) continue;
        if (ltc->HocKy != hk) continue;

        int soDk = DemSVDangKyHopLe(ltc->FirstDK);
        int conTrong = ltc->SoSVMax - soDk;
        if (conTrong < 0) conTrong = 0;

        TreeMonHoc mhNode = TimMonHoc(dsMH.root, ltc->MAMH);
        QString tenMH = mhNode ? QString::fromUtf8(mhNode->mh.TENMH) : "(Không tìm thấy)";
        int stclt = mhNode ? mhNode->mh.STCLT : 0;
        int stcth = mhNode ? mhNode->mh.STCTH : 0;

        tableLopMo->insertRow(row);

        QTableWidgetItem *itemMaLTC = new QTableWidgetItem(QString::number(ltc->MALOPTC));
        itemMaLTC->setTextAlignment(Qt::AlignCenter);
        itemMaLTC->setData(Qt::UserRole, ltc->MALOPTC);

        QTableWidgetItem *itemMaMH = new QTableWidgetItem(QString::fromUtf8(ltc->MAMH));
        itemMaMH->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemTenMH = new QTableWidgetItem(tenMH);

        QTableWidgetItem *itemSTCLT = new QTableWidgetItem(QString::number(stclt));
        itemSTCLT->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemSTCTH = new QTableWidgetItem(QString::number(stcth));
        itemSTCTH->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemNhom = new QTableWidgetItem(QString::number(ltc->Nhom));
        itemNhom->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemMax = new QTableWidgetItem(QString::number(ltc->SoSVMax));
        itemMax->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemDk = new QTableWidgetItem(QString::number(soDk));
        itemDk->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemTrong = new QTableWidgetItem(QString::number(conTrong));
        itemTrong->setTextAlignment(Qt::AlignCenter);
        if (conTrong == 0) {
            itemTrong->setForeground(QBrush(QColor("#dc2626")));
            itemTrong->setText("Hết chỗ (0)");
        } else {
            itemTrong->setForeground(QBrush(QColor("#16a34a")));
        }

        tableLopMo->setItem(row, 0, itemMaLTC);
        tableLopMo->setItem(row, 1, itemMaMH);
        tableLopMo->setItem(row, 2, itemTenMH);
        tableLopMo->setItem(row, 3, itemSTCLT);
        tableLopMo->setItem(row, 4, itemSTCTH);
        tableLopMo->setItem(row, 5, itemNhom);
        tableLopMo->setItem(row, 6, itemMax);
        tableLopMo->setItem(row, 7, itemDk);
        tableLopMo->setItem(row, 8, itemTrong);

        row++;
    }

    if (tableLopMo->rowCount() > 0 && tableLopMo->currentRow() < 0) {
        tableLopMo->selectRow(0);
    }
}

void TabDangKy::capNhatDanhSachDaDangKy() {
    tableLopDaDK->setRowCount(0);
    lblTongTinChi->setText("Tổng tín chỉ: <b>0</b>");

    if (!svFound || std::strlen(currentMaSV) == 0) {
        return;
    }

    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_MonHoc &dsMH = AppData::instance().dsMH;

    QString nk = cboNienKhoa->currentText();
    int hk = cboHocKy->currentData().toInt();

    int row = 0;
    int tongTC = 0;

    for (int i = 0; i < dsltc.n; ++i) {
        if (dsltc.nodes[i] == nullptr) continue;
        LopTinChi *ltc = dsltc.nodes[i];

        if (strcmp(ltc->NienKhoa, nk.toUtf8().constData()) != 0) continue;
        if (ltc->HocKy != hk) continue;

        PTR_DangKy dkNode = TimDangKy(ltc->FirstDK, currentMaSV);
        if (dkNode != nullptr && !dkNode->dk.HuyDangKy) {
            TreeMonHoc mhNode = TimMonHoc(dsMH.root, ltc->MAMH);
            QString tenMH = mhNode ? QString::fromUtf8(mhNode->mh.TENMH) : "(Không tìm thấy)";
            int stclt = mhNode ? mhNode->mh.STCLT : 0;
            int stcth = mhNode ? mhNode->mh.STCTH : 0;
            tongTC += (stclt + stcth);

            tableLopDaDK->insertRow(row);

            QTableWidgetItem *itemMaLTC = new QTableWidgetItem(QString::number(ltc->MALOPTC));
            itemMaLTC->setTextAlignment(Qt::AlignCenter);
            itemMaLTC->setData(Qt::UserRole, ltc->MALOPTC);

            QTableWidgetItem *itemMaMH = new QTableWidgetItem(QString::fromUtf8(ltc->MAMH));
            itemMaMH->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *itemTenMH = new QTableWidgetItem(tenMH);

            QTableWidgetItem *itemSTCLT = new QTableWidgetItem(QString::number(stclt));
            itemSTCLT->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *itemSTCTH = new QTableWidgetItem(QString::number(stcth));
            itemSTCTH->setTextAlignment(Qt::AlignCenter);

            QTableWidgetItem *itemNhom = new QTableWidgetItem(QString::number(ltc->Nhom));
            itemNhom->setTextAlignment(Qt::AlignCenter);

            QString strDiem = (dkNode->dk.DIEM < 0) ? "Chưa nhập" : QString::number(dkNode->dk.DIEM, 'f', 1);
            QTableWidgetItem *itemDiem = new QTableWidgetItem(strDiem);
            itemDiem->setTextAlignment(Qt::AlignCenter);
            if (dkNode->dk.DIEM >= 0) {
                itemDiem->setForeground(QBrush(QColor("#0284c7")));
                itemDiem->setData(Qt::UserRole + 1, dkNode->dk.DIEM);
            } else {
                itemDiem->setData(Qt::UserRole + 1, -1.0f);
            }

            tableLopDaDK->setItem(row, 0, itemMaLTC);
            tableLopDaDK->setItem(row, 1, itemMaMH);
            tableLopDaDK->setItem(row, 2, itemTenMH);
            tableLopDaDK->setItem(row, 3, itemSTCLT);
            tableLopDaDK->setItem(row, 4, itemSTCTH);
            tableLopDaDK->setItem(row, 5, itemNhom);
            tableLopDaDK->setItem(row, 6, itemDiem);

            row++;
        }
    }

    lblTongTinChi->setText(QString("Tổng tín chỉ đã ĐK: <b style='color:#be3142; font-size:14px;'>%1</b> STC (%2 môn)").arg(tongTC).arg(row));
}

void TabDangKy::onDangKyLTC() {
    if (!svFound || std::strlen(currentMaSV) == 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng tra cứu hoặc chọn sinh viên trước khi thực hiện đăng ký!");
        txtMaSV->setFocus();
        return;
    }

    int curRow = tableLopMo->currentRow();
    if (curRow < 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một lớp tín chỉ trong danh sách đang mở để đăng ký!");
        return;
    }

    int maLTC = tableLopMo->item(curRow, 0)->data(Qt::UserRole).toInt();
    DS_LopTC &dsltc = AppData::instance().dsLTC;

    int idx = TimLopTCTheoMa(dsltc, maLTC);
    if (idx == -1 || dsltc.nodes[idx] == nullptr) return;

    LopTinChi *ltc = dsltc.nodes[idx];

    // Kiểm tra lớp có bị hủy không
    if (ltc->HuyLop) {
        QMessageBox::warning(this, "Lỗi đăng ký", "Lớp tín chỉ này đã bị hủy, không thể đăng ký!");
        return;
    }

    // Kiểm tra SV đã đăng ký chính lớp này chưa
    PTR_DangKy dkCurrent = TimDangKy(ltc->FirstDK, currentMaSV);
    if (dkCurrent != nullptr && !dkCurrent->dk.HuyDangKy) {
        QMessageBox::information(this, "Thông báo", "Sinh viên đã đăng ký lớp tín chỉ này rồi!");
        return;
    }

    // Kiểm tra sinh viên đã đăng ký môn học này ở nhóm khác trong cùng kỳ chưa (Theo đề bài)
    if (DaDangKyMonHocTrongKy(dsltc, currentMaSV, ltc->NienKhoa, ltc->HocKy, ltc->MAMH)) {
        QMessageBox::warning(this, "Trùng môn học trong kỳ",
                             QString("Sinh viên đã đăng ký môn học '%1' ở một nhóm khác trong cùng Niên khóa %2, Học kỳ %3!\n"
                                     "Theo quy định, một học kỳ sinh viên chỉ được đăng ký 1 nhóm cho mỗi môn học.")
                                 .arg(ltc->MAMH).arg(ltc->NienKhoa).arg(ltc->HocKy));
        return;
    }

    // Kiểm tra sĩ số lớp đã đầy chưa
    int soDk = DemSVDangKyHopLe(ltc->FirstDK);
    if (soDk >= ltc->SoSVMax) {
        QMessageBox::warning(this, "Lớp đã đầy",
                             QString("Lớp tín chỉ này đã đủ sĩ số tối đa (%1/%2)! Không thể đăng ký thêm.")
                                 .arg(soDk).arg(ltc->SoSVMax));
        return;
    }

    // Thực hiện đăng ký (hoặc phục hồi nếu trước đó đã hủy)
    bool success = false;
    if (dkCurrent != nullptr && dkCurrent->dk.HuyDangKy) {
        success = PhucHoiDangKySV(ltc->FirstDK, currentMaSV);
    } else {
        success = ThemDangKy(ltc->FirstDK, currentMaSV);
    }

    if (success) {
        AppData::instance().saveAll();
        capNhatDanhSachLopMo();
        capNhatDanhSachDaDangKy();
        Toast::showSuccess(this, QString("Sinh viên %1 đã đăng ký thành công lớp %2! (Đã tự động lưu)").arg(currentMaSV).arg(maLTC));
    } else {
        QMessageBox::critical(this, "Lỗi", "Đăng ký thất bại!");
    }
}

void TabDangKy::onHuyDangKyLTC() {
    if (!svFound || std::strlen(currentMaSV) == 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng tra cứu sinh viên trước!");
        return;
    }

    int curRow = tableLopDaDK->currentRow();
    if (curRow < 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một môn học trong danh sách đã đăng ký để hủy!");
        return;
    }

    int maLTC = tableLopDaDK->item(curRow, 0)->data(Qt::UserRole).toInt();
    float diem = tableLopDaDK->item(curRow, 6)->data(Qt::UserRole + 1).toFloat();

    if (diem >= 0.0f) {
        QMessageBox::warning(this, "Từ chối hủy đăng ký",
                             "Lớp tín chỉ này ĐÃ CÓ ĐIỂM THI!\nTheo quy định đề tài, không thể hủy đăng ký môn học đã có điểm.");
        return;
    }

    DS_LopTC &dsltc = AppData::instance().dsLTC;
    int idx = TimLopTCTheoMa(dsltc, maLTC);
    if (idx == -1 || dsltc.nodes[idx] == nullptr) return;

    auto ret = QMessageBox::question(this, "Xác nhận hủy đăng ký",
                                     QString("Bạn có chắc chắn muốn hủy đăng ký lớp tín chỉ mã %1 cho sinh viên %2 không?")
                                         .arg(maLTC).arg(currentMaSV),
                                     QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        if (HuyDangKySV(dsltc.nodes[idx]->FirstDK, currentMaSV)) {
            AppData::instance().saveAll();
            capNhatDanhSachLopMo();
            capNhatDanhSachDaDangKy();
            Toast::showSuccess(this, "Đã hủy đăng ký môn học thành công! (Đã tự động lưu)");
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể hủy đăng ký môn học này!");
        }
    }
}

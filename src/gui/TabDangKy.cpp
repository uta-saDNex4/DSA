#include "TabDangKy.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSplitter>
#include <cstring>
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
    QGroupBox *grpTop = new QGroupBox("🔍 TRA CỨU SINH VIÊN & CHỌN HỌC KỲ ĐĂNG KÝ (CÂU G)", this);
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
    lblThongTinSV = new QLabel("Vui lòng nhập Mã Sinh Viên và nhấn 'Tìm Sinh Viên' để bắt đầu đăng ký tín chỉ.", grpTop);
    lblThongTinSV->setStyleSheet("background-color: #f8fafc; border: 1px dashed #cbd5e1; border-radius: 6px; padding: 8px 14px; font-size: 13px; color: #475569;");
    grpLayout->addWidget(lblThongTinSV);

    mainLayout->addWidget(grpTop);

    // ==========================================
    // CENTER SPLIT: LỚP ĐANG MỞ & LỚP ĐÃ ĐĂNG KÝ
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
    QPushButton *btnDK = new QPushButton("✅ [Enter] Đăng Ký Lớp Đã Chọn", pnlLopMo);
    btnDK->setMinimumHeight(32);
    btnDK->setStyleSheet("background-color: #059669; color: white; font-weight: bold; border-radius: 5px; padding: 0 16px;");
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

    layoutLopMo->addWidget(tableLopMo);
    splitter->addWidget(pnlLopMo);

    // BOTTOM TABLE: Lớp SV đã đăng ký
    QWidget *pnlDaDK = new QWidget(this);
    QVBoxLayout *layoutDaDK = new QVBoxLayout(pnlDaDK);
    layoutDaDK->setContentsMargins(0, 0, 0, 0);
    layoutDaDK->setSpacing(6);

    QHBoxLayout *hdrDaDK = new QHBoxLayout();
    QLabel *lblTitleDaDK = new QLabel("🎓 DANH SÁCH CÁC MÔN SINH VIÊN ĐÃ ĐĂNG KÝ TRONG KỲ NÀY", pnlDaDK);
    lblTitleDaDK->setStyleSheet("font-weight: bold; font-size: 13px; color: #1e293b;");
    
    lblTongTinChi = new QLabel("Tổng tín chỉ: <b>0</b>", pnlDaDK);
    lblTongTinChi->setStyleSheet("background-color: #eff6ff; color: #1d4ed8; border: 1px solid #bfdbfe; border-radius: 4px; padding: 4px 10px; font-size: 13px;");

    QPushButton *btnHuyDK = new QPushButton("🗑️ [Del] Hủy Đăng Ký Môn Này", pnlDaDK);
    btnHuyDK->setMinimumHeight(32);
    btnHuyDK->setStyleSheet("background-color: #dc2626; color: white; font-weight: bold; border-radius: 5px; padding: 0 16px;");
    connect(btnHuyDK, &QPushButton::clicked, this, &TabDangKy::onHuyDangKyLTC);

    hdrDaDK->addWidget(lblTitleDaDK);
    hdrDaDK->addSpacing(16);
    hdrDaDK->addWidget(lblTongTinChi);
    hdrDaDK->addStretch();
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

    layoutDaDK->addWidget(tableLopDaDK);
    splitter->addWidget(pnlDaDK);

    mainLayout->addWidget(splitter, 1);
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
        cboNienKhoa->addItem("2023-2024");
    }

    int idx = cboNienKhoa->findText(current);
    if (idx >= 0) cboNienKhoa->setCurrentIndex(idx);
    else cboNienKhoa->setCurrentIndex(0);

    cboNienKhoa->blockSignals(false);
}

void TabDangKy::loadData() {
    capNhatDanhSachNienKhoa();
    capNhatDanhSachLopMo();
    capNhatDanhSachDaDangKy();
}

void TabDangKy::onFilterChanged() {
    capNhatDanhSachLopMo();
    capNhatDanhSachDaDangKy();
}

void TabDangKy::onSearchSinhVien() {
    QString masvInput = txtMaSV->text().trimmed().toUpper();
    txtMaSV->setText(masvInput);

    if (masvInput.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng nhập Mã Sinh Viên!");
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
    } else {
        svFound = false;
        std::memset(currentMaSV, 0, sizeof(currentMaSV));
        lblThongTinSV->setText(QString("❌ Không tìm thấy sinh viên có mã: <b>%1</b> trong hệ thống!").arg(masvInput));
        lblThongTinSV->setStyleSheet("background-color: #fef2f2; border: 1px solid #fca5a5; border-radius: 6px; padding: 8px 14px; font-size: 13px; color: #991b1b;");
    }

    capNhatDanhSachLopMo();
    capNhatDanhSachDaDangKy();
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
        QMessageBox::warning(this, "Thông báo", "Vui lòng tra cứu sinh viên trước khi thực hiện đăng ký!");
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
        QMessageBox::information(this, "Đăng ký thành công",
                                 QString("Sinh viên %1 đã đăng ký thành công vào Lớp Tín Chỉ mã: %2!")
                                     .arg(currentMaSV).arg(maLTC));
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
            QMessageBox::information(this, "Thành công", "Đã hủy đăng ký môn học thành công!");
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể hủy đăng ký môn học này!");
        }
    }
}

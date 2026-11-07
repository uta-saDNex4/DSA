#include "TabDiem.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <cstring>
#include "lop.h"
#include "sinhvien.h"
#include "monhoc.h"
#include "loptc.h"
#include "dangky.h"

TabDiem::TabDiem(QWidget *parent) : QWidget(parent), currentMaLTC_Nhap(-1) {
    setupUI();
    loadData();
}

void TabDiem::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    subTabs = new QTabWidget(this);
    subTabs->setStyleSheet(
        "QTabWidget::pane { border: 1px solid #e2e8f0; border-radius: 8px; background: white; padding: 12px; } "
        "QTabBar::tab { background: #f1f5f9; color: #475569; padding: 8px 18px; font-weight: bold; border-top-left-radius: 6px; border-top-right-radius: 6px; margin-right: 4px; } "
        "QTabBar::tab:selected { background: #be3142; color: white; }"
    );

    subTabs->addTab(createSubTabNhapDiem(), "✍️ Nhập Điểm Thi (Câu i)");
    subTabs->addTab(createSubTabBangDiemMH(), "📋 Bảng Điểm Môn Học (Câu j)");
    subTabs->addTab(createSubTabDTBKhoaHoc(), "🎯 Điểm Trung Bình Khóa Học (Câu k)");
    subTabs->addTab(createSubTabTongKet(), "📊 Bảng Điểm Tổng Kết Ma Trận (Câu l)");

    mainLayout->addWidget(subTabs);
}

// ====================================================================
// SUB-TAB 1: NHẬP ĐIỂM THI (CÂU I)
// ====================================================================
QWidget* TabDiem::createSubTabNhapDiem() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    QLabel *lblNK = new QLabel("Niên khóa:", tab);
    lblNK->setStyleSheet("font-weight: bold;");
    cboNK_Nhap = new QComboBox(tab);
    cboNK_Nhap->setMinimumWidth(120);

    QLabel *lblHK = new QLabel("Học kỳ:", tab);
    lblHK->setStyleSheet("font-weight: bold;");
    cboHK_Nhap = new QComboBox(tab);
    cboHK_Nhap->addItem("1", 1);
    cboHK_Nhap->addItem("2", 2);
    cboHK_Nhap->addItem("3", 3);

    QLabel *lblMH = new QLabel("Môn học:", tab);
    lblMH->setStyleSheet("font-weight: bold;");
    cboMH_Nhap = new QComboBox(tab);
    cboMH_Nhap->setMinimumWidth(220);

    QLabel *lblNhom = new QLabel("Nhóm:", tab);
    lblNhom->setStyleSheet("font-weight: bold;");
    cboNhom_Nhap = new QComboBox(tab);
    for (int i = 1; i <= 20; ++i) cboNhom_Nhap->addItem(QString::number(i), i);

    QPushButton *btnTai = new QPushButton("🔍 Tải Lớp & DSSV", tab);
    btnTai->setMinimumHeight(32);
    btnTai->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 4px; padding: 0 14px;");
    connect(btnTai, &QPushButton::clicked, this, &TabDiem::onTaiDanhSachNhapDiem);

    QPushButton *btnLuu = new QPushButton("💾 [F2] Lưu Điểm Thi", tab);
    btnLuu->setMinimumHeight(32);
    btnLuu->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");
    connect(btnLuu, &QPushButton::clicked, this, &TabDiem::onLuuDiemThi);

    filterLayout->addWidget(lblNK);
    filterLayout->addWidget(cboNK_Nhap);
    filterLayout->addWidget(lblHK);
    filterLayout->addWidget(cboHK_Nhap);
    filterLayout->addWidget(lblMH);
    filterLayout->addWidget(cboMH_Nhap, 1);
    filterLayout->addWidget(lblNhom);
    filterLayout->addWidget(cboNhom_Nhap);
    filterLayout->addWidget(btnTai);
    filterLayout->addWidget(btnLuu);

    layout->addLayout(filterLayout);

    lblInfoLTC_Nhap = new QLabel("Vui lòng chọn thông tin lớp tín chỉ và nhấn 'Tải Lớp & DSSV'.", tab);
    lblInfoLTC_Nhap->setStyleSheet("color: #64748b; font-size: 13px; font-weight: 500;");
    layout->addWidget(lblInfoLTC_Nhap);

    tblNhapDiem = new QTableWidget(tab);
    tblNhapDiem->setColumnCount(6);
    tblNhapDiem->setHorizontalHeaderLabels({"STT", "Mã SV", "Họ", "Tên", "Mã Lớp", "Điểm Thi (0.0 - 10.0)"});
    tblNhapDiem->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblNhapDiem->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tblNhapDiem->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tblNhapDiem->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tblNhapDiem->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tblNhapDiem->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tblNhapDiem->setAlternatingRowColors(true);
    tblNhapDiem->verticalHeader()->setVisible(false);

    layout->addWidget(tblNhapDiem);

    return tab;
}

void TabDiem::onTaiDanhSachNhapDiem() {
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_LOPSV &dsLop = AppData::instance().dsLop;

    QString nk = cboNK_Nhap->currentText();
    int hk = cboHK_Nhap->currentData().toInt();
    QString mamh = cboMH_Nhap->currentData().toString();
    int nhom = cboNhom_Nhap->currentData().toInt();

    if (mamh.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn môn học!");
        return;
    }

    int idx = TimLopTCTrung(dsltc, nk.toUtf8().constData(), hk, mamh.toUtf8().constData(), nhom);
    if (idx == -1 || dsltc.nodes[idx] == nullptr) {
        currentMaLTC_Nhap = -1;
        tblNhapDiem->setRowCount(0);
        lblInfoLTC_Nhap->setText(QString("❌ Không tìm thấy Lớp Tín Chỉ nào với Niên khóa %1, Học kỳ %2, Môn %3, Nhóm %4!")
                                     .arg(nk).arg(hk).arg(mamh).arg(nhom));
        lblInfoLTC_Nhap->setStyleSheet("color: #dc2626; font-weight: bold;");
        return;
    }

    LopTinChi *ltc = dsltc.nodes[idx];
    currentMaLTC_Nhap = ltc->MALOPTC;

    if (ltc->HuyLop) {
        tblNhapDiem->setRowCount(0);
        lblInfoLTC_Nhap->setText(QString("⚠️ Lớp Tín Chỉ mã %1 đã bị HỦY, không thể nhập điểm!").arg(currentMaLTC_Nhap));
        lblInfoLTC_Nhap->setStyleSheet("color: #ea580c; font-weight: bold;");
        return;
    }

    SinhVienDiem dsSV[MAX_SV_DANGKY];
    int count = LayDSSVDangKyLTC(dsltc, dsLop, currentMaLTC_Nhap, dsSV, MAX_SV_DANGKY);

    lblInfoLTC_Nhap->setText(
        QString("✅ <b>Mã LTC: %1</b> | Môn: %2 | Niên khóa: %3 - HK%4 - Nhóm %5 | Sĩ số ĐK: <b>%6</b> sinh viên")
            .arg(currentMaLTC_Nhap).arg(mamh).arg(nk).arg(hk).arg(nhom).arg(count));
    lblInfoLTC_Nhap->setStyleSheet("color: #166534; font-size: 13px; font-weight: 500;");

    tblNhapDiem->setRowCount(count);
    for (int i = 0; i < count; ++i) {
        QTableWidgetItem *itemSTT = new QTableWidgetItem(QString::number(i + 1));
        itemSTT->setTextAlignment(Qt::AlignCenter);
        itemSTT->setFlags(itemSTT->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *itemMa = new QTableWidgetItem(QString::fromUtf8(dsSV[i].MASV));
        itemMa->setTextAlignment(Qt::AlignCenter);
        itemMa->setFlags(itemMa->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *itemHo = new QTableWidgetItem(QString::fromUtf8(dsSV[i].HO));
        itemHo->setFlags(itemHo->flags() & ~Qt::ItemIsEditable);

        QTableWidgetItem *itemTen = new QTableWidgetItem(QString::fromUtf8(dsSV[i].TEN));
        itemTen->setTextAlignment(Qt::AlignCenter);
        itemTen->setFlags(itemTen->flags() & ~Qt::ItemIsEditable);

        // Lấy mã lớp của sinh viên
        LOPSV *lopChuaSV = nullptr;
        PTRSV pSV = TimSVToanTruong(dsLop, dsSV[i].MASV, lopChuaSV);
        QString maLopSV = lopChuaSV ? QString::fromUtf8(lopChuaSV->MALOP) : "";
        QTableWidgetItem *itemLop = new QTableWidgetItem(maLopSV);
        itemLop->setTextAlignment(Qt::AlignCenter);
        itemLop->setFlags(itemLop->flags() & ~Qt::ItemIsEditable);

        // DoubleSpinBox cho điểm thi (0.0 - 10.0, bước 0.1, -1 nếu chưa nhập)
        QDoubleSpinBox *spinDiem = new QDoubleSpinBox(tblNhapDiem);
        spinDiem->setRange(-1.0, 10.0);
        spinDiem->setSingleStep(0.25);
        spinDiem->setDecimals(1);
        spinDiem->setSpecialValueText("Chưa nhập (-1)");
        spinDiem->setValue(dsSV[i].DIEM < 0 ? -1.0 : dsSV[i].DIEM);
        spinDiem->setAlignment(Qt::AlignCenter);
        spinDiem->setStyleSheet("font-weight: bold; color: #be3142;");

        tblNhapDiem->setItem(i, 0, itemSTT);
        tblNhapDiem->setItem(i, 1, itemMa);
        tblNhapDiem->setItem(i, 2, itemHo);
        tblNhapDiem->setItem(i, 3, itemTen);
        tblNhapDiem->setItem(i, 4, itemLop);
        tblNhapDiem->setCellWidget(i, 5, spinDiem);
    }
}

void TabDiem::onLuuDiemThi() {
    if (currentMaLTC_Nhap <= 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng tải danh sách lớp tín chỉ trước khi lưu điểm!");
        return;
    }

    DS_LopTC &dsltc = AppData::instance().dsLTC;
    int rowCount = tblNhapDiem->rowCount();

    int soLuongLuu = 0;
    for (int i = 0; i < rowCount; ++i) {
        QString masv = tblNhapDiem->item(i, 1)->text().trimmed();
        QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox*>(tblNhapDiem->cellWidget(i, 5));
        if (spin) {
            float diem = spin->value();
            CapNhatDiemSinhVien(dsltc, currentMaLTC_Nhap, masv.toUtf8().constData(), diem);
            soLuongLuu++;
        }
    }

    AppData::instance().saveAll();
    QMessageBox::information(this, "Thành công (Câu i)",
                             QString("Đã lưu bảng điểm thành công cho %1 sinh viên của Lớp Tín Chỉ mã %2!")
                                 .arg(soLuongLuu).arg(currentMaLTC_Nhap));
}

// ====================================================================
// SUB-TAB 2: BẢNG ĐIỂM MÔN HỌC (CÂU J)
// ====================================================================
QWidget* TabDiem::createSubTabBangDiemMH() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    QLabel *lblNK = new QLabel("Niên khóa:", tab);
    lblNK->setStyleSheet("font-weight: bold;");
    cboNK_BD = new QComboBox(tab);
    cboNK_BD->setMinimumWidth(120);

    QLabel *lblHK = new QLabel("Học kỳ:", tab);
    lblHK->setStyleSheet("font-weight: bold;");
    cboHK_BD = new QComboBox(tab);
    cboHK_BD->addItem("1", 1);
    cboHK_BD->addItem("2", 2);
    cboHK_BD->addItem("3", 3);

    QLabel *lblMH = new QLabel("Môn học:", tab);
    lblMH->setStyleSheet("font-weight: bold;");
    cboMH_BD = new QComboBox(tab);
    cboMH_BD->setMinimumWidth(220);

    QLabel *lblNhom = new QLabel("Nhóm:", tab);
    lblNhom->setStyleSheet("font-weight: bold;");
    cboNhom_BD = new QComboBox(tab);
    for (int i = 1; i <= 20; ++i) cboNhom_BD->addItem(QString::number(i), i);

    QPushButton *btnXem = new QPushButton("📊 Xem Bảng Điểm", tab);
    btnXem->setMinimumHeight(32);
    btnXem->setStyleSheet("background-color: #059669; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");
    connect(btnXem, &QPushButton::clicked, this, &TabDiem::onXemBangDiemMonHoc);

    filterLayout->addWidget(lblNK);
    filterLayout->addWidget(cboNK_BD);
    filterLayout->addWidget(lblHK);
    filterLayout->addWidget(cboHK_BD);
    filterLayout->addWidget(lblMH);
    filterLayout->addWidget(cboMH_BD, 1);
    filterLayout->addWidget(lblNhom);
    filterLayout->addWidget(cboNhom_BD);
    filterLayout->addWidget(btnXem);

    layout->addLayout(filterLayout);

    lblInfo_BD = new QLabel("Chọn tổ hợp Niên khóa, Học kỳ, Môn học, Nhóm để xem bảng điểm môn học.", tab);
    lblInfo_BD->setStyleSheet("color: #64748b; font-size: 13px; font-weight: 500;");
    layout->addWidget(lblInfo_BD);

    tblBangDiemMH = new QTableWidget(tab);
    tblBangDiemMH->setColumnCount(7);
    tblBangDiemMH->setHorizontalHeaderLabels({"STT", "Mã SV", "Họ", "Tên", "Mã Lớp", "Điểm", "Xếp Loại"});
    tblBangDiemMH->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblBangDiemMH->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tblBangDiemMH->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tblBangDiemMH->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tblBangDiemMH->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tblBangDiemMH->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tblBangDiemMH->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tblBangDiemMH->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblBangDiemMH->setAlternatingRowColors(true);
    tblBangDiemMH->verticalHeader()->setVisible(false);

    layout->addWidget(tblBangDiemMH);

    return tab;
}

void TabDiem::onXemBangDiemMonHoc() {
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_LOPSV &dsLop = AppData::instance().dsLop;
    DS_MonHoc &dsMH = AppData::instance().dsMH;

    QString nk = cboNK_BD->currentText();
    int hk = cboHK_BD->currentData().toInt();
    QString mamh = cboMH_BD->currentData().toString();
    int nhom = cboNhom_BD->currentData().toInt();

    SinhVienDiem dsSV[MAX_SV_DANGKY];
    int maLTCFound = -1;
    int count = LayDSSVDangKyLTCTheoThongTin(dsltc, dsLop, nk.toUtf8().constData(), hk,
                                            mamh.toUtf8().constData(), nhom, dsSV, MAX_SV_DANGKY, maLTCFound);

    if (maLTCFound == -1) {
        tblBangDiemMH->setRowCount(0);
        lblInfo_BD->setText("❌ Không tìm thấy Lớp Tín Chỉ tương ứng!");
        lblInfo_BD->setStyleSheet("color: #dc2626; font-weight: bold;");
        return;
    }

    TreeMonHoc mhNode = TimMonHoc(dsMH.root, mamh.toUtf8().constData());
    QString tenMH = mhNode ? QString::fromUtf8(mhNode->mh.TENMH) : mamh;

    lblInfo_BD->setText(QString("🎓 <b>BẢNG ĐIỂM MÔN HỌC (CÂU J):</b> %1 | Niên khóa: %2 - HK%3 - Nhóm: %4 | Tổng SV: <b>%5</b>")
                            .arg(tenMH).arg(nk).arg(hk).arg(nhom).arg(count));
    lblInfo_BD->setStyleSheet("color: #1e293b; font-size: 13px; font-weight: 500;");

    tblBangDiemMH->setRowCount(count);
    for (int i = 0; i < count; ++i) {
        QTableWidgetItem *itemSTT = new QTableWidgetItem(QString::number(i + 1));
        itemSTT->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemMa = new QTableWidgetItem(QString::fromUtf8(dsSV[i].MASV));
        itemMa->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemHo = new QTableWidgetItem(QString::fromUtf8(dsSV[i].HO));
        QTableWidgetItem *itemTen = new QTableWidgetItem(QString::fromUtf8(dsSV[i].TEN));
        itemTen->setTextAlignment(Qt::AlignCenter);

        LOPSV *lopChuaSV = nullptr;
        PTRSV pSV = TimSVToanTruong(dsLop, dsSV[i].MASV, lopChuaSV);
        QString maLopSV = lopChuaSV ? QString::fromUtf8(lopChuaSV->MALOP) : "";
        QTableWidgetItem *itemLop = new QTableWidgetItem(maLopSV);
        itemLop->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemDiem;
        QTableWidgetItem *itemXepLoai;
        if (dsSV[i].DIEM < 0) {
            itemDiem = new QTableWidgetItem("Chưa có");
            itemDiem->setForeground(QBrush(QColor("#94a3b8")));
            itemXepLoai = new QTableWidgetItem("-");
        } else {
            itemDiem = new QTableWidgetItem(QString::number(dsSV[i].DIEM, 'f', 1));
            itemDiem->setForeground(QBrush(QColor("#be3142")));

            float d = dsSV[i].DIEM;
            if (d >= 8.5) itemXepLoai = new QTableWidgetItem("Giỏi / Xuất sắc");
            else if (d >= 7.0) itemXepLoai = new QTableWidgetItem("Khá");
            else if (d >= 5.0) itemXepLoai = new QTableWidgetItem("Trung bình");
            else itemXepLoai = new QTableWidgetItem("Không đạt");
        }
        itemDiem->setTextAlignment(Qt::AlignCenter);
        itemXepLoai->setTextAlignment(Qt::AlignCenter);

        tblBangDiemMH->setItem(i, 0, itemSTT);
        tblBangDiemMH->setItem(i, 1, itemMa);
        tblBangDiemMH->setItem(i, 2, itemHo);
        tblBangDiemMH->setItem(i, 3, itemTen);
        tblBangDiemMH->setItem(i, 4, itemLop);
        tblBangDiemMH->setItem(i, 5, itemDiem);
        tblBangDiemMH->setItem(i, 6, itemXepLoai);
    }
}

// ====================================================================
// SUB-TAB 3: ĐIỂM TRUNG BÌNH KHÓA HỌC (CÂU K)
// ====================================================================
QWidget* TabDiem::createSubTabDTBKhoaHoc() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    QLabel *lblLop = new QLabel("Chọn Lớp Sinh Viên:", tab);
    lblLop->setStyleSheet("font-weight: bold;");
    cboLop_DTB = new QComboBox(tab);
    cboLop_DTB->setMinimumWidth(220);

    QPushButton *btnXem = new QPushButton("🎯 Tính & Xem Điểm Trung Bình Khóa Học", tab);
    btnXem->setMinimumHeight(32);
    btnXem->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");
    connect(btnXem, &QPushButton::clicked, this, &TabDiem::onXemDiemTrungBinhKhoaHoc);

    filterLayout->addWidget(lblLop);
    filterLayout->addWidget(cboLop_DTB);
    filterLayout->addWidget(btnXem);
    filterLayout->addStretch();

    layout->addLayout(filterLayout);

    lblInfo_DTB = new QLabel("Điểm trung bình khóa học được tính theo công thức tích lũy số tín chỉ (điểm thi lớn nhất từng môn).", tab);
    lblInfo_DTB->setStyleSheet("color: #64748b; font-size: 13px; font-weight: 500;");
    layout->addWidget(lblInfo_DTB);

    tblDTB = new QTableWidget(tab);
    tblDTB->setColumnCount(7);
    tblDTB->setHorizontalHeaderLabels({"STT", "Mã SV", "Họ", "Tên", "Tổng Tín Chỉ", "Điểm Trung Bình (Hệ 10)", "Xếp Loại Tốt Nghiệp"});
    tblDTB->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblDTB->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tblDTB->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tblDTB->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tblDTB->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tblDTB->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tblDTB->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tblDTB->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblDTB->setAlternatingRowColors(true);
    tblDTB->verticalHeader()->setVisible(false);

    layout->addWidget(tblDTB);

    return tab;
}

void TabDiem::onXemDiemTrungBinhKhoaHoc() {
    QString malop = cboLop_DTB->currentData().toString();
    if (malop.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn lớp sinh viên!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_MonHoc &dsMH = AppData::instance().dsMH;

    SinhVienDiemTB dsTB[MAX_SV_LOP];
    int count = LayDSDiemTrungBinhLop(dsLop, dsltc, dsMH.root, malop.toUtf8().constData(), dsTB, MAX_SV_LOP);

    lblInfo_DTB->setText(QString("🎓 <b>ĐIỂM TRUNG BÌNH KHÓA HỌC LỚP: %1</b> | Sĩ số: <b>%2</b> sinh viên").arg(malop).arg(count));
    lblInfo_DTB->setStyleSheet("color: #1e293b; font-size: 13px; font-weight: 500;");

    tblDTB->setRowCount(count);
    for (int i = 0; i < count; ++i) {
        QTableWidgetItem *itemSTT = new QTableWidgetItem(QString::number(i + 1));
        itemSTT->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemMa = new QTableWidgetItem(QString::fromUtf8(dsTB[i].MASV));
        itemMa->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemHo = new QTableWidgetItem(QString::fromUtf8(dsTB[i].HO));
        QTableWidgetItem *itemTen = new QTableWidgetItem(QString::fromUtf8(dsTB[i].TEN));
        itemTen->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemTC = new QTableWidgetItem(QString::number(dsTB[i].TONG_TIN_CHI));
        itemTC->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemDTB;
        QTableWidgetItem *itemXepLoai;
        if (dsTB[i].DIEMTB < 0) {
            itemDTB = new QTableWidgetItem("Chưa tích lũy");
            itemDTB->setForeground(QBrush(QColor("#94a3b8")));
            itemXepLoai = new QTableWidgetItem("-");
        } else {
            itemDTB = new QTableWidgetItem(QString::number(dsTB[i].DIEMTB, 'f', 2));
            itemDTB->setForeground(QBrush(QColor("#be3142")));

            float d = dsTB[i].DIEMTB;
            if (d >= 9.0) itemXepLoai = new QTableWidgetItem("Xuất sắc");
            else if (d >= 8.0) itemXepLoai = new QTableWidgetItem("Giỏi");
            else if (d >= 7.0) itemXepLoai = new QTableWidgetItem("Khá");
            else if (d >= 5.0) itemXepLoai = new QTableWidgetItem("Trung bình");
            else itemXepLoai = new QTableWidgetItem("Yếu / Kém");
        }
        itemDTB->setTextAlignment(Qt::AlignCenter);
        itemXepLoai->setTextAlignment(Qt::AlignCenter);

        tblDTB->setItem(i, 0, itemSTT);
        tblDTB->setItem(i, 1, itemMa);
        tblDTB->setItem(i, 2, itemHo);
        tblDTB->setItem(i, 3, itemTen);
        tblDTB->setItem(i, 4, itemTC);
        tblDTB->setItem(i, 5, itemDTB);
        tblDTB->setItem(i, 6, itemXepLoai);
    }
}

// ====================================================================
// SUB-TAB 4: BẢNG ĐIỂM TỔNG KẾT MA TRẬN (CÂU L)
// ====================================================================
QWidget* TabDiem::createSubTabTongKet() {
    QWidget *tab = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(10);

    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    QLabel *lblLop = new QLabel("Chọn Lớp Sinh Viên:", tab);
    lblLop->setStyleSheet("font-weight: bold;");
    cboLop_TK = new QComboBox(tab);
    cboLop_TK->setMinimumWidth(220);

    QPushButton *btnXem = new QPushButton("📊 Trích Xuất Bảng Điểm Ma Trận (Câu l)", tab);
    btnXem->setMinimumHeight(32);
    btnXem->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");
    connect(btnXem, &QPushButton::clicked, this, &TabDiem::onXemBangDiemTongKet);

    filterLayout->addWidget(lblLop);
    filterLayout->addWidget(cboLop_TK);
    filterLayout->addWidget(btnXem);
    filterLayout->addStretch();

    layout->addLayout(filterLayout);

    lblInfo_TK = new QLabel("Bảng điểm tổng kết hiển thị điểm thi lớn nhất của từng môn học cho toàn bộ sinh viên trong lớp.", tab);
    lblInfo_TK->setStyleSheet("color: #64748b; font-size: 13px; font-weight: 500;");
    layout->addWidget(lblInfo_TK);

    tblTongKet = new QTableWidget(tab);
    tblTongKet->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblTongKet->setAlternatingRowColors(true);
    tblTongKet->verticalHeader()->setVisible(false);

    layout->addWidget(tblTongKet);

    return tab;
}

void TabDiem::onXemBangDiemTongKet() {
    QString malop = cboLop_TK->currentData().toString();
    if (malop.isEmpty()) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn lớp sinh viên!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_MonHoc &dsMH = AppData::instance().dsMH;

    // Use heap allocation for BangDiemTongKet to avoid stack pressure
    BangDiemTongKet *bangDiem = new BangDiemTongKet();
    bool ok = LayBangDiemTongKetLop(dsLop, dsltc, dsMH.root, malop.toUtf8().constData(), *bangDiem);

    if (!ok) {
        delete bangDiem;
        tblTongKet->setRowCount(0);
        tblTongKet->setColumnCount(0);
        lblInfo_TK->setText(QString("❌ Không tìm thấy thông tin của lớp: %1!").arg(malop));
        lblInfo_TK->setStyleSheet("color: #dc2626; font-weight: bold;");
        return;
    }

    lblInfo_TK->setText(
        QString("📊 <b>BẢNG ĐIỂM TỔNG KẾT (CÂU L) - LỚP: %1</b> (%2) | Số môn: <b>%3</b> | Số SV: <b>%4</b>")
            .arg(bangDiem->MALOP)
            .arg(QString::fromUtf8(bangDiem->TENLOP))
            .arg(bangDiem->soMon)
            .arg(bangDiem->soSV));
    lblInfo_TK->setStyleSheet("color: #1e293b; font-size: 13px; font-weight: 500;");

    // Dynamic columns: STT, Mã SV, Họ và Tên, [MAMH_1...MAMH_n]
    int colCount = 3 + bangDiem->soMon;
    tblTongKet->clear();
    tblTongKet->setColumnCount(colCount);

    QStringList headers;
    headers << "STT" << "Mã SV" << "Họ và Tên";
    for (int j = 0; j < bangDiem->soMon; ++j) {
        headers << QString("%1\n(%2)").arg(bangDiem->danhSachMAMH[j]).arg(QString::fromUtf8(bangDiem->danhSachTENMH[j]));
    }
    tblTongKet->setHorizontalHeaderLabels(headers);
    tblTongKet->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblTongKet->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tblTongKet->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    for (int j = 3; j < colCount; ++j) {
        tblTongKet->horizontalHeader()->setSectionResizeMode(j, QHeaderView::ResizeToContents);
    }

    tblTongKet->setRowCount(bangDiem->soSV);
    for (int i = 0; i < bangDiem->soSV; ++i) {
        QTableWidgetItem *itemSTT = new QTableWidgetItem(QString::number(i + 1));
        itemSTT->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemMa = new QTableWidgetItem(QString::fromUtf8(bangDiem->dsSV[i].MASV));
        itemMa->setTextAlignment(Qt::AlignCenter);

        QString hoTen = QString("%1 %2").arg(QString::fromUtf8(bangDiem->dsSV[i].HO)).arg(QString::fromUtf8(bangDiem->dsSV[i].TEN));
        QTableWidgetItem *itemHoTen = new QTableWidgetItem(hoTen);

        tblTongKet->setItem(i, 0, itemSTT);
        tblTongKet->setItem(i, 1, itemMa);
        tblTongKet->setItem(i, 2, itemHoTen);

        for (int j = 0; j < bangDiem->soMon; ++j) {
            float diem = bangDiem->dsSV[i].diemMon[j];
            QTableWidgetItem *itemDiem;
            if (diem < 0) {
                itemDiem = new QTableWidgetItem("-");
                itemDiem->setForeground(QBrush(QColor("#cbd5e1")));
            } else {
                itemDiem = new QTableWidgetItem(QString::number(diem, 'f', 1));
                if (diem < 5.0) itemDiem->setForeground(QBrush(QColor("#dc2626")));
                else itemDiem->setForeground(QBrush(QColor("#166534")));
            }
            itemDiem->setTextAlignment(Qt::AlignCenter);
            tblTongKet->setItem(i, 3 + j, itemDiem);
        }
    }

    delete bangDiem;
}

// ====================================================================
// DATA POPULATION HELPERS
// ====================================================================
void TabDiem::capNhatComboMonHoc(QComboBox *cbo) {
    cbo->blockSignals(true);
    cbo->clear();
    DS_MonHoc &dsMH = AppData::instance().dsMH;
    if (dsMH.root != nullptr && dsMH.n > 0) {
        TreeMonHoc *arrMH = new TreeMonHoc[dsMH.n];
        int count = 0;
        InOrderTraversal(dsMH.root, arrMH, count);
        for (int i = 0; i < count; ++i) {
            cbo->addItem(QString("%1 - %2").arg(arrMH[i]->mh.MAMH).arg(QString::fromUtf8(arrMH[i]->mh.TENMH)),
                         QString::fromUtf8(arrMH[i]->mh.MAMH));
        }
        delete[] arrMH;
    }
    cbo->blockSignals(false);
}

void TabDiem::capNhatComboLop(QComboBox *cbo) {
    cbo->blockSignals(true);
    cbo->clear();
    DS_LOPSV &dsLop = AppData::instance().dsLop;
    for (int i = 0; i < dsLop.n; ++i) {
        cbo->addItem(QString("%1 - %2").arg(dsLop.nodes[i].MALOP).arg(QString::fromUtf8(dsLop.nodes[i].TENLOP)),
                     QString::fromUtf8(dsLop.nodes[i].MALOP));
    }
    cbo->blockSignals(false);
}

void TabDiem::capNhatComboNienKhoa(QComboBox *cbo) {
    cbo->blockSignals(true);
    cbo->clear();
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    for (int i = 0; i < dsltc.n; ++i) {
        if (dsltc.nodes[i] != nullptr) {
            QString nk = QString::fromUtf8(dsltc.nodes[i]->NienKhoa);
            if (cbo->findText(nk) == -1) {
                cbo->addItem(nk);
            }
        }
    }
    if (cbo->count() == 0) {
        cbo->addItem("2023-2024");
    }
    cbo->blockSignals(false);
}

void TabDiem::loadData() {
    capNhatComboNienKhoa(cboNK_Nhap);
    capNhatComboMonHoc(cboMH_Nhap);

    capNhatComboNienKhoa(cboNK_BD);
    capNhatComboMonHoc(cboMH_BD);

    capNhatComboLop(cboLop_DTB);
    capNhatComboLop(cboLop_TK);
}

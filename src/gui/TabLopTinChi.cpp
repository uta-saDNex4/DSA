#include "TabLopTinChi.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QShortcut>
#include <QKeySequence>
#include <cstring>
#include "monhoc.h"
#include "lop.h"
#include "sinhvien.h"
#include "dangky.h"

TabLopTinChi::TabLopTinChi(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadData();
}

void TabLopTinChi::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // ================= TOP FILTER BAR =================
    QHBoxLayout *filterLayout = new QHBoxLayout();
    
    QLabel *lblNK = new QLabel("Niên khóa:", this);
    lblNK->setStyleSheet("font-weight: bold; color: #1e293b;");
    cboNienKhoa = new QComboBox(this);
    cboNienKhoa->setMinimumWidth(130);
    cboNienKhoa->setMinimumHeight(34);
    cboNienKhoa->addItem("-- Tất cả --");
    connect(cboNienKhoa, &QComboBox::currentTextChanged, this, &TabLopTinChi::onFilterChanged);

    QLabel *lblHK = new QLabel("Học kỳ:", this);
    lblHK->setStyleSheet("font-weight: bold; color: #1e293b;");
    cboHocKy = new QComboBox(this);
    cboHocKy->setMinimumWidth(100);
    cboHocKy->setMinimumHeight(34);
    cboHocKy->addItem("-- Tất cả --", 0);
    cboHocKy->addItem("Học kỳ 1", 1);
    cboHocKy->addItem("Học kỳ 2", 2);
    cboHocKy->addItem("Học kỳ 3", 3);
    connect(cboHocKy, &QComboBox::currentIndexChanged, this, &TabLopTinChi::onFilterChanged);

    QPushButton *btnMoLop = new QPushButton("➕ [F1] Mở Lớp TC", this);
    btnMoLop->setMinimumHeight(34);
    btnMoLop->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 5px; padding: 0 12px;");
    connect(btnMoLop, &QPushButton::clicked, this, &TabLopTinChi::onMoLopTC);

    QPushButton *btnSua = new QPushButton("✏️ [F3] Hiệu Chỉnh", this);
    btnSua->setMinimumHeight(34);
    btnSua->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 5px; padding: 0 12px;");
    connect(btnSua, &QPushButton::clicked, this, &TabLopTinChi::onHieuChinhLopTC);

    QPushButton *btnXoa = new QPushButton("🗑️ [Del] Xóa Lớp", this);
    btnXoa->setMinimumHeight(34);
    btnXoa->setStyleSheet("background-color: #dc2626; color: white; font-weight: bold; border-radius: 5px; padding: 0 12px;");
    connect(btnXoa, &QPushButton::clicked, this, &TabLopTinChi::onXoaLopTC);

    QPushButton *btnHuyThieuSiSo = new QPushButton("⚠️ [F4] Hủy Lớp < Min", this);
    btnHuyThieuSiSo->setMinimumHeight(34);
    btnHuyThieuSiSo->setStyleSheet("background-color: #d97706; color: white; font-weight: bold; border-radius: 5px; padding: 0 12px;");
    connect(btnHuyThieuSiSo, &QPushButton::clicked, this, &TabLopTinChi::onHuyLopThieuSiSo);

    QPushButton *btnXemDSSV = new QPushButton("👥 [F6] Xem DSSV", this);
    btnXemDSSV->setMinimumHeight(34);
    btnXemDSSV->setStyleSheet("background-color: #059669; color: white; font-weight: bold; border-radius: 5px; padding: 0 12px;");
    connect(btnXemDSSV, &QPushButton::clicked, this, &TabLopTinChi::onXemDSSVDangKy);

    QPushButton *btnReload = new QPushButton("🔄 [F5]", this);
    btnReload->setMinimumHeight(34);
    btnReload->setToolTip("Làm mới danh sách");
    connect(btnReload, &QPushButton::clicked, this, &TabLopTinChi::loadData);

    filterLayout->addWidget(lblNK);
    filterLayout->addWidget(cboNienKhoa);
    filterLayout->addWidget(lblHK);
    filterLayout->addWidget(cboHocKy);
    filterLayout->addStretch(1);
    filterLayout->addWidget(btnMoLop);
    filterLayout->addWidget(btnSua);
    filterLayout->addWidget(btnXoa);
    filterLayout->addWidget(btnHuyThieuSiSo);
    filterLayout->addWidget(btnXemDSSV);
    filterLayout->addWidget(btnReload);

    mainLayout->addLayout(filterLayout);

    // ================= TABLE =================
    tableLTC = new QTableWidget(this);
    tableLTC->setColumnCount(11);
    tableLTC->setHorizontalHeaderLabels({
        "Mã LTC", "Mã MH", "Tên Môn Học", "Niên Khóa", "Học Kỳ",
        "Nhóm", "SV Min", "SV Max", "Đã ĐK", "Còn Trống", "Trạng Thái"
    });
    tableLTC->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableLTC->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
    tableLTC->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);
    tableLTC->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableLTC->setSelectionMode(QAbstractItemView::SingleSelection);
    tableLTC->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableLTC->setAlternatingRowColors(true);
    tableLTC->verticalHeader()->setVisible(false);

    connect(tableLTC, &QTableWidget::cellDoubleClicked, this, &TabLopTinChi::onXemDSSVDangKy);

    mainLayout->addWidget(tableLTC);

    // ================= BOTTOM STATUS BAR =================
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: #475569; font-size: 13px; font-weight: 500;");
    bottomLayout->addWidget(statusLabel);

    bottomLayout->addStretch();
    QLabel *hintLabel = new QLabel("Phím tắt: [F1] Mở lớp | [F3] Sửa | [Del] Xóa | [F4] Hủy lớp < Min | [F6] DSSV | [F5] Tải lại", this);
    hintLabel->setStyleSheet("color: #94a3b8; font-size: 12px; font-style: italic;");
    bottomLayout->addWidget(hintLabel);

    mainLayout->addLayout(bottomLayout);

    // Keyboard shortcuts
    new QShortcut(QKeySequence(Qt::Key_F1), this, SLOT(onMoLopTC()));
    new QShortcut(QKeySequence(Qt::Key_F3), this, SLOT(onHieuChinhLopTC()));
    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(onXoaLopTC()));
    new QShortcut(QKeySequence(Qt::Key_F4), this, SLOT(onHuyLopThieuSiSo()));
    new QShortcut(QKeySequence(Qt::Key_F5), this, SLOT(loadData()));
    new QShortcut(QKeySequence(Qt::Key_F6), this, SLOT(onXemDSSVDangKy()));
}

void TabLopTinChi::capNhatDanhSachNienKhoa() {
    QString current = cboNienKhoa->currentText();
    cboNienKhoa->blockSignals(true);
    cboNienKhoa->clear();
    cboNienKhoa->addItem("-- Tất cả --");

    DS_LopTC &dsltc = AppData::instance().dsLTC;
    for (int i = 0; i < dsltc.n; ++i) {
        if (dsltc.nodes[i] != nullptr) {
            QString nk = QString::fromUtf8(dsltc.nodes[i]->NienKhoa);
            if (cboNienKhoa->findText(nk) == -1) {
                cboNienKhoa->addItem(nk);
            }
        }
    }

    int idx = cboNienKhoa->findText(current);
    if (idx >= 0) cboNienKhoa->setCurrentIndex(idx);
    else cboNienKhoa->setCurrentIndex(0);

    cboNienKhoa->blockSignals(false);
}

void TabLopTinChi::loadData() {
    capNhatDanhSachNienKhoa();
    onFilterChanged();
}

void TabLopTinChi::onFilterChanged() {
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_MonHoc &dsMH = AppData::instance().dsMH;

    QString filterNK = cboNienKhoa->currentText();
    int filterHK = cboHocKy->currentData().toInt();

    tableLTC->setRowCount(0);
    int row = 0;

    for (int i = 0; i < dsltc.n; ++i) {
        if (dsltc.nodes[i] == nullptr) continue;
        LopTinChi *ltc = dsltc.nodes[i];

        if (filterNK != "-- Tất cả --" && filterNK != QString::fromUtf8(ltc->NienKhoa)) {
            continue;
        }
        if (filterHK > 0 && ltc->HocKy != filterHK) {
            continue;
        }

        tableLTC->insertRow(row);

        // Ten MH
        TreeMonHoc mhNode = TimMonHoc(dsMH.root, ltc->MAMH);
        QString tenMH = mhNode ? QString::fromUtf8(mhNode->mh.TENMH) : "(Không tìm thấy)";

        int soDk = DemSVDangKyHopLe(ltc->FirstDK);
        int conTrong = ltc->SoSVMax - soDk;
        if (conTrong < 0) conTrong = 0;

        QTableWidgetItem *itemMaLTC = new QTableWidgetItem(QString::number(ltc->MALOPTC));
        itemMaLTC->setTextAlignment(Qt::AlignCenter);
        itemMaLTC->setData(Qt::UserRole, ltc->MALOPTC);

        QTableWidgetItem *itemMaMH = new QTableWidgetItem(QString::fromUtf8(ltc->MAMH));
        itemMaMH->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemTenMH = new QTableWidgetItem(tenMH);

        QTableWidgetItem *itemNK = new QTableWidgetItem(QString::fromUtf8(ltc->NienKhoa));
        itemNK->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemHK = new QTableWidgetItem(QString::number(ltc->HocKy));
        itemHK->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemNhom = new QTableWidgetItem(QString::number(ltc->Nhom));
        itemNhom->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemMin = new QTableWidgetItem(QString::number(ltc->SoSVMin));
        itemMin->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemMax = new QTableWidgetItem(QString::number(ltc->SoSVMax));
        itemMax->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemDK = new QTableWidgetItem(QString::number(soDk));
        itemDK->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemTrong = new QTableWidgetItem(QString::number(conTrong));
        itemTrong->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemTrangThai;
        if (ltc->HuyLop) {
            itemTrangThai = new QTableWidgetItem("❌ Đã Hủy");
            itemTrangThai->setForeground(QBrush(QColor("#dc2626")));
        } else if (soDk >= ltc->SoSVMax) {
            itemTrangThai = new QTableWidgetItem("🔴 Hết Chỗ");
            itemTrangThai->setForeground(QBrush(QColor("#ea580c")));
        } else if (soDk < ltc->SoSVMin) {
            itemTrangThai = new QTableWidgetItem(QString("⚠️ Thiếu SV (%1/%2)").arg(soDk).arg(ltc->SoSVMin));
            itemTrangThai->setForeground(QBrush(QColor("#d97706")));
        } else {
            itemTrangThai = new QTableWidgetItem("🟢 Đang Mở");
            itemTrangThai->setForeground(QBrush(QColor("#16a34a")));
        }
        itemTrangThai->setTextAlignment(Qt::AlignCenter);

        tableLTC->setItem(row, 0, itemMaLTC);
        tableLTC->setItem(row, 1, itemMaMH);
        tableLTC->setItem(row, 2, itemTenMH);
        tableLTC->setItem(row, 3, itemNK);
        tableLTC->setItem(row, 4, itemHK);
        tableLTC->setItem(row, 5, itemNhom);
        tableLTC->setItem(row, 6, itemMin);
        tableLTC->setItem(row, 7, itemMax);
        tableLTC->setItem(row, 8, itemDK);
        tableLTC->setItem(row, 9, itemTrong);
        tableLTC->setItem(row, 10, itemTrangThai);

        row++;
    }

    statusLabel->setText(QString("Hiển thị %1 / %2 lớp tín chỉ").arg(row).arg(dsltc.n));
}

void TabLopTinChi::onMoLopTC() {
    DS_MonHoc &dsMH = AppData::instance().dsMH;
    DS_LopTC &dsltc = AppData::instance().dsLTC;

    if (dsMH.root == nullptr || dsMH.n == 0) {
        QMessageBox::warning(this, "Thông báo", "Chưa có môn học nào trong hệ thống! Vui lòng thêm môn học trước.");
        return;
    }

    if (dsltc.n >= MAX_LOPTC) {
        QMessageBox::warning(this, "Thông báo", "Số lượng lớp tín chỉ đã đạt tối đa (10,000 lớp)!");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("➕ Mở Lớp Tín Chỉ Mới");
    dlg.setMinimumWidth(450);

    QFormLayout *form = new QFormLayout(&dlg);

    int autoMa = LayMaLopTCTuDong(dsltc);
    QLabel *lblAutoMa = new QLabel(QString::number(autoMa), &dlg);
    lblAutoMa->setStyleSheet("font-weight: bold; color: #be3142; font-size: 14px;");
    form->addRow("Mã Lớp TC (Tự động):", lblAutoMa);

    // Môn học combo
    QComboBox *cboMH = new QComboBox(&dlg);
    TreeMonHoc *arrMH = new TreeMonHoc[dsMH.n];
    int countMH = 0;
    InOrderTraversal(dsMH.root, arrMH, countMH);
    for (int i = 0; i < countMH; ++i) {
        cboMH->addItem(QString("%1 - %2").arg(arrMH[i]->mh.MAMH).arg(QString::fromUtf8(arrMH[i]->mh.TENMH)),
                       QString::fromUtf8(arrMH[i]->mh.MAMH));
    }
    delete[] arrMH;
    form->addRow("Môn học (*):", cboMH);

    QLineEdit *txtNK = new QLineEdit(&dlg);
    txtNK->setPlaceholderText("VD: 2023-2024");
    if (cboNienKhoa->currentText() != "-- Tất cả --") {
        txtNK->setText(cboNienKhoa->currentText());
    } else {
        txtNK->setText("2023-2024");
    }
    form->addRow("Niên khóa (*):", txtNK);

    QSpinBox *spnHK = new QSpinBox(&dlg);
    spnHK->setRange(1, 3);
    if (cboHocKy->currentIndex() > 0) spnHK->setValue(cboHocKy->currentIndex());
    else spnHK->setValue(1);
    form->addRow("Học kỳ (1 - 3):", spnHK);

    QSpinBox *spnNhom = new QSpinBox(&dlg);
    spnNhom->setRange(1, 100);
    spnNhom->setValue(1);
    form->addRow("Nhóm lớp (1 - 100):", spnNhom);

    QSpinBox *spnMin = new QSpinBox(&dlg);
    spnMin->setRange(1, 500);
    spnMin->setValue(10);
    form->addRow("Số SV tối thiểu (Min):", spnMin);

    QSpinBox *spnMax = new QSpinBox(&dlg);
    spnMax->setRange(1, 500);
    spnMax->setValue(60);
    form->addRow("Số SV tối đa (Max):", spnMax);

    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(box);

    connect(box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        QString mamh = cboMH->currentData().toString().trimmed();
        QString nk = txtNK->text().trimmed();
        int hk = spnHK->value();
        int nhom = spnNhom->value();
        int minSV = spnMin->value();
        int maxSV = spnMax->value();

        if (nk.isEmpty()) {
            QMessageBox::warning(this, "Lỗi nhập liệu", "Niên khóa không được để trống!");
            return;
        }

        if (minSV > maxSV) {
            QMessageBox::warning(this, "Lỗi nhập liệu", "Số SV tối thiểu không được lớn hơn số SV tối đa!");
            return;
        }

        LopTinChi ltc;
        ltc.MALOPTC = autoMa;
        std::strncpy(ltc.MAMH, mamh.toUtf8().constData(), sizeof(ltc.MAMH) - 1);
        ltc.MAMH[sizeof(ltc.MAMH) - 1] = '\0';
        std::strncpy(ltc.NienKhoa, nk.toUtf8().constData(), sizeof(ltc.NienKhoa) - 1);
        ltc.NienKhoa[sizeof(ltc.NienKhoa) - 1] = '\0';
        ltc.HocKy = hk;
        ltc.Nhom = nhom;
        ltc.SoSVMin = minSV;
        ltc.SoSVMax = maxSV;
        ltc.HuyLop = false;
        KhoiTaoDS_DangKy(ltc.FirstDK);

        if (ThemLopTC(dsltc, ltc, dsMH.root)) {
            AppData::instance().saveAll();
            loadData();
            QMessageBox::information(this, "Thành công", QString("Đã mở thành công Lớp Tín Chỉ mã: %1!").arg(autoMa));
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể mở lớp! Lớp tín chỉ cùng Niên khóa, Học kỳ, Môn học và Nhóm đã tồn tại.");
        }
    }
}

void TabLopTinChi::onHieuChinhLopTC() {
    int curRow = tableLTC->currentRow();
    if (curRow < 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một lớp tín chỉ cần hiệu chỉnh!");
        return;
    }

    int maLTC = tableLTC->item(curRow, 0)->data(Qt::UserRole).toInt();
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_MonHoc &dsMH = AppData::instance().dsMH;

    int idx = TimLopTCTheoMa(dsltc, maLTC);
    if (idx == -1 || dsltc.nodes[idx] == nullptr) {
        QMessageBox::critical(this, "Lỗi", "Không tìm thấy thông tin lớp tín chỉ này!");
        return;
    }

    LopTinChi *ltc = dsltc.nodes[idx];
    int soSV = DemSVDangKyHopLe(ltc->FirstDK);

    QDialog dlg(this);
    dlg.setWindowTitle(QString("✏️ Hiệu Chỉnh Lớp Tín Chỉ: %1").arg(maLTC));
    dlg.setMinimumWidth(450);

    QFormLayout *form = new QFormLayout(&dlg);

    QLabel *lblMa = new QLabel(QString::number(maLTC), &dlg);
    lblMa->setStyleSheet("font-weight: bold; color: #be3142;");
    form->addRow("Mã LTC:", lblMa);

    TreeMonHoc mhNode = TimMonHoc(dsMH.root, ltc->MAMH);
    QString tenMH = mhNode ? QString::fromUtf8(mhNode->mh.TENMH) : "(Không tìm thấy)";
    QLabel *lblMH = new QLabel(QString("%1 - %2").arg(ltc->MAMH).arg(tenMH), &dlg);
    form->addRow("Môn học:", lblMH);

    QLineEdit *txtNK = new QLineEdit(QString::fromUtf8(ltc->NienKhoa), &dlg);
    QSpinBox *spnHK = new QSpinBox(&dlg);
    spnHK->setRange(1, 3);
    spnHK->setValue(ltc->HocKy);

    QSpinBox *spnNhom = new QSpinBox(&dlg);
    spnNhom->setRange(1, 100);
    spnNhom->setValue(ltc->Nhom);

    // Nếu đã có SV đăng ký, không cho sửa Niên khóa, Học kỳ, Nhóm để đảm bảo tính toàn vẹn
    if (soSV > 0) {
        txtNK->setReadOnly(true);
        spnHK->setEnabled(false);
        spnNhom->setEnabled(false);
        QLabel *lblNotice = new QLabel("⚠️ Lớp đã có sinh viên đăng ký, chỉ được phép sửa Sĩ số Min / Max!", &dlg);
        lblNotice->setStyleSheet("color: #d97706; font-weight: bold;");
        form->addRow(lblNotice);
    }

    form->addRow("Niên khóa:", txtNK);
    form->addRow("Học kỳ:", spnHK);
    form->addRow("Nhóm:", spnNhom);

    QSpinBox *spnMin = new QSpinBox(&dlg);
    spnMin->setRange(1, 500);
    spnMin->setValue(ltc->SoSVMin);
    form->addRow("Số SV Min:", spnMin);

    QSpinBox *spnMax = new QSpinBox(&dlg);
    spnMax->setRange(soSV > 0 ? soSV : 1, 500);
    spnMax->setValue(ltc->SoSVMax);
    form->addRow("Số SV Max:", spnMax);

    QDialogButtonBox *box = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(box);

    connect(box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        if (spnMin->value() > spnMax->value()) {
            QMessageBox::warning(this, "Lỗi", "Số SV Min không được lớn hơn Số SV Max!");
            return;
        }

        LopTinChi ltcMoi = *ltc;
        std::strncpy(ltcMoi.NienKhoa, txtNK->text().trimmed().toUtf8().constData(), sizeof(ltcMoi.NienKhoa) - 1);
        ltcMoi.NienKhoa[sizeof(ltcMoi.NienKhoa) - 1] = '\0';
        ltcMoi.HocKy = spnHK->value();
        ltcMoi.Nhom = spnNhom->value();
        ltcMoi.SoSVMin = spnMin->value();
        ltcMoi.SoSVMax = spnMax->value();

        if (HieuChinhLopTC(dsltc, maLTC, ltcMoi, dsMH.root)) {
            AppData::instance().saveAll();
            loadData();
            QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin lớp tín chỉ thành công!");
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể cập nhật! Trùng tổ hợp Niên khóa, Học kỳ, Môn học và Nhóm với lớp khác.");
        }
    }
}

void TabLopTinChi::onXoaLopTC() {
    int curRow = tableLTC->currentRow();
    if (curRow < 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một lớp tín chỉ cần xóa!");
        return;
    }

    int maLTC = tableLTC->item(curRow, 0)->data(Qt::UserRole).toInt();
    DS_LopTC &dsltc = AppData::instance().dsLTC;

    int idx = TimLopTCTheoMa(dsltc, maLTC);
    if (idx == -1 || dsltc.nodes[idx] == nullptr) return;

    if (DemSVDangKyHopLe(dsltc.nodes[idx]->FirstDK) > 0) {
        QMessageBox::warning(this, "Từ chối xóa",
                             "Lớp tín chỉ này hiện ĐÃ CÓ sinh viên đăng ký!\nTheo quy định đề tài, không thể xóa lớp tín chỉ này.");
        return;
    }

    auto ret = QMessageBox::question(this, "Xác nhận xóa",
                                     QString("Bạn có chắc chắn muốn xóa Lớp Tín Chỉ mã: %1 không?").arg(maLTC),
                                     QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        if (XoaLopTC(dsltc, maLTC)) {
            AppData::instance().saveAll();
            loadData();
            QMessageBox::information(this, "Thành công", "Đã xóa lớp tín chỉ thành công!");
        } else {
            QMessageBox::critical(this, "Lỗi", "Không thể xóa lớp tín chỉ này!");
        }
    }
}

void TabLopTinChi::onHuyLopThieuSiSo() {
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    QString filterNK = cboNienKhoa->currentText();
    int filterHK = cboHocKy->currentData().toInt();

    if (filterNK == "-- Tất cả --" || filterHK <= 0) {
        QMessageBox::information(this, "Hướng dẫn",
                                 "Vui lòng chọn cụ thể Niên khóa và Học kỳ ở thanh lọc phía trên trước khi thực hiện chức năng hủy lớp thiếu sĩ số!");
        return;
    }

    // Đếm số lớp thiếu sĩ số
    int countThieu = 0;
    QString danhSachHuy = "";
    for (int i = 0; i < dsltc.n; ++i) {
        if (dsltc.nodes[i] == nullptr) continue;
        LopTinChi *ltc = dsltc.nodes[i];
        if (strcmp(ltc->NienKhoa, filterNK.toUtf8().constData()) == 0 &&
            ltc->HocKy == filterHK &&
            !ltc->HuyLop) {
            int soDk = DemSVDangKyHopLe(ltc->FirstDK);
            if (soDk < ltc->SoSVMin) {
                countThieu++;
                danhSachHuy += QString("- Mã LTC %1 | Môn %2 | Nhóm %3 | Đã ĐK: %4 / Min: %5\n")
                                   .arg(ltc->MALOPTC)
                                   .arg(ltc->MAMH)
                                   .arg(ltc->Nhom)
                                   .arg(soDk)
                                   .arg(ltc->SoSVMin);
            }
        }
    }

    if (countThieu == 0) {
        QMessageBox::information(this, "Thông báo",
                                 QString("Tất cả các lớp trong Niên khóa %1, Học kỳ %2 đều đạt đủ sĩ số tối thiểu hoặc đã hủy trước đó!")
                                     .arg(filterNK).arg(filterHK));
        return;
    }

    auto ret = QMessageBox::warning(this, "Xác nhận Hủy Lớp Tín Chỉ",
                                    QString("Tìm thấy %1 lớp tín chỉ có số SV đăng ký < Min trong Niên khóa %2, Học kỳ %3:\n\n%4\n"
                                            "Bạn có chắc chắn muốn HỦY TẤT CẢ các lớp tín chỉ trên không?")
                                        .arg(countThieu).arg(filterNK).arg(filterHK).arg(danhSachHuy),
                                    QMessageBox::Yes | QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        XacNhanVaHuyLopTC(dsltc, filterNK.toUtf8().constData(), filterHK);
        AppData::instance().saveAll();
        loadData();
        QMessageBox::information(this, "Thành công", QString("Đã hủy thành công %1 lớp tín chỉ thiếu sĩ số!").arg(countThieu));
    }
}

void TabLopTinChi::onXemDSSVDangKy() {
    int curRow = tableLTC->currentRow();
    if (curRow < 0) {
        QMessageBox::warning(this, "Thông báo", "Vui lòng chọn một lớp tín chỉ để xem danh sách sinh viên!");
        return;
    }

    int maLTC = tableLTC->item(curRow, 0)->data(Qt::UserRole).toInt();
    DS_LopTC &dsltc = AppData::instance().dsLTC;
    DS_MonHoc &dsMH = AppData::instance().dsMH;
    DS_LOPSV &dsLop = AppData::instance().dsLop;

    int idx = TimLopTCTheoMa(dsltc, maLTC);
    if (idx == -1 || dsltc.nodes[idx] == nullptr) return;

    LopTinChi *ltc = dsltc.nodes[idx];
    TreeMonHoc mhNode = TimMonHoc(dsMH.root, ltc->MAMH);
    QString tenMH = mhNode ? QString::fromUtf8(mhNode->mh.TENMH) : "(Không tìm thấy)";

    QDialog dlg(this);
    dlg.setWindowTitle(QString("📋 DANH SÁCH SINH VIÊN ĐĂNG KÝ LỚP TÍN CHỈ #%1").arg(maLTC));
    dlg.resize(750, 480);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    // Tiêu đề thông tin lớp
    QLabel *lblTitle = new QLabel(
        QString("<h3 style='color: #740d18; margin: 0;'>DANH SÁCH SINH VIÊN ĐĂNG KÝ LỚP TÍN CHỈ</h3>"
                "<b>Mã lớp TC:</b> %1 &nbsp;&nbsp;|&nbsp;&nbsp; <b>Môn:</b> %2 - %3<br>"
                "<b>Niên khóa:</b> %4 &nbsp;&nbsp;|&nbsp;&nbsp; <b>Học kỳ:</b> %5 &nbsp;&nbsp;|&nbsp;&nbsp; <b>Nhóm:</b> %6")
            .arg(ltc->MALOPTC)
            .arg(ltc->MAMH)
            .arg(tenMH)
            .arg(ltc->NienKhoa)
            .arg(ltc->HocKy)
            .arg(ltc->Nhom),
        &dlg);
    layout->addWidget(lblTitle);

    QTableWidget *tblSV = new QTableWidget(&dlg);
    tblSV->setColumnCount(7);
    tblSV->setHorizontalHeaderLabels({"STT", "Mã SV", "Họ", "Tên", "Phái", "Số ĐT", "Mã Lớp"});
    tblSV->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblSV->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tblSV->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tblSV->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tblSV->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tblSV->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tblSV->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    tblSV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblSV->setAlternatingRowColors(true);
    tblSV->verticalHeader()->setVisible(false);

    // Thu thập danh sách sinh viên hợp lệ và sắp xếp theo Mã SV
    struct TempSVDK {
        char MASV[16];
        char HO[51];
        char TEN[16];
        char PHAI[5];
        char SODT[16];
        char MALOP[16];
    };

    TempSVDK listSV[1000];
    int count = 0;

    PTR_DangKy p = ltc->FirstDK;
    while (p != nullptr && count < 1000) {
        if (!p->dk.HuyDangKy) {
            LOPSV *lopChuaSV = nullptr;
            PTRSV pSV = TimSVToanTruong(dsLop, p->dk.MASV, lopChuaSV);
            if (pSV != nullptr && lopChuaSV != nullptr) {
                std::strcpy(listSV[count].MASV, pSV->sv.MASV);
                std::strcpy(listSV[count].HO, pSV->sv.HO);
                std::strcpy(listSV[count].TEN, pSV->sv.TEN);
                std::strcpy(listSV[count].PHAI, pSV->sv.PHAI);
                std::strcpy(listSV[count].SODT, pSV->sv.SODT);
                std::strcpy(listSV[count].MALOP, lopChuaSV->MALOP);
                count++;
            }
        }
        p = p->next;
    }

    // QuickSort theo MASV theo đúng Câu B
    for (int i = 0; i < count - 1; ++i) {
        for (int j = i + 1; j < count; ++j) {
            if (strcmp(listSV[i].MASV, listSV[j].MASV) > 0) {
                TempSVDK tmp = listSV[i];
                listSV[i] = listSV[j];
                listSV[j] = tmp;
            }
        }
    }

    tblSV->setRowCount(count);
    for (int i = 0; i < count; ++i) {
        QTableWidgetItem *itemSTT = new QTableWidgetItem(QString::number(i + 1));
        itemSTT->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemMa = new QTableWidgetItem(QString::fromUtf8(listSV[i].MASV));
        itemMa->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemHo = new QTableWidgetItem(QString::fromUtf8(listSV[i].HO));
        QTableWidgetItem *itemTen = new QTableWidgetItem(QString::fromUtf8(listSV[i].TEN));

        QTableWidgetItem *itemPhai = new QTableWidgetItem(QString::fromUtf8(listSV[i].PHAI));
        itemPhai->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemSDT = new QTableWidgetItem(QString::fromUtf8(listSV[i].SODT));
        itemSDT->setTextAlignment(Qt::AlignCenter);

        QTableWidgetItem *itemLop = new QTableWidgetItem(QString::fromUtf8(listSV[i].MALOP));
        itemLop->setTextAlignment(Qt::AlignCenter);

        tblSV->setItem(i, 0, itemSTT);
        tblSV->setItem(i, 1, itemMa);
        tblSV->setItem(i, 2, itemHo);
        tblSV->setItem(i, 3, itemTen);
        tblSV->setItem(i, 4, itemPhai);
        tblSV->setItem(i, 5, itemSDT);
        tblSV->setItem(i, 6, itemLop);
    }

    layout->addWidget(tblSV);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QLabel *lblCount = new QLabel(QString("Tổng cộng: <b>%1</b> sinh viên đăng ký").arg(count), &dlg);
    QPushButton *btnClose = new QPushButton("Đóng", &dlg);
    btnClose->setStyleSheet("background-color: #64748b; color: white; padding: 6px 18px; font-weight: bold; border-radius: 4px;");
    connect(btnClose, &QPushButton::clicked, &dlg, &QDialog::accept);

    btnLayout->addWidget(lblCount);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);
    layout->addLayout(btnLayout);

    dlg.exec();
}

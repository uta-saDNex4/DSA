#include "TabLopSinhVien.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <cstring>

TabLopSinhVien::TabLopSinhVien(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadData();
}

void TabLopSinhVien::setupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // ==========================================
    // LEFT PANEL: DANH SÁCH LỚP HỌC
    // ==========================================
    QWidget *leftPanel = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10);

    QLabel *lblLopTitle = new QLabel("📋 DANH SÁCH LỚP SINH VIÊN", leftPanel);
    lblLopTitle->setStyleSheet("font-weight: bold; font-size: 14px; color: #740d18;");
    leftLayout->addWidget(lblLopTitle);

    searchLopEdit = new QLineEdit(leftPanel);
    searchLopEdit->setPlaceholderText("🔍 Tìm mã hoặc tên lớp...");
    searchLopEdit->setClearButtonEnabled(true);
    connect(searchLopEdit, &QLineEdit::textChanged, this, &TabLopSinhVien::onSearchClass);
    leftLayout->addWidget(searchLopEdit);

    listLop = new QListWidget(leftPanel);
    listLop->setStyleSheet("QListWidget::item { padding: 8px; border-bottom: 1px solid #e2e8f0; } "
                           "QListWidget::item:selected { background-color: #fbe6e8; color: #740d18; font-weight: bold; }");
    connect(listLop, &QListWidget::currentRowChanged, this, &TabLopSinhVien::onClassSelected);
    leftLayout->addWidget(listLop);

    QHBoxLayout *lopBtnLayout = new QHBoxLayout();
    QPushButton *btnThemLop = new QPushButton("➕ Thêm Lớp", leftPanel);
    QPushButton *btnSuaLop = new QPushButton("✏️ Sửa Tên", leftPanel);
    QPushButton *btnXoaLop = new QPushButton("🗑️ Xóa Lớp", leftPanel);

    btnThemLop->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 4px; padding: 6px;");
    btnSuaLop->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 4px; padding: 6px;");
    btnXoaLop->setStyleSheet("background-color: #dc2626; color: white; font-weight: bold; border-radius: 4px; padding: 6px;");

    connect(btnThemLop, &QPushButton::clicked, this, &TabLopSinhVien::onThemLop);
    connect(btnSuaLop, &QPushButton::clicked, this, &TabLopSinhVien::onSuaLop);
    connect(btnXoaLop, &QPushButton::clicked, this, &TabLopSinhVien::onXoaLop);

    lopBtnLayout->addWidget(btnThemLop);
    lopBtnLayout->addWidget(btnSuaLop);
    lopBtnLayout->addWidget(btnXoaLop);
    leftLayout->addLayout(lopBtnLayout);

    leftPanel->setMinimumWidth(300);
    splitter->addWidget(leftPanel);

    // ==========================================
    // RIGHT PANEL: SINH VIÊN CỦA LỚP
    // ==========================================
    QWidget *rightPanel = new QWidget(this);
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10);

    // Header info
    classHeaderLabel = new QLabel("Vui lòng chọn một lớp để xem danh sách sinh viên", rightPanel);
    classHeaderLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #1e293b; padding: 4px 0;");
    rightLayout->addWidget(classHeaderLabel);

    // Filter and Sort bar
    QHBoxLayout *filterSVLayout = new QHBoxLayout();
    searchSVEdit = new QLineEdit(rightPanel);
    searchSVEdit->setPlaceholderText("🔍 Tìm theo Mã SV hoặc Họ tên...");
    searchSVEdit->setClearButtonEnabled(true);
    connect(searchSVEdit, &QLineEdit::textChanged, this, &TabLopSinhVien::onSearchSV);
    filterSVLayout->addWidget(searchSVEdit, 2);

    QGroupBox *grpSort = new QGroupBox("Chế độ sắp xếp:", rightPanel);
    QHBoxLayout *sortLayout = new QHBoxLayout(grpSort);
    sortLayout->setContentsMargins(8, 4, 8, 4);
    radSortTenHo = new QRadioButton("Tăng dần theo Tên + Họ", grpSort);
    radSortMaSV = new QRadioButton("Theo Mã Sinh Viên", grpSort);
    radSortTenHo->setChecked(true);
    connect(radSortTenHo, &QRadioButton::toggled, this, &TabLopSinhVien::onSortModeChanged);
    sortLayout->addWidget(radSortTenHo);
    sortLayout->addWidget(radSortMaSV);
    filterSVLayout->addWidget(grpSort);

    rightLayout->addLayout(filterSVLayout);

    // Table SV
    tableSV = new QTableWidget(rightPanel);
    tableSV->setColumnCount(6);
    tableSV->setHorizontalHeaderLabels({"STT", "Mã Sinh Viên", "Họ & Tên Đệm", "Tên", "Phái", "Số Điện Thoại"});
    tableSV->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableSV->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableSV->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableSV->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tableSV->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tableSV->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tableSV->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableSV->setSelectionMode(QAbstractItemView::SingleSelection);
    tableSV->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableSV->setAlternatingRowColors(true);
    tableSV->verticalHeader()->setVisible(false);
    rightLayout->addWidget(tableSV);

    // SV action buttons
    QHBoxLayout *svBtnLayout = new QHBoxLayout();
    QPushButton *btnThemSV = new QPushButton("➕ Thêm Sinh Viên", rightPanel);
    QPushButton *btnSuaSV = new QPushButton("✏️ Hiệu Chỉnh Sinh Viên", rightPanel);
    QPushButton *btnXoaSV = new QPushButton("🗑️ Xóa Sinh Viên", rightPanel);

    btnThemSV->setMinimumHeight(34);
    btnThemSV->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");
    btnSuaSV->setMinimumHeight(34);
    btnSuaSV->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");
    btnXoaSV->setMinimumHeight(34);
    btnXoaSV->setStyleSheet("background-color: #dc2626; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");

    connect(btnThemSV, &QPushButton::clicked, this, &TabLopSinhVien::onThemSV);
    connect(btnSuaSV, &QPushButton::clicked, this, &TabLopSinhVien::onSuaSV);
    connect(btnXoaSV, &QPushButton::clicked, this, &TabLopSinhVien::onXoaSV);

    statusSVLabel = new QLabel(rightPanel);
    statusSVLabel->setStyleSheet("color: #64748b; font-size: 13px; font-weight: 500;");

    svBtnLayout->addWidget(btnThemSV);
    svBtnLayout->addWidget(btnSuaSV);
    svBtnLayout->addWidget(btnXoaSV);
    svBtnLayout->addStretch();
    svBtnLayout->addWidget(statusSVLabel);

    rightLayout->addLayout(svBtnLayout);

    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    mainLayout->addWidget(splitter);
}

void TabLopSinhVien::loadData() {
    DS_LOPSV &dsLop = AppData::instance().dsLop;
    listLop->clear();
    QString filter = searchLopEdit->text().trimmed().toLower();

    for (int i = 0; i < dsLop.n; i++) {
        QString ma = QString::fromUtf8(dsLop.nodes[i].MALOP);
        QString ten = QString::fromUtf8(dsLop.nodes[i].TENLOP);
        int soSV = DemSV(dsLop.nodes[i].FirstSV);

        if (!filter.isEmpty() && !ma.toLower().contains(filter) && !ten.toLower().contains(filter)) {
            continue;
        }

        QListWidgetItem *item = new QListWidgetItem(QString("%1 - %2 (%3 SV)").arg(ma).arg(ten).arg(soSV), listLop);
        item->setData(Qt::UserRole, i);
    }

    if (listLop->count() > 0) {
        listLop->setCurrentRow(0);
    } else {
        classHeaderLabel->setText("Chưa có lớp nào trong hệ thống");
        tableSV->setRowCount(0);
        statusSVLabel->setText("");
    }
}

void TabLopSinhVien::onSearchClass(const QString &) {
    loadData();
}

void TabLopSinhVien::onClassSelected(int row) {
    if (row < 0 || row >= listLop->count()) {
        currentClassIdx = -1;
        classHeaderLabel->setText("Chưa chọn lớp");
        tableSV->setRowCount(0);
        statusSVLabel->setText("");
        return;
    }

    currentClassIdx = listLop->item(row)->data(Qt::UserRole).toInt();
    loadSinhVienCuaLop(currentClassIdx);
}

void TabLopSinhVien::onSortModeChanged() {
    if (currentClassIdx >= 0) {
        loadSinhVienCuaLop(currentClassIdx);
    }
}

void TabLopSinhVien::onSearchSV(const QString &) {
    if (currentClassIdx >= 0) {
        loadSinhVienCuaLop(currentClassIdx);
    }
}

// Cấu trúc hỗ trợ sắp xếp theo Mã SV (Câu d)
struct SV_SortMa {
    SinhVien sv;
};

static void QuickSortSVMa(SV_SortMa arr[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        SV_SortMa t = arr[mid]; arr[mid] = arr[high]; arr[high] = t;
        char pivot[16];
        strcpy(pivot, arr[high].sv.MASV);

        int i = low - 1;
        for (int j = low; j < high; j++) {
            if (stricmp(arr[j].sv.MASV, pivot) < 0) {
                i++;
                SV_SortMa temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }
        }
        SV_SortMa temp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = temp;
        int pi = i + 1;
        QuickSortSVMa(arr, low, pi - 1);
        QuickSortSVMa(arr, pi + 1, high);
    }
}

void TabLopSinhVien::loadSinhVienCuaLop(int classIdx) {
    DS_LOPSV &dsLop = AppData::instance().dsLop;
    if (classIdx < 0 || classIdx >= dsLop.n) return;

    LOPSV &lop = dsLop.nodes[classIdx];
    int count = DemSV(lop.FirstSV);
    classHeaderLabel->setText(QString("LỚP: %1 - %2  (Sĩ số: %3 sinh viên)")
        .arg(QString::fromUtf8(lop.MALOP))
        .arg(QString::fromUtf8(lop.TENLOP))
        .arg(count));

    tableSV->setRowCount(0);
    if (count == 0) {
        statusSVLabel->setText("Lớp này chưa có sinh viên nào.");
        return;
    }

    // Đưa sinh viên vào mảng để xử lý hiển thị / sắp xếp
    SV_SortMa *arr = new SV_SortMa[count];
    int k = 0;
    for (PTRSV p = lop.FirstSV; p != nullptr; p = p->next) {
        arr[k++].sv = p->sv;
    }

    if (radSortMaSV->isChecked()) {
        QuickSortSVMa(arr, 0, count - 1);
    }

    QString keyword = searchSVEdit->text().trimmed().toLower();
    int stt = 1;

    for (int i = 0; i < count; i++) {
        QString ma = QString::fromUtf8(arr[i].sv.MASV);
        QString ho = QString::fromUtf8(arr[i].sv.HO);
        QString ten = QString::fromUtf8(arr[i].sv.TEN);
        QString hoTen = ho + " " + ten;
        QString phai = QString::fromUtf8(arr[i].sv.PHAI);
        QString sdt = QString::fromUtf8(arr[i].sv.SODT);

        if (!keyword.isEmpty() && !ma.toLower().contains(keyword) && !hoTen.toLower().contains(keyword)) {
            continue;
        }

        int row = tableSV->rowCount();
        tableSV->insertRow(row);

        tableSV->setItem(row, 0, new QTableWidgetItem(QString::number(stt++)));
        tableSV->setItem(row, 1, new QTableWidgetItem(ma));
        tableSV->setItem(row, 2, new QTableWidgetItem(ho));
        tableSV->setItem(row, 3, new QTableWidgetItem(ten));
        tableSV->setItem(row, 4, new QTableWidgetItem(phai));
        tableSV->setItem(row, 5, new QTableWidgetItem(sdt));

        tableSV->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        tableSV->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        tableSV->item(row, 4)->setTextAlignment(Qt::AlignCenter);
        tableSV->item(row, 5)->setTextAlignment(Qt::AlignCenter);
    }

    delete[] arr;
    statusSVLabel->setText(QString("Hiển thị %1 / %2 sinh viên").arg(tableSV->rowCount()).arg(count));
}

void TabLopSinhVien::onThemLop() {
    QDialog dlg(this);
    dlg.setWindowTitle("Thêm Lớp Sinh Viên Mới");
    dlg.resize(400, 180);

    QFormLayout *form = new QFormLayout(&dlg);
    form->setSpacing(10);

    QLineEdit *edtMa = new QLineEdit(&dlg);
    edtMa->setMaxLength(15);
    edtMa->setPlaceholderText("VD: D24CQCN01");

    QLineEdit *edtTen = new QLineEdit(&dlg);
    edtTen->setMaxLength(50);
    edtTen->setPlaceholderText("VD: Cong nghe thong tin 1");

    form->addRow("Mã lớp (C15):", edtMa);
    form->addRow("Tên lớp (C50):", edtTen);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted, [&]() {
        QString ma = edtMa->text().trimmed().toUpper();
        QString ten = edtTen->text().trimmed();

        if (ma.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Mã lớp không được để trống!");
            return;
        }
        if (ten.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Tên lớp không được để trống!");
            return;
        }

        DS_LOPSV &dsLop = AppData::instance().dsLop;
        char cMa[16];
        strncpy(cMa, ma.toUtf8().constData(), sizeof(cMa) - 1);
        cMa[sizeof(cMa) - 1] = '\0';
        ChuanHoaMa(cMa);

        if (TimLop(dsLop, cMa) != -1) {
            QMessageBox::warning(&dlg, "Trùng mã lớp", QString("Mã lớp '%1' đã tồn tại!").arg(cMa));
            return;
        }

        LOPSV lop;
        strcpy(lop.MALOP, cMa);
        strncpy(lop.TENLOP, ten.toUtf8().constData(), sizeof(lop.TENLOP) - 1);
        lop.TENLOP[sizeof(lop.TENLOP) - 1] = '\0';
        ChuanHoaTen(lop.TENLOP);
        lop.FirstSV = nullptr;

        ThemLop(dsLop, lop);
        AppData::instance().save();
        dlg.accept();
    });

    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        loadData();
        QMessageBox::information(this, "Thành công", "Đã thêm lớp học mới thành công!");
    }
}

void TabLopSinhVien::onSuaLop() {
    if (currentClassIdx < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn lớp cần đổi tên!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    LOPSV &lop = dsLop.nodes[currentClassIdx];

    QDialog dlg(this);
    dlg.setWindowTitle(QString("Đổi Tên Lớp - %1").arg(QString::fromUtf8(lop.MALOP)));
    dlg.resize(400, 160);

    QFormLayout *form = new QFormLayout(&dlg);
    form->setSpacing(10);

    QLineEdit *edtMa = new QLineEdit(QString::fromUtf8(lop.MALOP), &dlg);
    edtMa->setReadOnly(true);
    edtMa->setStyleSheet("background-color: #f1f5f9; color: #475569;");

    QLineEdit *edtTen = new QLineEdit(QString::fromUtf8(lop.TENLOP), &dlg);
    edtTen->setMaxLength(50);

    form->addRow("Mã lớp:", edtMa);
    form->addRow("Tên lớp mới:", edtTen);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted, [&]() {
        QString ten = edtTen->text().trimmed();
        if (ten.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Tên lớp không được để trống!");
            return;
        }
        strncpy(lop.TENLOP, ten.toUtf8().constData(), sizeof(lop.TENLOP) - 1);
        lop.TENLOP[sizeof(lop.TENLOP) - 1] = '\0';
        ChuanHoaTen(lop.TENLOP);

        AppData::instance().save();
        dlg.accept();
    });

    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        loadData();
        QMessageBox::information(this, "Thành công", "Đã cập nhật tên lớp thành công!");
    }
}

void TabLopSinhVien::onXoaLop() {
    if (currentClassIdx < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn lớp cần xóa!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    DS_LopTC &dsLTC = AppData::instance().dsLTC;
    LOPSV &lop = dsLop.nodes[currentClassIdx];

    // Ràng buộc 1: Lớp còn sinh viên không?
    int soSV = DemSV(lop.FirstSV);
    if (soSV > 0) {
        QMessageBox::critical(this, "Ràng buộc toàn vẹn",
            QString("Không thể xóa lớp '%1' vì lớp đang có %2 sinh viên!\n"
                    "Vui lòng xóa hết sinh viên thuộc lớp trước khi xóa lớp.").arg(lop.MALOP).arg(soSV));
        return;
    }

    // Ràng buộc 2: Có sinh viên trong lớp đang đăng ký tín chỉ không?
    int soDK = DemDangKyByLop(dsLTC, lop.FirstSV);
    if (soDK > 0) {
        QMessageBox::critical(this, "Ràng buộc toàn vẹn",
            QString("Không thể xóa lớp '%1' vì có %2 bản ghi đăng ký tín chỉ liên quan!").arg(lop.MALOP).arg(soDK));
        return;
    }

    auto ret = QMessageBox::question(this, "Xác nhận xóa",
        QString("Bạn có chắc chắn muốn xóa lớp '%1 - %2' không?").arg(lop.MALOP).arg(lop.TENLOP),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        XoaLop(dsLop, lop.MALOP, dsLTC);
        AppData::instance().save();
        currentClassIdx = -1;
        loadData();
        QMessageBox::information(this, "Thành công", "Đã xóa lớp thành công!");
    }
}

void TabLopSinhVien::onThemSV() {
    if (currentClassIdx < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một lớp học trước khi thêm sinh viên!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    LOPSV &lop = dsLop.nodes[currentClassIdx];

    QDialog dlg(this);
    dlg.setWindowTitle(QString("Thêm Sinh Viên - Lớp %1").arg(QString::fromUtf8(lop.MALOP)));
    dlg.resize(440, 260);

    QFormLayout *form = new QFormLayout(&dlg);
    form->setSpacing(10);

    QLineEdit *edtMa = new QLineEdit(&dlg);
    edtMa->setMaxLength(15);
    edtMa->setPlaceholderText("VD: N24DCCN001");

    QLineEdit *edtHo = new QLineEdit(&dlg);
    edtHo->setMaxLength(50);
    edtHo->setPlaceholderText("VD: Nguyen Van");

    QLineEdit *edtTen = new QLineEdit(&dlg);
    edtTen->setMaxLength(15);
    edtTen->setPlaceholderText("VD: An");

    QComboBox *cboPhai = new QComboBox(&dlg);
    cboPhai->addItems({"NAM", "NU"});

    QLineEdit *edtSDT = new QLineEdit(&dlg);
    edtSDT->setMaxLength(15);
    edtSDT->setPlaceholderText("VD: 0901234567");

    form->addRow("Mã sinh viên (C15):", edtMa);
    form->addRow("Họ & tên đệm (C50):", edtHo);
    form->addRow("Tên sinh viên (C15):", edtTen);
    form->addRow("Phái:", cboPhai);
    form->addRow("Số điện thoại (C15):", edtSDT);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted, [&]() {
        QString ma = edtMa->text().trimmed().toUpper();
        QString ho = edtHo->text().trimmed();
        QString ten = edtTen->text().trimmed();
        QString sdt = edtSDT->text().trimmed();

        if (ma.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Mã sinh viên không được để trống!");
            return;
        }
        if (ho.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Họ sinh viên không được để trống!");
            return;
        }
        if (ten.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Tên sinh viên không được để trống!");
            return;
        }

        char cMa[16];
        strncpy(cMa, ma.toUtf8().constData(), sizeof(cMa) - 1);
        cMa[sizeof(cMa) - 1] = '\0';
        ChuanHoaMa(cMa);

        // Kiểm tra trùng mã SV trên TOÀN TRƯỜNG
        LOPSV* lopTrung = nullptr;
        if (TimSVToanTruong(dsLop, cMa, lopTrung) != nullptr) {
            QString tenLopTrung = lopTrung ? QString::fromUtf8(lopTrung->MALOP) : "";
            QMessageBox::warning(&dlg, "Trùng Mã SV",
                QString("Mã sinh viên '%1' đã tồn tại ở lớp '%2'!\nMã sinh viên là duy nhất trên toàn trường.")
                    .arg(cMa).arg(tenLopTrung));
            return;
        }

        SinhVien sv;
        strcpy(sv.MASV, cMa);
        strncpy(sv.HO, ho.toUtf8().constData(), sizeof(sv.HO) - 1);
        sv.HO[sizeof(sv.HO) - 1] = '\0';
        ChuanHoaTen(sv.HO);

        strncpy(sv.TEN, ten.toUtf8().constData(), sizeof(sv.TEN) - 1);
        sv.TEN[sizeof(sv.TEN) - 1] = '\0';
        ChuanHoaTen(sv.TEN);

        strcpy(sv.PHAI, cboPhai->currentText().toUtf8().constData());

        strncpy(sv.SODT, sdt.toUtf8().constData(), sizeof(sv.SODT) - 1);
        sv.SODT[sizeof(sv.SODT) - 1] = '\0';
        ChuanHoaMa(sv.SODT);

        // Chèn vào DSLK đơn có thứ tự tăng dần theo Tên + Họ
        InsertOrderSV(lop.FirstSV, sv);
        AppData::instance().save();
        dlg.accept();
    });

    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        loadSinhVienCuaLop(currentClassIdx);
        // Cập nhật số lượng SV trên danh sách lớp
        listLop->item(listLop->currentRow())->setText(
            QString("%1 - %2 (%3 SV)").arg(lop.MALOP).arg(lop.TENLOP).arg(DemSV(lop.FirstSV)));
        QMessageBox::information(this, "Thành công", "Đã thêm sinh viên vào lớp thành công (tự động xếp theo Tên + Họ)!");
    }
}

void TabLopSinhVien::onSuaSV() {
    int row = tableSV->currentRow();
    if (row < 0 || currentClassIdx < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một sinh viên trên bảng để sửa!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    LOPSV &lop = dsLop.nodes[currentClassIdx];

    QString maSV = tableSV->item(row, 1)->text();
    char cMa[16];
    strncpy(cMa, maSV.toUtf8().constData(), sizeof(cMa) - 1);
    cMa[sizeof(cMa) - 1] = '\0';

    PTRSV p = TimSV(lop.FirstSV, cMa);
    if (!p) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy sinh viên trong lớp!");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(QString("Hiệu Chỉnh Sinh Viên - %1").arg(cMa));
    dlg.resize(440, 260);

    QFormLayout *form = new QFormLayout(&dlg);
    form->setSpacing(10);

    QLineEdit *edtMa = new QLineEdit(cMa, &dlg);
    edtMa->setReadOnly(true);
    edtMa->setStyleSheet("background-color: #f1f5f9; color: #475569;");

    QLineEdit *edtHo = new QLineEdit(QString::fromUtf8(p->sv.HO), &dlg);
    edtHo->setMaxLength(50);

    QLineEdit *edtTen = new QLineEdit(QString::fromUtf8(p->sv.TEN), &dlg);
    edtTen->setMaxLength(15);

    QComboBox *cboPhai = new QComboBox(&dlg);
    cboPhai->addItems({"NAM", "NU"});
    cboPhai->setCurrentText(QString::fromUtf8(p->sv.PHAI).toUpper());

    QLineEdit *edtSDT = new QLineEdit(QString::fromUtf8(p->sv.SODT), &dlg);
    edtSDT->setMaxLength(15);

    form->addRow("Mã sinh viên:", edtMa);
    form->addRow("Họ & tên đệm:", edtHo);
    form->addRow("Tên sinh viên:", edtTen);
    form->addRow("Phái:", cboPhai);
    form->addRow("Số điện thoại:", edtSDT);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted, [&]() {
        QString ho = edtHo->text().trimmed();
        QString ten = edtTen->text().trimmed();
        QString sdt = edtSDT->text().trimmed();

        if (ho.isEmpty() || ten.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi", "Họ và tên không được để trống!");
            return;
        }

        SinhVien svMoi;
        strcpy(svMoi.MASV, cMa);
        strncpy(svMoi.HO, ho.toUtf8().constData(), sizeof(svMoi.HO) - 1);
        svMoi.HO[sizeof(svMoi.HO) - 1] = '\0';
        ChuanHoaTen(svMoi.HO);

        strncpy(svMoi.TEN, ten.toUtf8().constData(), sizeof(svMoi.TEN) - 1);
        svMoi.TEN[sizeof(svMoi.TEN) - 1] = '\0';
        ChuanHoaTen(svMoi.TEN);

        strcpy(svMoi.PHAI, cboPhai->currentText().toUtf8().constData());
        strncpy(svMoi.SODT, sdt.toUtf8().constData(), sizeof(svMoi.SODT) - 1);
        svMoi.SODT[sizeof(svMoi.SODT) - 1] = '\0';
        ChuanHoaMa(svMoi.SODT);

        // Nếu họ tên thay đổi, xóa node cũ và InsertOrderSV để đảm bảo thứ tự
        XoaSVNoiB(lop.FirstSV, cMa);
        InsertOrderSV(lop.FirstSV, svMoi);

        AppData::instance().save();
        dlg.accept();
    });

    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        loadSinhVienCuaLop(currentClassIdx);
        QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin sinh viên thành công!");
    }
}

void TabLopSinhVien::onXoaSV() {
    int row = tableSV->currentRow();
    if (row < 0 || currentClassIdx < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn sinh viên cần xóa!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    DS_LopTC &dsLTC = AppData::instance().dsLTC;
    LOPSV &lop = dsLop.nodes[currentClassIdx];

    QString maSV = tableSV->item(row, 1)->text();
    QString hoTen = tableSV->item(row, 2)->text() + " " + tableSV->item(row, 3)->text();
    char cMa[16];
    strncpy(cMa, maSV.toUtf8().constData(), sizeof(cMa) - 1);
    cMa[sizeof(cMa) - 1] = '\0';

    // Ràng buộc: SV có điểm thi hoặc đang đăng ký tín chỉ không?
    if (SVDaCoDiem(dsLTC, cMa)) {
        QMessageBox::critical(this, "Ràng buộc toàn vẹn",
            QString("Không thể xóa sinh viên '%1'!\nSinh viên đã có điểm thi ở các lớp tín chỉ.").arg(maSV));
        return;
    }

    int soDK = DemDangKyByMASV(dsLTC, cMa);
    if (soDK > 0) {
        QMessageBox::critical(this, "Ràng buộc toàn vẹn",
            QString("Không thể xóa sinh viên '%1'!\nSinh viên đang đăng ký %2 lớp tín chỉ.").arg(maSV).arg(soDK));
        return;
    }

    auto ret = QMessageBox::question(this, "Xác nhận xóa sinh viên",
        QString("Bạn có chắc chắn muốn xóa sinh viên sau khỏi lớp không?\n\nMã SV: %1\nHọ tên: %2")
            .arg(maSV).arg(hoTen),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        XoaSVNoiB(lop.FirstSV, cMa);
        AppData::instance().save();
        loadSinhVienCuaLop(currentClassIdx);
        listLop->item(listLop->currentRow())->setText(
            QString("%1 - %2 (%3 SV)").arg(lop.MALOP).arg(lop.TENLOP).arg(DemSV(lop.FirstSV)));
        QMessageBox::information(this, "Thành công", QString("Đã xóa sinh viên '%1' thành công!").arg(maSV));
    }
}

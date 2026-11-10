#include "TabLopSinhVien.h"
#include "Toast.h"
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
#include <QKeyEvent>
#include <cstring>
#include "utils.h"

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

    listLop = new QListWidget(leftPanel);
    listLop->setStyleSheet("QListWidget::item { padding: 8px; border-bottom: 1px solid #e2e8f0; } "
                           "QListWidget::item:selected { background-color: #fbe6e8; color: #740d18; font-weight: bold; }");
    connect(listLop, &QListWidget::currentRowChanged, this, &TabLopSinhVien::onClassSelected);
    connect(listLop, &QListWidget::itemDoubleClicked, this, &TabLopSinhVien::onSuaLop);
    leftLayout->addWidget(listLop, 1);

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

    // Filter, Sort and Actions Bar (ALL ON TOP)
    QHBoxLayout *topToolBar = new QHBoxLayout();
    topToolBar->setSpacing(8);

    searchSVEdit = new QLineEdit(rightPanel);
    searchSVEdit->setPlaceholderText("🔍 Tìm theo Mã SV hoặc Họ tên...");
    searchSVEdit->setClearButtonEnabled(true);
    searchSVEdit->setMinimumHeight(34);
    connect(searchSVEdit, &QLineEdit::textChanged, this, &TabLopSinhVien::onSearchSV);
    topToolBar->addWidget(searchSVEdit, 2);

    QGroupBox *grpSort = new QGroupBox("Sắp xếp:", rightPanel);
    QHBoxLayout *sortLayout = new QHBoxLayout(grpSort);
    sortLayout->setContentsMargins(8, 2, 8, 2);
    sortLayout->setSpacing(8);
    radSortTenHo = new QRadioButton("Tên + Họ", grpSort);
    radSortMaSV = new QRadioButton("Mã SV", grpSort);
    radSortTenHo->setChecked(true);
    connect(radSortTenHo, &QRadioButton::toggled, this, &TabLopSinhVien::onSortModeChanged);
    sortLayout->addWidget(radSortTenHo);
    sortLayout->addWidget(radSortMaSV);
    topToolBar->addWidget(grpSort);

    QPushButton *btnThemSV = new QPushButton("➕ Thêm Sinh Viên", rightPanel);
    QPushButton *btnSuaSV = new QPushButton("✏️ Hiệu Chỉnh", rightPanel);
    QPushButton *btnXoaSV = new QPushButton("🗑️ Xóa", rightPanel);

    btnThemSV->setMinimumHeight(34);
    btnThemSV->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 6px; padding: 0 14px;");
    btnSuaSV->setMinimumHeight(34);
    btnSuaSV->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 6px; padding: 0 14px;");
    btnXoaSV->setMinimumHeight(34);
    btnXoaSV->setStyleSheet("background-color: #dc2626; color: white; font-weight: bold; border-radius: 6px; padding: 0 14px;");

    connect(btnThemSV, &QPushButton::clicked, this, &TabLopSinhVien::onThemSV);
    connect(btnSuaSV, &QPushButton::clicked, this, &TabLopSinhVien::onSuaSV);
    connect(btnXoaSV, &QPushButton::clicked, this, &TabLopSinhVien::onXoaSV);

    topToolBar->addWidget(btnThemSV);
    topToolBar->addWidget(btnSuaSV);
    topToolBar->addWidget(btnXoaSV);

    rightLayout->addLayout(topToolBar);

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
    tableSV->setStyleSheet(
        "QTableWidget { background-color: #ffffff; alternate-background-color: #f8fafc; color: #1e293b; } "
        "QTableWidget::item { color: #1e293b; } "
        "QTableWidget::item:selected { background-color: #fee2e2; color: #740d18; font-weight: 600; }"
    );
    tableSV->verticalHeader()->setVisible(false);
    tableSV->installEventFilter(this);
    connect(tableSV, &QTableWidget::cellDoubleClicked, this, &TabLopSinhVien::onSuaSV);
    rightLayout->addWidget(tableSV, 1);

    statusSVLabel = new QLabel(rightPanel);
    statusSVLabel->setStyleSheet("color: #64748b; font-size: 13px; font-weight: 500; padding: 4px 0;");
    rightLayout->addWidget(statusSVLabel);

    splitter->addWidget(rightPanel);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    mainLayout->addWidget(splitter);
}

bool TabLopSinhVien::eventFilter(QObject *obj, QEvent *event) {
    if (obj == tableSV && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent*>(event);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter) {
            onSuaSV();
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
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
        statusSVLabel->setText("💡 Lớp này chưa có sinh viên nào. Bấm nút [➕ Thêm Sinh Viên] ở thanh công cụ phía trên để bắt đầu thêm.");
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

    // Chống chớp nháy cho bảng sinh viên
    tableSV->setUpdatesEnabled(false);
    tableSV->blockSignals(true);

    QString keyword = searchSVEdit->text().trimmed().toLower();
    int* matching = new int[count > 0 ? count : 1];
    int totalMatch = 0;

    for (int i = 0; i < count; i++) {
        QString ma = QString::fromUtf8(arr[i].sv.MASV);
        QString ho = QString::fromUtf8(arr[i].sv.HO);
        QString ten = QString::fromUtf8(arr[i].sv.TEN);
        QString hoTen = ho + " " + ten;

        if (keyword.isEmpty() || ma.toLower().contains(keyword) || hoTen.toLower().contains(keyword)) {
            matching[totalMatch++] = i;
        }
    }

    tableSV->clearContents();
    tableSV->setRowCount(totalMatch);

    for (int r = 0; r < totalMatch; r++) {
        int i = matching[r];
        QString ma = QString::fromUtf8(arr[i].sv.MASV);
        QString ho = QString::fromUtf8(arr[i].sv.HO);
        QString ten = QString::fromUtf8(arr[i].sv.TEN);
        QString phai = QString::fromUtf8(arr[i].sv.PHAI);
        QString sdt = QString::fromUtf8(arr[i].sv.SODT);

        tableSV->setItem(r, 0, new QTableWidgetItem(QString::number(r + 1)));
        tableSV->setItem(r, 1, new QTableWidgetItem(ma));
        tableSV->setItem(r, 2, new QTableWidgetItem(ho));
        tableSV->setItem(r, 3, new QTableWidgetItem(ten));
        tableSV->setItem(r, 4, new QTableWidgetItem(phai));
        tableSV->setItem(r, 5, new QTableWidgetItem(sdt));

        tableSV->item(r, 0)->setTextAlignment(Qt::AlignCenter);
        tableSV->item(r, 1)->setTextAlignment(Qt::AlignCenter);
        tableSV->item(r, 4)->setTextAlignment(Qt::AlignCenter);
        tableSV->item(r, 5)->setTextAlignment(Qt::AlignCenter);
    }

    delete[] matching;
    delete[] arr;

    tableSV->blockSignals(false);
    tableSV->setUpdatesEnabled(true);

    statusSVLabel->setText(QString("Hiển thị %1 / %2 sinh viên").arg(totalMatch).arg(count));
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
        Toast::showSuccess(this, "Đã thêm lớp học mới! (Tự động lưu)");
    }
}

void TabLopSinhVien::onSuaLop() {
    if (currentClassIdx < 0) {
        Toast::showInfo(this, "Vui lòng chọn lớp cần đổi tên!");
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
        Toast::showSuccess(this, "Đã cập nhật tên lớp! (Tự động lưu)");
    }
}

void TabLopSinhVien::onXoaLop() {
    if (currentClassIdx < 0) {
        Toast::showInfo(this, "Vui lòng chọn lớp cần xóa!");
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
        Toast::showSuccess(this, "Đã xóa lớp thành công! (Tự động lưu)");
    }
}

void TabLopSinhVien::onThemSV() {
    if (currentClassIdx < 0) {
        Toast::showInfo(this, "Vui lòng chọn một lớp học trước khi thêm sinh viên!");
        return;
    }

    DS_LOPSV &dsLop = AppData::instance().dsLop;
    LOPSV &lop = dsLop.nodes[currentClassIdx];

    QDialog dlg(this);
    dlg.setWindowTitle(QString("➕ Nhập Danh Sách Sinh Viên - Lớp %1").arg(QString::fromUtf8(lop.MALOP)));
    dlg.resize(480, 310);

    QVBoxLayout *mainLayout = new QVBoxLayout(&dlg);
    mainLayout->setContentsMargins(16, 14, 16, 14);
    mainLayout->setSpacing(10);

    // Banner hướng dẫn
    QLabel *lblHeader = new QLabel(
        QString("🎓 <b>LỚP: %1 - %2</b> (Sĩ số hiện tại: <b>%3</b> SV)<br>"
                "<span style='color: #475569; font-size: 12px;'>"
                "💡 <b>Chế độ nhập liên tục:</b> Nhập thông tin và bấm <b>[➕ Lưu & Nhập Tiếp (Enter)]</b> để thêm liên tục nhiều SV.<br>"
                "Quá trình nhập sẽ <b>DỪNG</b> khi để trống <b>Mã SV</b> và bấm Enter (hoặc bấm nút [⏹️ Dừng Nhập / Hoàn Tất]).</span>")
            .arg(QString::fromUtf8(lop.MALOP))
            .arg(QString::fromUtf8(lop.TENLOP))
            .arg(DemSV(lop.FirstSV)), &dlg);
    lblHeader->setStyleSheet("background-color: #f1f5f9; padding: 10px; border-radius: 6px; border-left: 4px solid #be3142;");
    mainLayout->addWidget(lblHeader);

    // Form nhập liệu
    QFormLayout *form = new QFormLayout();
    form->setSpacing(8);

    QLineEdit *edtMa = new QLineEdit(&dlg);
    edtMa->setMaxLength(15);
    edtMa->setPlaceholderText("VD: N24DCCN001 (để trống & bấm Enter để dừng)");

    QLineEdit *edtHo = new QLineEdit(&dlg);
    edtHo->setMaxLength(50);
    edtHo->setPlaceholderText("VD: Nguyen Van");

    QLineEdit *edtTen = new QLineEdit(&dlg);
    edtTen->setMaxLength(50);
    edtTen->setPlaceholderText("VD: An (hoặc Nguyen Van An)");

    QComboBox *cboPhai = new QComboBox(&dlg);
    cboPhai->addItems({"NAM", "NU"});

    QLineEdit *edtSDT = new QLineEdit(&dlg);
    edtSDT->setMaxLength(15);
    edtSDT->setPlaceholderText("VD: 0901234567");

    form->addRow("Mã sinh viên (*):", edtMa);
    form->addRow("Họ & tên đệm (*):", edtHo);
    form->addRow("Tên sinh viên (*):", edtTen);
    form->addRow("Phái:", cboPhai);
    form->addRow("Số điện thoại (*):", edtSDT);
    mainLayout->addLayout(form);

    // Status label
    QLabel *lblStatus = new QLabel("Sẵn sàng nhập sinh viên...", &dlg);
    lblStatus->setStyleSheet("color: #64748b; font-size: 12px; font-weight: 500;");
    mainLayout->addWidget(lblStatus);

    // Hàng nút bấm
    QHBoxLayout *btnLayout = new QHBoxLayout();
    QPushButton *btnLuuTiep = new QPushButton("➕ Lưu & Nhập Tiếp (Enter)", &dlg);
    btnLuuTiep->setMinimumHeight(32);
    btnLuuTiep->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");

    QPushButton *btnDung = new QPushButton("⏹️ Dừng Nhập / Hoàn Tất", &dlg);
    btnDung->setMinimumHeight(32);
    btnDung->setStyleSheet("background-color: #64748b; color: white; font-weight: bold; border-radius: 4px; padding: 0 16px;");

    btnLayout->addWidget(btnLuuTiep);
    btnLayout->addWidget(btnDung);
    mainLayout->addLayout(btnLayout);



    int soLuongDaThem = 0;

    auto thucHienLuuSV = [&]() -> bool {
        QString ma = edtMa->text().trimmed().toUpper();
        QString hoInput = edtHo->text().trimmed();
        QString tenInput = edtTen->text().trimmed();
        QString sdt = edtSDT->text().trimmed();

        if (ma.isEmpty()) {
            return false;
        }

        char bufHo[128];
        char bufTen[128];
        strncpy(bufHo, hoInput.toUtf8().constData(), sizeof(bufHo) - 1);
        bufHo[sizeof(bufHo) - 1] = '\0';
        strncpy(bufTen, tenInput.toUtf8().constData(), sizeof(bufTen) - 1);
        bufTen[sizeof(bufTen) - 1] = '\0';

        TachVaChuanHoaHoTen(bufHo, bufTen, sizeof(SinhVien::HO), sizeof(SinhVien::TEN));

        if (KiemTraRong(bufHo)) {
            QMessageBox::warning(&dlg, "Lỗi", "Họ sinh viên không được để trống! (Nếu để trống ô Họ, vui lòng nhập đầy đủ cả Họ và Tên vào ô Tên)");
            edtHo->setFocus();
            return false;
        }
        if (KiemTraRong(bufTen)) {
            QMessageBox::warning(&dlg, "Lỗi", "Tên sinh viên không được để trống!");
            edtTen->setFocus();
            return false;
        }

        char cMa[16];
        strncpy(cMa, ma.toUtf8().constData(), sizeof(cMa) - 1);
        cMa[sizeof(cMa) - 1] = '\0';
        ChuanHoaMa(cMa);

        LOPSV* lopTrung = nullptr;
        if (TimSVToanTruong(dsLop, cMa, lopTrung) != nullptr) {
            QString tenLopTrung = lopTrung ? QString::fromUtf8(lopTrung->MALOP) : "";
            QMessageBox::warning(&dlg, "Trùng Mã SV",
                QString("Mã sinh viên '%1' đã tồn tại ở lớp '%2'!\nMã sinh viên là duy nhất trên toàn trường.")
                    .arg(cMa).arg(tenLopTrung));
            edtMa->selectAll();
            edtMa->setFocus();
            return false;
        }

        SinhVien sv;
        strcpy(sv.MASV, cMa);
        strcpy(sv.HO, bufHo);
        strcpy(sv.TEN, bufTen);
        strcpy(sv.PHAI, cboPhai->currentText().toUtf8().constData());
        strncpy(sv.SODT, sdt.toUtf8().constData(), sizeof(sv.SODT) - 1);
        sv.SODT[sizeof(sv.SODT) - 1] = '\0';
        ChuanHoaMa(sv.SODT);

        InsertOrderSV(lop.FirstSV, sv);
        AppData::instance().save();

        soLuongDaThem++;

        // Nút thêm kèm refresh ngay lập tức danh sách sinh viên đằng sau dialog để dễ theo dõi
        loadSinhVienCuaLop(currentClassIdx);
        if (listLop->currentItem()) {
            listLop->currentItem()->setText(
                QString("%1 - %2 (%3 SV)").arg(QString::fromUtf8(lop.MALOP))
                                         .arg(QString::fromUtf8(lop.TENLOP))
                                         .arg(DemSV(lop.FirstSV)));
        }
        lblHeader->setText(
            QString("🎓 <b>LỚP: %1 - %2</b> (Sĩ số hiện tại: <b>%3</b> SV)<br>"
                    "<span style='color: #475569; font-size: 12px;'>"
                    "💡 <b>Chế độ nhập liên tục:</b> Nhập thông tin và bấm <b>[➕ Lưu & Nhập Tiếp (Enter)]</b> để thêm liên tục nhiều SV.<br>"
                    "Quá trình nhập sẽ <b>DỪNG</b> khi để trống <b>Mã SV</b> và bấm Enter (hoặc bấm nút [⏹️ Dừng Nhập / Hoàn Tất]).</span>")
                .arg(QString::fromUtf8(lop.MALOP))
                .arg(QString::fromUtf8(lop.TENLOP))
                .arg(DemSV(lop.FirstSV)));

        lblStatus->setText(QString("✅ Đã thêm SV <b>%1</b> (%2 %3) thành công! Danh sách lớp đằng sau đã cập nhật.")
                               .arg(QString::fromUtf8(sv.MASV))
                               .arg(QString::fromUtf8(sv.HO))
                               .arg(QString::fromUtf8(sv.TEN)));
        lblStatus->setStyleSheet("color: #166534; font-size: 12px; font-weight: bold;");

        // Ép Qt cập nhật lại ngay tức thì giao diện trang đằng sau
        qApp->processEvents();

        edtMa->clear();
        edtHo->clear();
        edtTen->clear();
        edtSDT->clear();
        cboPhai->setCurrentIndex(0);
        edtMa->setFocus();
        return true;
    };

    connect(btnLuuTiep, &QPushButton::clicked, [&]() {
        if (edtMa->text().trimmed().isEmpty()) {
            if (soLuongDaThem > 0) {
                dlg.accept();
            } else {
                QMessageBox::information(&dlg, "Thông báo", "Vui lòng nhập Mã sinh viên! Hoặc bấm [⏹️ Dừng Nhập / Hoàn Tất] để đóng.");
                edtMa->setFocus();
            }
            return;
        }
        thucHienLuuSV();
    });

    connect(btnDung, &QPushButton::clicked, [&]() {
        dlg.accept();
    });

    connect(edtMa, &QLineEdit::returnPressed, [&]() {
        if (edtMa->text().trimmed().isEmpty()) {
            dlg.accept();
        } else {
            edtHo->setFocus();
        }
    });
    connect(edtHo, &QLineEdit::returnPressed, [&]() { edtTen->setFocus(); });
    connect(edtTen, &QLineEdit::returnPressed, [&]() { edtSDT->setFocus(); });
    connect(edtSDT, &QLineEdit::returnPressed, [&]() { btnLuuTiep->click(); });

    dlg.exec();

    loadSinhVienCuaLop(currentClassIdx);
    if (listLop->currentItem()) {
        listLop->currentItem()->setText(
            QString("%1 - %2 (%3 SV)").arg(lop.MALOP).arg(lop.TENLOP).arg(DemSV(lop.FirstSV)));
    }
    if (soLuongDaThem > 0) {
        Toast::showSuccess(this, QString("Đã thêm thành công %1 sinh viên vào lớp %2! (Đã tự động lưu)")
                                     .arg(soLuongDaThem).arg(QString::fromUtf8(lop.MALOP)));
    }
}

void TabLopSinhVien::onSuaSV() {
    int row = tableSV->currentRow();
    if (row < 0 || currentClassIdx < 0) {
        Toast::showInfo(this, "Vui lòng chọn một sinh viên trên bảng để sửa!");
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
    edtTen->setMaxLength(50);

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
        QString hoInput = edtHo->text().trimmed();
        QString tenInput = edtTen->text().trimmed();
        QString sdt = edtSDT->text().trimmed();

        // Tạo buffer tạm đủ lớn để xử lý tách họ tên trước khi kiểm tra hợp lệ
        char bufHo[128];
        char bufTen[128];
        strncpy(bufHo, hoInput.toUtf8().constData(), sizeof(bufHo) - 1);
        bufHo[sizeof(bufHo) - 1] = '\0';
        strncpy(bufTen, tenInput.toUtf8().constData(), sizeof(bufTen) - 1);
        bufTen[sizeof(bufTen) - 1] = '\0';

        // Gọi hàm dùng chung xử lý họ và tên trước khi kiểm tra hợp lệ
        TachVaChuanHoaHoTen(bufHo, bufTen, sizeof(SinhVien::HO), sizeof(SinhVien::TEN));

        if (KiemTraRong(bufHo)) {
            QMessageBox::warning(&dlg, "Lỗi", "Họ sinh viên không được để trống! (Nếu để trống ô Họ, vui lòng nhập đầy đủ cả Họ và Tên vào ô Tên)");
            return;
        }
        if (KiemTraRong(bufTen)) {
            QMessageBox::warning(&dlg, "Lỗi", "Tên sinh viên không được để trống!");
            return;
        }

        SinhVien svMoi;
        strcpy(svMoi.MASV, cMa);
        strcpy(svMoi.HO, bufHo);
        strcpy(svMoi.TEN, bufTen);

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
        Toast::showSuccess(this, "Đã cập nhật thông tin sinh viên! (Tự động lưu)");
    }
}

void TabLopSinhVien::onXoaSV() {
    int row = tableSV->currentRow();
    if (row < 0 || currentClassIdx < 0) {
        Toast::showInfo(this, "Vui lòng chọn sinh viên cần xóa!");
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
        Toast::showSuccess(this, QString("Đã xóa sinh viên '%1'! (Tự động lưu)").arg(maSV));
    }
}

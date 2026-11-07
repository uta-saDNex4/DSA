#include "TabMonHoc.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QFormLayout>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <cstring>

TabMonHoc::TabMonHoc(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadData();
}

void TabMonHoc::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // Toolbar
    QHBoxLayout *toolLayout = new QHBoxLayout();
    
    searchEdit = new QLineEdit(this);
    searchEdit->setPlaceholderText("🔍 Tìm kiếm môn học theo mã hoặc tên...");
    searchEdit->setClearButtonEnabled(true);
    searchEdit->setMinimumHeight(36);
    connect(searchEdit, &QLineEdit::textChanged, this, &TabMonHoc::onSearchChanged);

    QPushButton *btnThem = new QPushButton("➕ Thêm Môn Học", this);
    btnThem->setMinimumHeight(36);
    btnThem->setStyleSheet("background-color: #be3142; color: white; font-weight: bold; border-radius: 6px; padding: 0 16px;");
    connect(btnThem, &QPushButton::clicked, this, &TabMonHoc::onThemMonHoc);

    QPushButton *btnSua = new QPushButton("✏️ Hiệu Chỉnh", this);
    btnSua->setMinimumHeight(36);
    btnSua->setStyleSheet("background-color: #0284c7; color: white; font-weight: bold; border-radius: 6px; padding: 0 16px;");
    connect(btnSua, &QPushButton::clicked, this, &TabMonHoc::onSuaMonHoc);

    QPushButton *btnXoa = new QPushButton("🗑️ Xóa Môn", this);
    btnXoa->setMinimumHeight(36);
    btnXoa->setStyleSheet("background-color: #dc2626; color: white; font-weight: bold; border-radius: 6px; padding: 0 16px;");
    connect(btnXoa, &QPushButton::clicked, this, &TabMonHoc::onXoaMonHoc);

    QPushButton *btnReload = new QPushButton("🔄 Làm Mới", this);
    btnReload->setMinimumHeight(36);
    connect(btnReload, &QPushButton::clicked, this, &TabMonHoc::loadData);

    toolLayout->addWidget(searchEdit, 2);
    toolLayout->addWidget(btnThem);
    toolLayout->addWidget(btnSua);
    toolLayout->addWidget(btnXoa);
    toolLayout->addWidget(btnReload);

    mainLayout->addLayout(toolLayout);

    // Table
    table = new QTableWidget(this);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"STT", "Mã Môn Học", "Tên Môn Học", "STC Lý Thuyết", "STC Thực Hành", "Tổng STC"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);

    mainLayout->addWidget(table);

    // Status label
    statusLabel = new QLabel(this);
    statusLabel->setStyleSheet("color: #64748b; font-size: 13px; font-weight: 500;");
    mainLayout->addWidget(statusLabel);
}

void TabMonHoc::loadData() {
    DS_MonHoc &ds = AppData::instance().dsMH;
    if (ds.n == 0 || ds.root == nullptr) {
        table->setRowCount(0);
        statusLabel->setText("Danh sách môn học đang trống.");
        return;
    }

    TreeMonHoc *arr = new TreeMonHoc[ds.n];
    int idx = 0;
    InOrderTraversal(ds.root, arr, idx);
    if (idx > 1) {
        QuickSortMH(arr, 0, idx - 1);
    }

    hienThiDanhSach(arr, idx, searchEdit->text().trimmed());
    delete[] arr;
}

void TabMonHoc::hienThiDanhSach(TreeMonHoc* arr, int count, const QString &keyword) {
    table->setRowCount(0);
    int stt = 1;
    QString key = keyword.toLower();

    for (int i = 0; i < count; i++) {
        QString ma = QString::fromUtf8(arr[i]->mh.MAMH);
        QString ten = QString::fromUtf8(arr[i]->mh.TENMH);

        if (!key.isEmpty() && !ma.toLower().contains(key) && !ten.toLower().contains(key)) {
            continue;
        }

        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0, new QTableWidgetItem(QString::number(stt++)));
        table->setItem(row, 1, new QTableWidgetItem(ma));
        table->setItem(row, 2, new QTableWidgetItem(ten));
        table->setItem(row, 3, new QTableWidgetItem(QString::number(arr[i]->mh.STCLT)));
        table->setItem(row, 4, new QTableWidgetItem(QString::number(arr[i]->mh.STCTH)));
        table->setItem(row, 5, new QTableWidgetItem(QString::number(arr[i]->mh.STCLT + arr[i]->mh.STCTH)));

        table->item(row, 0)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 1)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 3)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 4)->setTextAlignment(Qt::AlignCenter);
        table->item(row, 5)->setTextAlignment(Qt::AlignCenter);
    }

    statusLabel->setText(QString("Hiển thị %1 / %2 môn học trên cây BST (đã sắp xếp tăng dần theo Tên)").arg(table->rowCount()).arg(count));
}

void TabMonHoc::onSearchChanged(const QString &text) {
    DS_MonHoc &ds = AppData::instance().dsMH;
    if (ds.n == 0 || ds.root == nullptr) return;

    TreeMonHoc *arr = new TreeMonHoc[ds.n];
    int idx = 0;
    InOrderTraversal(ds.root, arr, idx);
    if (idx > 1) {
        QuickSortMH(arr, 0, idx - 1);
    }
    hienThiDanhSach(arr, idx, text.trimmed());
    delete[] arr;
}

void TabMonHoc::onThemMonHoc() {
    QDialog dlg(this);
    dlg.setWindowTitle("Thêm Môn Học Mới (Cây BST)");
    dlg.resize(420, 260);

    QFormLayout *form = new QFormLayout(&dlg);
    form->setSpacing(10);

    QLineEdit *edtMa = new QLineEdit(&dlg);
    edtMa->setMaxLength(10);
    edtMa->setPlaceholderText("VD: CTDL, INT1414");

    QLineEdit *edtTen = new QLineEdit(&dlg);
    edtTen->setMaxLength(50);
    edtTen->setPlaceholderText("VD: Cau truc du lieu va giai thuat");

    QSpinBox *spnLT = new QSpinBox(&dlg);
    spnLT->setRange(0, 10);
    spnLT->setValue(3);

    QSpinBox *spnTH = new QSpinBox(&dlg);
    spnTH->setRange(0, 10);
    spnTH->setValue(1);

    form->addRow("Mã môn học (C10):", edtMa);
    form->addRow("Tên môn học (C50):", edtTen);
    form->addRow("Số TC lý thuyết:", spnLT);
    form->addRow("Số TC thực hành:", spnTH);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted, [&]() {
        QString ma = edtMa->text().trimmed().toUpper();
        QString ten = edtTen->text().trimmed();

        if (ma.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi nhập liệu", "Mã môn học không được để trống!");
            return;
        }
        if (ten.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi nhập liệu", "Tên môn học không được để trống!");
            return;
        }
        if (spnLT->value() + spnTH->value() == 0) {
            QMessageBox::warning(&dlg, "Lỗi nhập liệu", "Tổng số tín chỉ (LT + TH) phải lớn hơn 0!");
            return;
        }

        DS_MonHoc &ds = AppData::instance().dsMH;
        char cMa[11];
        strncpy(cMa, ma.toUtf8().constData(), sizeof(cMa) - 1);
        cMa[sizeof(cMa) - 1] = '\0';
        ChuanHoaMa(cMa);

        if (TimMonHoc(ds.root, cMa) != nullptr) {
            QMessageBox::warning(&dlg, "Trùng khóa", QString("Mã môn học '%1' đã tồn tại trên hệ thống!").arg(cMa));
            return;
        }

        MonHoc mh;
        strcpy(mh.MAMH, cMa);
        strncpy(mh.TENMH, ten.toUtf8().constData(), sizeof(mh.TENMH) - 1);
        mh.TENMH[sizeof(mh.TENMH) - 1] = '\0';
        ChuanHoaTen(mh.TENMH);
        mh.STCLT = spnLT->value();
        mh.STCTH = spnTH->value();

        ThemMonHoc(ds.root, mh, ds.n);
        AppData::instance().save();
        dlg.accept();
    });

    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        loadData();
        QMessageBox::information(this, "Thành công", "Đã thêm môn học mới vào cây BST thành công!");
    }
}

void TabMonHoc::onSuaMonHoc() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn một môn học trên bảng để hiệu chỉnh!");
        return;
    }

    QString maHienTai = table->item(row, 1)->text();
    char cMa[11];
    strncpy(cMa, maHienTai.toUtf8().constData(), sizeof(cMa) - 1);
    cMa[sizeof(cMa) - 1] = '\0';

    DS_MonHoc &ds = AppData::instance().dsMH;
    TreeMonHoc node = TimMonHoc(ds.root, cMa);
    if (!node) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy môn học trên cây BST!");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(QString("Hiệu Chỉnh Môn Học - %1").arg(cMa));
    dlg.resize(420, 240);

    QFormLayout *form = new QFormLayout(&dlg);
    form->setSpacing(10);

    QLineEdit *edtMa = new QLineEdit(cMa, &dlg);
    edtMa->setReadOnly(true);
    edtMa->setStyleSheet("background-color: #f1f5f9; color: #475569;");

    QLineEdit *edtTen = new QLineEdit(QString::fromUtf8(node->mh.TENMH), &dlg);
    edtTen->setMaxLength(50);

    QSpinBox *spnLT = new QSpinBox(&dlg);
    spnLT->setRange(0, 10);
    spnLT->setValue(node->mh.STCLT);

    QSpinBox *spnTH = new QSpinBox(&dlg);
    spnTH->setRange(0, 10);
    spnTH->setValue(node->mh.STCTH);

    form->addRow("Mã môn học:", edtMa);
    form->addRow("Tên môn học mới:", edtTen);
    form->addRow("Số TC lý thuyết:", spnLT);
    form->addRow("Số TC thực hành:", spnTH);

    QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
    form->addRow(btnBox);

    connect(btnBox, &QDialogButtonBox::accepted, [&]() {
        QString ten = edtTen->text().trimmed();
        if (ten.isEmpty()) {
            QMessageBox::warning(&dlg, "Lỗi nhập liệu", "Tên môn học không được để trống!");
            return;
        }
        if (spnLT->value() + spnTH->value() == 0) {
            QMessageBox::warning(&dlg, "Lỗi nhập liệu", "Tổng số tín chỉ (LT + TH) phải lớn hơn 0!");
            return;
        }

        strncpy(node->mh.TENMH, ten.toUtf8().constData(), sizeof(node->mh.TENMH) - 1);
        node->mh.TENMH[sizeof(node->mh.TENMH) - 1] = '\0';
        ChuanHoaTen(node->mh.TENMH);
        node->mh.STCLT = spnLT->value();
        node->mh.STCTH = spnTH->value();

        AppData::instance().save();
        dlg.accept();
    });

    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    if (dlg.exec() == QDialog::Accepted) {
        loadData();
        QMessageBox::information(this, "Thành công", "Đã cập nhật thông tin môn học thành công!");
    }
}

void TabMonHoc::onXoaMonHoc() {
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Thông báo", "Vui lòng chọn môn học cần xóa trên bảng!");
        return;
    }

    QString maHienTai = table->item(row, 1)->text();
    QString tenHienTai = table->item(row, 2)->text();
    char cMa[11];
    strncpy(cMa, maHienTai.toUtf8().constData(), sizeof(cMa) - 1);
    cMa[sizeof(cMa) - 1] = '\0';

    DS_LopTC &dsLTC = AppData::instance().dsLTC;
    int soLTC = DemLopTCByMAMH(dsLTC, cMa);
    if (soLTC > 0) {
        QMessageBox::critical(this, "Ràng buộc toàn vẹn",
            QString("Không thể xóa môn học '%1'!\nMôn học này đang được mở bởi %2 lớp tín chỉ.\n"
                    "Vui lòng hủy hoặc xóa các lớp tín chỉ liên quan trước.").arg(maHienTai).arg(soLTC));
        return;
    }

    auto ret = QMessageBox::question(this, "Xác nhận xóa",
        QString("Bạn có chắc chắn muốn xóa môn học sau khỏi cây BST không?\n\nMã MH: %1\nTên MH: %2")
            .arg(maHienTai).arg(tenHienTai),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        DS_MonHoc &dsMH = AppData::instance().dsMH;
        if (XoaMonHocNoiB(dsMH.root, cMa, dsMH.n)) {
            AppData::instance().save();
            loadData();
            QMessageBox::information(this, "Thành công", QString("Đã xóa môn học '%1' thành công!").arg(maHienTai));
        } else {
            QMessageBox::warning(this, "Lỗi", "Không tìm thấy môn học trên cây BST để xóa!");
        }
    }
}

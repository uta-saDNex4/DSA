#include <QApplication>
#include <QStyleFactory>
#include <QFont>
#include <filesystem>
#include "MainWindow.h"
#include "AppData.h"

int main(int argc, char *argv[]) {
    // Tự động chuyển working directory về thư mục gốc của đồ án nếu chạy từ build/
    if (!std::filesystem::exists("data") && std::filesystem::exists("../data")) {
        std::error_code ec;
        std::filesystem::current_path("..", ec);
    }

    // Enable High DPI scaling
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

    QApplication app(argc, argv);
    app.setApplicationName("PTIT DSA - Quan Ly Diem Tin Chi");
    app.setApplicationVersion("2.0.0");
    app.setOrganizationName("PTIT");

    // Use Fusion style as modern clean base
    app.setStyle(QStyleFactory::create("Fusion"));

    // Ep Palette sang dong bo de khong bi xung dot khi Windows bat Dark Mode
    QPalette lightPalette;
    lightPalette.setColor(QPalette::Window, QColor("#f8fafc"));
    lightPalette.setColor(QPalette::WindowText, QColor("#1e293b"));
    lightPalette.setColor(QPalette::Base, QColor("#ffffff"));
    lightPalette.setColor(QPalette::AlternateBase, QColor("#f8fafc"));
    lightPalette.setColor(QPalette::ToolTipBase, QColor("#ffffff"));
    lightPalette.setColor(QPalette::ToolTipText, QColor("#1e293b"));
    lightPalette.setColor(QPalette::Text, QColor("#1e293b"));
    lightPalette.setColor(QPalette::Button, QColor("#ffffff"));
    lightPalette.setColor(QPalette::ButtonText, QColor("#1e293b"));
    lightPalette.setColor(QPalette::BrightText, Qt::red);
    lightPalette.setColor(QPalette::Link, QColor("#be3142"));
    lightPalette.setColor(QPalette::Highlight, QColor("#fee2e2"));
    lightPalette.setColor(QPalette::HighlightedText, QColor("#740d18"));
    app.setPalette(lightPalette);

    // Modern clean font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    // Initialize backend data structures & load from data/*.txt
    AppData::instance().init();

    MainWindow window;
    window.show();

    return app.exec();
}

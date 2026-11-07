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

    // Modern clean font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    // Initialize backend data structures & load from data/*.txt
    AppData::instance().init();

    MainWindow window;
    window.show();

    return app.exec();
}

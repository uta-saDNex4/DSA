#ifndef TOAST_H
#define TOAST_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

class Toast : public QWidget {
    Q_OBJECT
public:
    static void showSuccess(QWidget *parent, const QString &message, int durationMs = 2200) {
        show(parent, "✅ " + message, "#0f766e", durationMs);
    }

    static void showInfo(QWidget *parent, const QString &message, int durationMs = 2200) {
        show(parent, "ℹ️ " + message, "#1e293b", durationMs);
    }

    static void show(QWidget *parentWidget, const QString &text, const QString &bgColor = "#1e293b", int durationMs = 2200) {
        if (!parentWidget) return;
        QWidget *topWindow = parentWidget->window();
        if (!topWindow) topWindow = parentWidget;

        QLabel *toast = new QLabel(topWindow);
        toast->setText(text);
        toast->setStyleSheet(QString(
            "background-color: %1; "
            "color: #ffffff; "
            "font-size: 13px; "
            "font-weight: 600; "
            "border-radius: 8px; "
            "padding: 10px 22px; "
            "border: 1px solid rgba(255, 255, 255, 0.25); "
        ).arg(bgColor));
        toast->setAlignment(Qt::AlignCenter);
        toast->adjustSize();

        // Căn giữa ngang và nằm cách đáy màn hình 55px (ngay trên StatusBar)
        int x = (topWindow->width() - toast->width()) / 2;
        int y = topWindow->height() - toast->height() - 55;
        if (y < 20) y = 20;
        toast->move(x, y);
        toast->show();
        toast->raise();

        // Hiệu ứng mờ dần (Fade out) trước khi tự hủy
        QGraphicsOpacityEffect *eff = new QGraphicsOpacityEffect(toast);
        toast->setGraphicsEffect(eff);

        int fadeTime = 400;
        int stayTime = (durationMs > fadeTime) ? (durationMs - fadeTime) : 1000;

        QTimer::singleShot(stayTime, toast, [toast, eff, fadeTime]() {
            QPropertyAnimation *anim = new QPropertyAnimation(eff, "opacity", toast);
            anim->setDuration(fadeTime);
            anim->setStartValue(1.0);
            anim->setEndValue(0.0);
            QObject::connect(anim, &QPropertyAnimation::finished, toast, &QWidget::deleteLater);
            anim->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }
};

#endif // TOAST_H

#include "ColorPicker.h"

#include <QApplication>
#include <QColor>
#include <QMainWindow>
#include <QtLogging>

class TestMainWindow : public QMainWindow
{
    QColor color {Qt::red};

public:
    TestMainWindow()
    {
        auto *colorPicker {new ColorPicker {color, this}};
        connect(colorPicker, &ColorPicker::colorChanged, this, [](const QColor &color) {
            qDebug() << "Current color: " << color;
        });
    }
};

int main(int argc, char *argv[])
{
    QApplication app {argc, argv};
    TestMainWindow win;
    win.show();
    return QApplication::exec();
}

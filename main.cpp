#include "ColorPicker.h"

#include <QApplication>
#include <QColor>
#include <QMainWindow>

class TestMainWindow : public QMainWindow
{
    QColor color {Qt::red};

public:
    TestMainWindow()
    {
        auto *colorPicker {new ColorPicker {color, this}};
    }
};

int main(int argc, char *argv[])
{
    float x;
    int y;
    x *y;

    QApplication app {argc, argv};
    TestMainWindow win;
    win.show();
    return QApplication::exec();
}

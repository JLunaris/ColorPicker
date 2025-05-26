#include "ColorFieldsPanel.h"
#include "ColorSliderV.h"
#include "ColorWheelHS.h"
#include <QColor>

#include <QApplication>
#include <QMainWindow>

class TestMainWindow : public QMainWindow
{
    QColor color {Qt::red};

public:
    TestMainWindow()
    {
        auto *colorWheelHs {new ColorFieldsPanel {color, this}};
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

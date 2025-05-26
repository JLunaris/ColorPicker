#include "ColorPicker.h"

#include <QPainter>

void ColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor::fromRgb(83, 83, 83));
    painter.drawRect(rect());
}

ColorPicker::ColorPicker(const QColor &color, QWidget *parent)
        : QWidget(parent), m_color {color}
{
    // setupUI
    {
        setFixedSize(220 + 16 + 15 + 10, 220 + 145);
        m_colorSlider->move(m_colorWheel->width(), 6);
        m_colorFieldsPanel->move(0, m_colorWheel->height());
    }
}

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

    // 设置ColorChanged信号的发射
    {
        connect(m_colorWheel, &ColorWheelHS::ColorChanged, this, &ColorPicker::colorChanged);
        connect(m_colorSlider, &ColorSliderV::ColorChanged, this, &ColorPicker::colorChanged);
        connect(m_colorFieldsPanel, &ColorFieldsPanel::ColorChanged, this, &ColorPicker::colorChanged);
    }

    // 色彩盘、色彩滑杆、颜色字段面板 互联互通
    {
        connect(m_colorWheel, &ColorWheelHS::ColorChanged, m_colorSlider, &ColorSliderV::syncWithColor);
        connect(m_colorWheel, &ColorWheelHS::ColorChanged, m_colorFieldsPanel, &ColorFieldsPanel::syncWithColor);

        connect(m_colorSlider, &ColorSliderV::ColorChanged, m_colorWheel, &ColorWheelHS::syncWithColor);
        connect(m_colorSlider, &ColorSliderV::ColorChanged, m_colorFieldsPanel, &ColorFieldsPanel::syncWithColor);

        connect(m_colorFieldsPanel, &ColorFieldsPanel::ColorChanged, m_colorWheel, &ColorWheelHS::syncWithColor);
        connect(m_colorFieldsPanel, &ColorFieldsPanel::ColorChanged, m_colorSlider, &ColorSliderV::syncWithColor);
    }
}

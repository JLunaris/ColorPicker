#pragma once

#include "ColorFieldsPanel.h"
#include "ColorSliderV.h"
#include "ColorWheelHS.h"

#include <QColor>
#include <QPaintEvent>
#include <QWidget>

class ColorPicker : public QWidget
{
    Q_OBJECT

    QColor m_color;

    ColorWheelHS *m_colorWheel {new ColorWheelHS {m_color, this}};
    ColorSliderV *m_colorSlider {new ColorSliderV {m_color, this}};
    ColorFieldsPanel *m_colorFieldsPanel {new ColorFieldsPanel {m_color, this}};

Q_SIGNALS:
    void colorChanged(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit ColorPicker(const QColor &color, QWidget *parent = nullptr);
};

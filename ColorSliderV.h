#pragma once

#include <QColor>
#include <QWidget>

class ColorSliderV : public QWidget
{
    Q_OBJECT

    const int m_hMargin {8}; // 横向外边缘
    const int m_vMargin {4}; // 纵向外边缘
    const int m_sliderWidth;
    const int m_sliderHeight;

    QColor &m_color;
    QColor m_handleColor {Qt::black}; // 手柄颜色

Q_SIGNALS:
    void ColorChanged();

public Q_SLOTS:
    void syncWithColor();

private:
    void updateColorFromMousePos(const QPointF &pos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

public:
    explicit ColorSliderV(QColor &color, QWidget *parent = nullptr, int width = 15, int height = 200);

    void setHandleColor(const QColor &color) { m_color = color; }

    [[nodiscard]] QColor handleColor() const { return m_handleColor; }
};

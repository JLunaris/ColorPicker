#pragma once

#include <QColor>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPointF>
#include <QWidget>

class ColorWheelHS : public QWidget
{
    Q_OBJECT

    const int m_marginWidth {10}; // 外边缘宽度
    const int m_radius; // 半径
    const QPointF m_center {qreal(m_marginWidth + m_radius),
                            qreal(m_marginWidth + m_radius)}; // 中心点

    QColor &m_color;
    QPointF m_target; // 目标的位置
    bool m_isMouseGrabbed {false}; // 是否抓住鼠标

Q_SIGNALS:
    void ColorChanged(const QColor &color);

public Q_SLOTS:
    void syncWithColor();
    // TODO

private:
    [[nodiscard]] bool isInWheel(const QPointF &pos) const;
    void updateColorFromTarget();
    // TODO

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

public:
    explicit ColorWheelHS(QColor &color, QWidget *parent = nullptr, int radius = 100);
    // TODO
};

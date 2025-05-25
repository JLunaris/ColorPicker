#include "ColorSliderV.h"

#include <QColor>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainter>
#include <QPointF>
#include <QPolygonF>
#include <QtLogging>
#include <utility>

void ColorSliderV::syncWithColor()
{
    update();
}

void ColorSliderV::updateColorFromMousePos(const QPointF &pos)
{
    qreal y {std::min(std::max(pos.y(), double(m_vMargin)), double(m_vMargin + m_sliderHeight))};
    // 对鼠标事件的y坐标进行限制，限制它最小值为m_vMargin，最大值为m_vMargin + m_sliderHeight

    m_color.setHsvF(m_color.hueF(), m_color.saturationF(), float(1 - (y - m_vMargin) / m_sliderHeight));

    Q_EMIT ColorChanged();
}

void ColorSliderV::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    painter.setRenderHint(QPainter::Antialiasing);

    // 线性渐变
    {
        QLinearGradient linearGradient {QPointF(m_hMargin + m_sliderWidth, m_vMargin + m_sliderHeight), QPointF(m_hMargin, m_vMargin)};
        linearGradient.setColorAt(0, QColor::fromHsvF(m_color.hueF(), m_color.saturationF(), 0));
        linearGradient.setColorAt(1, QColor::fromHsvF(m_color.hueF(), m_color.saturationF(), 1));

        painter.setPen(Qt::NoPen);
        painter.setBrush(linearGradient);
        painter.drawRect(QRect {m_hMargin, m_vMargin, m_sliderWidth, m_sliderHeight});
    }

    // 滑动手柄
    {
        painter.setPen(Qt::NoPen);
        painter.setBrush(m_handleColor);

        qreal targetY {float(m_vMargin) + (1 - m_color.valueF()) * float(m_sliderHeight)};

        painter.drawConvexPolygon(QPolygonF {QList<QPointF> {
                QPointF(0, targetY - m_vMargin),
                QPointF(0, targetY + m_vMargin),
                QPointF(m_hMargin, targetY)}});

        painter.drawConvexPolygon(QPolygonF {QList<QPointF> {
                QPointF(width(), targetY - m_vMargin),
                QPointF(width(), targetY + m_vMargin),
                QPointF(width() - m_hMargin, targetY)}});
    }
}

void ColorSliderV::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        updateColorFromMousePos(event->position());
        update();
    }
}

void ColorSliderV::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        updateColorFromMousePos(event->position());
        update();
    }
}

ColorSliderV::ColorSliderV(QColor &color, QWidget *parent, int width, int height)
        : QWidget(parent), m_color {color}, m_sliderWidth {width}, m_sliderHeight {height}
{
    setFixedSize(width + 2 * m_hMargin, height + 2 * m_vMargin);
    syncWithColor();
}

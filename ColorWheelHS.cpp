#include "ColorWheelHS.h"

#include <QConicalGradient>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QRadialGradient>
#include <QVector2D>
#include <QWidget>
#include <cassert>
#include <cmath>
#include <numbers>

void ColorWheelHS::syncWithColor()
{
    double theta {m_color.hueF() * 2 * std::numbers::pi}; // 矢量的夹角(单位: 弧度)
    double r {m_color.saturationF() * float(m_radius)}; // 矢量的长度

    QVector2D colorVector {QPointF {r * std::cos(theta), -r * std::sin(theta)}}; // 求出颜色矢量
    m_target = (QVector2D {m_center} + colorVector).toPointF(); // 颜色矢量与原点矢量相加，求出新m_target位置

    update();
}

bool ColorWheelHS::isInWheel(const QPointF &pos) const
{
    auto vector {QVector2D {pos} - QVector2D {m_center}};
    return std::round(vector.length()) <= float(m_radius); // 使用std::round减小浮点数运算误差
}

void ColorWheelHS::updateColorFromTarget()
{
    assert(isInWheel(m_target));

    // 计算新H
    float h;
    {
        QPointF vectorPoint = (QVector2D {m_target} - QVector2D {m_center}).toPointF();
        double rad {std::atan2(-vectorPoint.y(), vectorPoint.x())};
        double twoPi {2 * std::numbers::pi};
        if (rad < 0) rad += twoPi;
        h = float(rad / twoPi);
    }

    // 计算新S
    float s;
    {
        auto vector {QVector2D {m_target} - QVector2D {m_center}};
        auto result {vector.length() / float(m_radius)};
        s = std::min(result, 1.0f); // 浮点数运算原因，有时可能会略大于1，因此用std::min限制它
    }

    m_color.setHsvF(h, s, m_color.valueF());
    Q_EMIT ColorChanged(m_color);
}

void ColorWheelHS::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    painter.setRenderHint(QPainter::RenderHint::Antialiasing);

    // 锥形渐变
    {
        QConicalGradient conicalGradient;

        for (int i {0}; i <= 6; ++i) {
            float h {float(i) / 6};
            float s {1};
            conicalGradient.setColorAt(i / 6.0, QColor::fromHsvF(h, s, m_color.valueF()));
        }

        conicalGradient.setAngle(0);
        conicalGradient.setCenter(m_center);

        painter.setPen(Qt::NoPen);
        painter.setBrush(conicalGradient);
        painter.drawEllipse(m_center, m_radius, m_radius);
    }

    // 叠加径向渐变
    {
        QRadialGradient radialGradient {m_center, qreal(m_radius)};

        radialGradient.setColorAt(0, QColor::fromHsvF(0, 0, m_color.valueF()));
        radialGradient.setColorAt(1, Qt::transparent);

        painter.setPen(Qt::NoPen);
        painter.setBrush(radialGradient);
        painter.drawEllipse(m_center, m_radius, m_radius);
    }

    // target小圆圈
    {
        painter.setPen(QPen {QBrush {0x444444}, 1});
        painter.setBrush(m_color);

        if (m_isMouseGrabbed) {
            painter.drawEllipse(m_target, 8, 8);
        } else {
            painter.drawEllipse(m_target, 5, 5);
        }
    }
}

void ColorWheelHS::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton &&
        isInWheel(event->position()))
    {
        m_isMouseGrabbed = true;
        m_target = event->position();
        updateColorFromTarget();
        update();
    }
}

void ColorWheelHS::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isMouseGrabbed &&
        event->buttons() & Qt::LeftButton) {

        auto vector {QVector2D {event->position()} - QVector2D {m_center}}; // 圆心 -> 鼠标位置
        float length {vector.length()};

        if (length > float(m_radius)) {
            vector *= (float(m_radius) / length);
            m_target = (QVector2D {m_center} + vector).toPointF();
        } else {
            m_target = event->position();
        }

        updateColorFromTarget();
        update();
    }
}

void ColorWheelHS::mouseReleaseEvent(QMouseEvent *event)
{
    m_isMouseGrabbed = false;
    update();
}

ColorWheelHS::ColorWheelHS(QColor &color, QWidget *parent, int radius)
        : QWidget(parent), m_color {color}, m_radius {radius}
{
    setFixedSize((m_marginWidth + m_radius) * 2, (m_marginWidth + m_radius) * 2);
    syncWithColor();
}

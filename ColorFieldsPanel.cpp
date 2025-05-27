#include "ColorFieldsPanel.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QSignalBlocker>
#include <QString>
#include <QVBoxLayout>
#include <format>

QString ColorNameSpinBox::textFromValue(int value) const
{
    return QString::fromStdString(std::format("{:06x}", value));
}

ColorNameSpinBox::ColorNameSpinBox(QWidget *parent)
        : QSpinBox(parent)
{
    setDisplayIntegerBase(16);
    setMaximum(0xffffff);
}

void ColorFrame::paintEvent(QPaintEvent *event)
{
    QPainter painter {this};
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    painter.drawRect(rect());
}

ColorFrame::ColorFrame(const QColor &color, QWidget *parent)
        : QWidget(parent), m_color {color}
{
}

void ColorFieldsPanel::syncWithColor()
{
    // 使用 QSignalBlocker 临时屏蔽信号, 避免调用 setValue() 时触发 valueChanged() 信号
    QSignalBlocker blocker1 {m_hField};
    QSignalBlocker blocker2 {m_sField};
    QSignalBlocker blocker3 {m_vField};
    QSignalBlocker blocker4 {m_rField};
    QSignalBlocker blocker5 {m_gField};
    QSignalBlocker blocker6 {m_bField};
    QSignalBlocker blocker7 {m_nameField};

    m_hField->setValue(m_color.hue());
    m_sField->setValue(std::lround(m_color.saturationF() * 100)); // 四舍五入以减小浮点数运算误差
    /* 采用 lround 而不是 round 是因为 round 返回结果为浮点型, 强转为 int 型可能会出问题
     * 比如 round 返回的 45.0f 可能实际存储为 44.999999f, 如果直接强转为 int 型, 可能会变为 44, 导致错误结果
     */
    m_vField->setValue(std::lround(m_color.valueF() * 100)); // 四舍五入以减小浮点数运算误差
    m_rField->setValue(m_color.red());
    m_gField->setValue(m_color.green());
    m_bField->setValue(m_color.blue());
    m_nameField->setValue(m_color.name(QColor::HexRgb).removeFirst().toInt(nullptr, 16));

    m_colorFrame->update();
}

void ColorFieldsPanel::setupUi()
{
    // 字段控件
    {
        m_hField->setFixedWidth(40);
        m_sField->setFixedWidth(40);
        m_vField->setFixedWidth(40);
        m_rField->setFixedWidth(40);
        m_gField->setFixedWidth(40);
        m_bField->setFixedWidth(40);
        m_nameField->setFixedWidth(75);

        m_hField->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_sField->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_vField->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_rField->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_gField->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_bField->setButtonSymbols(QAbstractSpinBox::NoButtons);
        m_nameField->setButtonSymbols(QAbstractSpinBox::NoButtons);
    }

    // 垂直布局
    {
        QVBoxLayout *vBoxLayout {new QVBoxLayout {this}};

        // 水平布局(两个表单布局构成)
        {
            QHBoxLayout *hBoxLayout {new QHBoxLayout};
            vBoxLayout->addLayout(hBoxLayout);

            hBoxLayout->addSpacing(7);

            // 左侧表单布局
            {
                QFormLayout *formLayout {new QFormLayout};
                hBoxLayout->addLayout(formLayout);

                formLayout->setHorizontalSpacing(5);

                QHBoxLayout *hLayout {new QHBoxLayout};
                hLayout->addWidget(m_hField);
                hLayout->addWidget(new QLabel {"度"});
                hLayout->setSpacing(3);
                formLayout->addRow("色相(H):", hLayout);

                QHBoxLayout *sLayout {new QHBoxLayout};
                sLayout->addWidget(m_sField);
                sLayout->addWidget(new QLabel {"%"});
                sLayout->setSpacing(3);
                formLayout->addRow("饱和度(S):", sLayout);

                QHBoxLayout *vLayout {new QHBoxLayout};
                vLayout->addWidget(m_vField);
                vLayout->addWidget(new QLabel {"%"});
                vLayout->setSpacing(3);
                formLayout->addRow("明度(B):", vLayout);
            }

            hBoxLayout->addStretch();

            // 右侧表单布局
            {
                QFormLayout *formLayout {new QFormLayout};
                hBoxLayout->addLayout(formLayout);

                formLayout->setHorizontalSpacing(5);

                formLayout->addRow("红色(R):", m_rField);
                formLayout->addRow("绿色(G):", m_gField);
                formLayout->addRow("蓝色(B):", m_bField);
            }

            hBoxLayout->addSpacing(7);
        }

        vBoxLayout->addStretch();

        // 水平布局(颜色显示控件 + colorName表单)
        {
            QHBoxLayout *hBoxLayout {new QHBoxLayout};
            vBoxLayout->addLayout(hBoxLayout);

            hBoxLayout->addSpacing(20);
            hBoxLayout->addWidget(m_colorFrame);
            m_colorFrame->setFixedWidth(80);
            hBoxLayout->addSpacing(20);

            // colorName表单
            {
                QFormLayout *formLayout {new QFormLayout};
                hBoxLayout->addLayout(formLayout);

                formLayout->addRow("#", m_nameField);
            }
        }

        vBoxLayout->addStretch();
    }

    setStyleSheet(R"(
        QWidget {
            color: #f0f0f0;
        }

        QSpinBox {
            border: 1px solid #666666;
            border-radius: 2px;
            padding-top: 2px;
            padding-bottom: 4px;
            background: #454545;
            color: #f0f0f0;
            font: "Segoe UI";
        }

        QSpinBox:focus {
            border: 1px solid #0078d7; /* 蓝色 */
        }
    )");
}

void ColorFieldsPanel::syncWithColorForNameFieldChanged()
{
    // 使用 QSignalBlocker 临时屏蔽信号, 避免调用 setValue() 时触发 valueChanged() 信号
    QSignalBlocker blocker1 {m_hField};
    QSignalBlocker blocker2 {m_sField};
    QSignalBlocker blocker3 {m_vField};
    QSignalBlocker blocker4 {m_rField};
    QSignalBlocker blocker5 {m_gField};
    QSignalBlocker blocker6 {m_bField};

    m_hField->setValue(m_color.hue());
    m_sField->setValue(std::lround(m_color.saturationF() * 100)); // 四舍五入以减小浮点数运算误差
    /* 采用 lround 而不是 round 是因为 round 返回结果为浮点型, 强转为 int 型可能会出问题
     * 比如 round 返回的 45.0f 可能实际存储为 44.999999f, 如果直接强转为 int 型, 可能会变为 44, 导致错误结果
     */
    m_vField->setValue(std::lround(m_color.valueF() * 100)); // 四舍五入以减小浮点数运算误差
    m_rField->setValue(m_color.red());
    m_gField->setValue(m_color.green());
    m_bField->setValue(m_color.blue());

    m_colorFrame->update();
}

ColorFieldsPanel::ColorFieldsPanel(QColor &color, QWidget *parent)
        : QWidget(parent), m_color {color}
{
    setFixedSize(261, 145);

    setupUi();

    // 字段控件设置
    {
        m_hField->setMaximum(359);
        m_sField->setMaximum(100);
        m_vField->setMaximum(100);
        m_rField->setMaximum(255);
        m_gField->setMaximum(255);
        m_bField->setMaximum(255);
    }

    // 连接信号和槽：当用户更改颜色字段时要做的事情
    {
        connect(m_hField, &QSpinBox::valueChanged, this, [this](int h) {
            m_color.setHsv(h, m_color.saturation(), m_color.value());
            syncWithColor();
            Q_EMIT ColorChanged(m_color);
        });
        connect(m_sField, &QSpinBox::valueChanged, this, [this](int s) {
            m_color.setHsvF(m_color.hueF(), float(s) / 100, m_color.valueF());
            syncWithColor();
            Q_EMIT ColorChanged(m_color);
        });
        connect(m_vField, &QSpinBox::valueChanged, this, [this](int v) {
            m_color.setHsvF(m_color.hueF(), m_color.saturationF(), float(v) / 100);
            syncWithColor();
            Q_EMIT ColorChanged(m_color);
        });
        connect(m_rField, &QSpinBox::valueChanged, this, [this](int r) {
            m_color.setRgb(r, m_color.green(), m_color.blue());
            syncWithColor();
            Q_EMIT ColorChanged(m_color);
        });
        connect(m_gField, &QSpinBox::valueChanged, this, [this](int g) {
            m_color.setRgb(m_color.red(), g, m_color.blue());
            syncWithColor();
            Q_EMIT ColorChanged(m_color);
        });
        connect(m_bField, &QSpinBox::valueChanged, this, [this](int b) {
            m_color.setRgb(m_color.red(), m_color.green(), b);
            syncWithColor();
            Q_EMIT ColorChanged(m_color);
        });
        connect(m_nameField, &QSpinBox::valueChanged, this, [this](int hex) {
            m_color = QColor::fromString(std::format("#{:06x}", hex));
            syncWithColorForNameFieldChanged();
            Q_EMIT ColorChanged(m_color);
        });
    }

    syncWithColor();
}

#pragma once

#include <QColor>
#include <QSpinBox>
#include <QWidget>

class ColorNameSpinBox : public QSpinBox
{
    Q_OBJECT

protected:
    QString textFromValue(int value) const override;

public:
    explicit ColorNameSpinBox(QWidget *parent = nullptr);
};

class ColorFrame : public QWidget
{
    Q_OBJECT

    const QColor &m_color;

protected:
    void paintEvent(QPaintEvent *event) override;

public:
    explicit ColorFrame(const QColor &color, QWidget *parent = nullptr);
};

class ColorFieldsPanel : public QWidget
{
    Q_OBJECT

    QColor &m_color;

    QSpinBox *m_hField {new QSpinBox {this}};
    QSpinBox *m_sField {new QSpinBox {this}};
    QSpinBox *m_vField {new QSpinBox {this}};
    QSpinBox *m_rField {new QSpinBox {this}};
    QSpinBox *m_gField {new QSpinBox {this}};
    QSpinBox *m_bField {new QSpinBox {this}};
    ColorFrame *m_colorFrame {new ColorFrame {m_color, this}};
    ColorNameSpinBox *m_nameField {new ColorNameSpinBox {this}};

Q_SIGNALS:
    void ColorChanged();

public Q_SLOTS:
    void syncWithColor();

private:
    void setupUi();
    void syncWithColorForNameFieldChanged();

public:
    explicit ColorFieldsPanel(QColor &color, QWidget *parent = nullptr);
};

#include <QPainter>
#include "vumeter.h"

VUMeter::VUMeter(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_MinValue = 0;
    m_MaxValue = 100;
    m_Value = 50;

    connect(this, SIGNAL(enabledChanged()), this, SLOT(doEnabledChanged()));
}

VUMeter::~VUMeter()
{

}

void VUMeter::doEnabledChanged()
{
    update();
}

void VUMeter::setValue(double v)
{
    if (v < m_MinValue)
        v = m_MinValue;
    else if (v > m_MaxValue)
        v = m_MaxValue;

    m_Value = v;

    emit valueChanged();

    update();
}

void VUMeter::setMinimumValue(double v)
{
    m_MinValue = v;

    emit minimumValueChanged();

    update();
}

void VUMeter::setMaximumValue(double v)
{
    m_MaxValue = v;

    emit maximumValueChanged();

    update();
}

void VUMeter::paint(QPainter *painter)
{
    qreal _w = width();
    qreal _h = height();

    qreal _wblock = 7*(_w / (31*7 + 30*2));
    qreal _wspc = 2*(_w / (31*7 + 30*2));
    qreal _vstep = (m_MaxValue - m_MinValue) / 31.0;
    qreal _v = m_MinValue;
    qreal _x = 0.0;

    painter->setRenderHints(QPainter::Antialiasing, true);

    if (!this->isEnabled())
        painter->setOpacity(0.4);

    QColor green(59,216,32);
    QColor gray(68,68,68);
    QColor orange(235,195,46);
    QColor red(255,91,56);

    for (int i=0; i < 21; i++)
    {
        QRectF rect(_x, 0, _wblock, _h);

        if (m_Value > _v)
            painter->fillRect(rect, green);
        else
            painter->fillRect(rect, gray);

        _x += _wblock + _wspc;
        _v += _vstep;
    }
    for (int i=0; i < 5; i++)
    {
        QRectF rect(_x, 0, _wblock, _h);

        if (m_Value > _v)
            painter->fillRect(rect, orange);
        else
            painter->fillRect(rect, gray);

        _x += _wblock + _wspc;
        _v += _vstep;
    }
    for (int i=0; i < 5; i++)
    {
        QRectF rect(_x, 0, _wblock, _h);

        if (m_Value > _v)
            painter->fillRect(rect, red);
        else
            painter->fillRect(rect, gray);

        _x += _wblock + _wspc;
        _v += _vstep;
    }
}

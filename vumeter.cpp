#include <QPainter>
#include "vumeter.h"

VUMeter::VUMeter(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    m_MinValue = 0;
    m_MaxValue = 1;
    m_Value = m_MinValue;
    m_CurrentValue = m_MinValue;
    m_CurrentMarker = m_MinValue;
    m_MarkerVisible = true;
    m_TotalSegments = 40;
    m_WarnSegments = 10;
    m_ErrSegments = 5;

    m_DecayTimer = new QTimer(this);

    connect(m_DecayTimer, SIGNAL(timeout()), this, SLOT(handleSignalDecay()));
    connect(this, SIGNAL(enabledChanged()), this, SLOT(doEnabledChanged()));

    m_DecayTimer->start(20);
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
    if (v > m_CurrentValue)
        m_CurrentValue = v;

    if (m_CurrentValue > m_CurrentMarker)
        m_CurrentMarker = m_CurrentValue;

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

void VUMeter::handleSignalDecay()
{
    if (m_CurrentValue >= m_Value + 0.1*(m_MaxValue - m_MinValue) / m_TotalSegments)
        m_CurrentValue = m_CurrentValue + 0.2 * (m_Value - m_CurrentValue);
    else
        m_CurrentValue = m_Value;

    if (m_CurrentMarker >= m_Value + 0.1*(m_MaxValue - m_MinValue) / m_TotalSegments)
        m_CurrentMarker = m_CurrentMarker + 0.02 * (m_Value - m_CurrentMarker);
    else
        m_CurrentMarker = m_Value;

    update();
}

void VUMeter::paint(QPainter *painter)
{
    qreal _w = width();
    qreal _h = height();

    qreal _wblock = 7*(_w / (m_TotalSegments*7 + (m_TotalSegments - 1)*2));
    qreal _wspc = 2*(_w / (m_TotalSegments*7 + (m_TotalSegments - 1)*2));
    qreal _vstep = (m_MaxValue - m_MinValue) / (double)m_TotalSegments;
    qreal _v = m_MinValue;
    qreal _x = 0.0;

    painter->setRenderHints(QPainter::Antialiasing, true);

    if (!this->isEnabled())
        painter->setOpacity(0.4);

    QColor green(59,216,32);
    QColor gray(68,68,68);
    QColor orange(235,195,46);
    QColor red(255,91,56);

    for (int i=0; i < (m_TotalSegments - m_WarnSegments - m_ErrSegments); i++)
    {
        QRectF rect(_x, 0, _wblock, _h);

        if (m_CurrentValue > _v)
            painter->fillRect(rect, green);
        else if (m_MarkerVisible && (m_CurrentMarker > _v && (m_CurrentMarker < (_v + _vstep))))
            painter->fillRect(rect, green);
        else
            painter->fillRect(rect, gray);

        _x += _wblock + _wspc;
        _v += _vstep;
    }
    for (int i=0; i < m_WarnSegments; i++)
    {
        QRectF rect(_x, 0, _wblock, _h);

        if (m_CurrentValue > _v)
            painter->fillRect(rect, orange);
        else if (m_MarkerVisible && (m_CurrentMarker > _v && (m_CurrentMarker < (_v + _vstep))))
            painter->fillRect(rect, orange);
        else
            painter->fillRect(rect, gray);

        _x += _wblock + _wspc;
        _v += _vstep;
    }
    for (int i=0; i < m_ErrSegments; i++)
    {
        QRectF rect(_x, 0, _wblock, _h);

        if (m_CurrentValue > _v)
            painter->fillRect(rect, red);
        else if (m_MarkerVisible && (m_CurrentMarker > _v && (m_CurrentMarker < (_v + _vstep))))
            painter->fillRect(rect, red);
        else
            painter->fillRect(rect, gray);

        _x += _wblock + _wspc;
        _v += _vstep;
    }
}

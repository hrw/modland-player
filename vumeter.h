#ifndef VUMETER_H
#define VUMETER_H

#include <QQuickPaintedItem>
#include <QQuickItem>

class VUMeter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_DISABLE_COPY(VUMeter)

    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qreal minimumValue READ minimumValue WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(qreal maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)

public:
    VUMeter(QQuickItem *parent = nullptr);
    virtual ~VUMeter();
    double maximumValue() { return m_MaxValue; }
    double minimumValue() { return m_MinValue; }
    double value() { return m_Value; }
    void setMaximumValue(double);
    void setMinimumValue(double);
    void setValue(double);

    void paint(QPainter *painter);

signals:
    void valueChanged();
    void minimumValueChanged();
    void maximumValueChanged();

private slots:
    void doEnabledChanged();

private:
    double  m_MinValue;
    double  m_MaxValue;
    double  m_Value;
};

#endif // VUMETER_H

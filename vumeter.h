#ifndef VUMETER_H
#define VUMETER_H

#include <QQuickPaintedItem>
#include <QQuickItem>
#include <QTimer>

class VUMeter : public QQuickPaintedItem
{
    Q_OBJECT
    Q_DISABLE_COPY(VUMeter)

    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(qreal minimumValue READ minimumValue WRITE setMinimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(qreal maximumValue READ maximumValue WRITE setMaximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(int totalSegments READ totalSegments WRITE setTotalSegments)
    Q_PROPERTY(int warningSegments READ warningSegments WRITE setWarningSegments)
    Q_PROPERTY(int errorSegments READ errorSegments WRITE setErrorSegments)
    Q_PROPERTY(bool markerVisible READ markerVisible WRITE setMarkerVisible)

public:
    VUMeter(QQuickItem *parent = nullptr);
    virtual ~VUMeter();
    double maximumValue() { return m_MaxValue; }
    double minimumValue() { return m_MinValue; }
    double value() { return m_Value; }
    void setMaximumValue(double);
    void setMinimumValue(double);
    void setValue(double);

    int totalSegments() { return m_TotalSegments; }
    int warningSegments() { return m_WarnSegments; }
    int errorSegments() { return m_ErrSegments; }
    bool markerVisible() { return m_MarkerVisible; }

    void setTotalSegments(int s) { m_TotalSegments = s; update(); }
    void setWarningSegments(int s) { m_WarnSegments = s; update(); }
    void setErrorSegments(int s) { m_ErrSegments = s; update(); }
    void setMarkerVisible(bool v) { m_MarkerVisible = v; update(); }

    void paint(QPainter *painter);

signals:
    void valueChanged();
    void minimumValueChanged();
    void maximumValueChanged();

private slots:
    void doEnabledChanged();
    void handleSignalDecay();

private:
    int     m_TotalSegments;
    int     m_WarnSegments;
    int     m_ErrSegments;
    double  m_MinValue;
    double  m_MaxValue;
    double  m_Value;
    double  m_CurrentValue;
    double  m_CurrentMarker;
    bool    m_MarkerVisible;

    QTimer *m_DecayTimer;
};

#endif // VUMETER_H

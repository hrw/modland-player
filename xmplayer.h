#ifndef XMPLAYER_H
#define XMPLAYER_H

#include <QObject>
#include <QAudioOutput>
#include <xmp.h>

class XMPlayer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(XMPlayer)

    Q_PROPERTY(bool moduleLoaded READ moduleLoaded NOTIFY moduleLoadedChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString type READ type NOTIFY typeChanged)

    Q_PROPERTY(int pos READ pos NOTIFY posChanged)
    Q_PROPERTY(int pattern READ pattern NOTIFY patternChanged)
    Q_PROPERTY(int row READ row NOTIFY rowChanged)
    Q_PROPERTY(int numRows READ numRows NOTIFY numRowsChanged)
    Q_PROPERTY(int frame READ frame NOTIFY frameChanged)
    Q_PROPERTY(int speed READ speed NOTIFY speedChanged)
    Q_PROPERTY(int bpm READ bpm NOTIFY bpmChanged)

public:
    explicit XMPlayer(QObject *parent = nullptr);
    virtual ~XMPlayer();

    bool load(const QString& fileName);
    bool load(QIODevice *device);
    bool loadFromData(const char *data, int length);
    bool loadFromData(const QByteArray &data);
    bool moduleLoaded() { return m_ModuleLoaded; }

    const QString name() { return m_Name; }
    const QString type() { return m_Type; }
    int pos() { return m_Pos; }
    int pattern() { return m_Pattern; }
    int row() { return m_Row; }
    int numRows() { return m_NumRows; }
    int frame() { return m_Frame; }
    int speed() { return m_Speed; }
    int bpm() { return m_BPM; }

signals:
    void moduleLoadedChanged(bool);
    void nameChanged(QString);
    void typeChanged(QString);
    void posChanged(int);
    void patternChanged(int);
    void rowChanged(int);
    void numRowsChanged(int);
    void frameChanged(int);
    void speedChanged(int);
    void bpmChanged(int);

public slots:

private slots:
    void fetchMoreAudioData();
    void audioStateChanged(QAudio::State newState);

private:
    void releaseModule();
    void getModuleInfo();

    xmp_context xmp_ctx;
    struct xmp_frame_info m_CurrentFrameInfo;
    QAudioOutput *m_AudioOutput;
    QIODevice *m_AudioStream;
    QAudioFormat m_AudioFormat;
    bool m_ModuleLoaded;
    bool m_LastFrameFetched;

    QString m_Name;
    QString m_Type;

    int m_Pos;
    int m_Pattern;
    int m_Row;
    int m_NumRows;
    int m_Frame;
    int m_Speed;
    int m_BPM;
};

#endif // XMPLAYER_H

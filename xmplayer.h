#ifndef XMPLAYER_H
#define XMPLAYER_H

#include <QObject>
#include <QAudioOutput>
#include <QThread>
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
    Q_PROPERTY(int bpm READ bpm NOTIFY bpmChanged)
    Q_PROPERTY(double vuLeft READ vuLeft NOTIFY vuLeftChanged)
    Q_PROPERTY(double vuRight READ vuRight NOTIFY vuRightChanged)
    Q_PROPERTY(int time READ time NOTIFY timeChanged)
    Q_PROPERTY(int totalTime READ totalTime NOTIFY totalTimeChanged)

    Q_PROPERTY(int mix READ mix WRITE setMix NOTIFY mixChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)

    Q_PROPERTY(int len READ len NOTIFY lenChanged)

public:
    explicit XMPlayer(QObject *parent = nullptr);
    virtual ~XMPlayer();

    bool moduleLoaded() { return m_ModuleLoaded; }
    const QString name() { return m_Name; }
    const QString type() { return m_Type; }
    int pos() { return m_Pos; }
    int pattern() { return m_Pattern; }
    int row() { return m_Row; }
    int numRows() { return m_NumRows; }
    int bpm() { return m_BPM; }
    int len() { return m_Len; }
    int mix();
    int volume();
    void setMix(int);
    void setVolume(int);
    int time() { return m_Time; }
    int totalTime() { return m_TotalTime; }
    double vuLeft() { return m_VULeft; }
    double vuRight() { return m_VURight; }

signals:
    void moduleLoadedChanged(bool);
    void nameChanged(QString);
    void typeChanged(QString);
    void posChanged(int);
    void patternChanged(int);
    void rowChanged(int);
    void numRowsChanged(int);
    void bpmChanged(int);
    void lenChanged(int);
    void vuLeftChanged(double);
    void vuRightChanged(double);
    void timeChanged(int);
    void totalTimeChanged(int);
    void mixChanged(int);
    void volumeChanged(int);

    void playStarted();
    void playStopped();
    void playFinished();
    void playPaused();
    void playResumed();

public slots:
    bool load(const QString& fileName);
    bool load(QIODevice *device);
    bool loadFromData(const char *data, int length);
    bool loadFromData(const QByteArray &data);
    void playStart();
    void playStop();
    void playPause();
    void playResume();

private slots:
    void fetchMoreAudioData();
    void audioStateChanged(QAudio::State newState);

private:
    void releaseModule();
    void getModuleInfo();

    QMutex m_AudioLock;

    xmp_context xmp_ctx;
    struct xmp_frame_info m_CurrentFrameInfo;
    QAudioOutput *m_AudioOutput;
    QIODevice *m_AudioStream;
    QAudioFormat m_AudioFormat;
    bool m_ModuleLoaded;
    bool m_LastFrameFetched;
    bool m_IgnoreIdleState;

    QString m_Name;
    QString m_Type;

    double m_VULeft;
    double m_VURight;

    int m_Time;
    int m_TotalTime;
    int m_Pos;
    int m_Pattern;
    int m_Row;
    int m_NumRows;
    int m_BPM;
    int m_Len;
    int m_Mix;
    int m_Volume;
};

#endif // XMPLAYER_H

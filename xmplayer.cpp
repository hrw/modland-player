#include <QDebug>
#include <QThread>
#include "xmplayer.h"

XMPlayer::XMPlayer(QObject *parent) : QObject(parent), m_ModuleLoaded(false), m_LastFrameFetched(false)
{
    xmp_ctx = xmp_create_context();

    // Set up the audio format
    m_AudioFormat.setSampleRate(48000);
    m_AudioFormat.setChannelCount(2);
    m_AudioFormat.setSampleSize(16);
    m_AudioFormat.setCodec("audio/pcm");
    m_AudioFormat.setByteOrder(QAudioFormat::LittleEndian);
    m_AudioFormat.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(m_AudioFormat))
    {
       qWarning() << "Raw audio format not supported by backend, switching to nearest compatible.";
       m_AudioFormat = info.nearestFormat(m_AudioFormat);
    }

    // Create audio output and tell it to send notify every 1ms.
    // It will be used to push new data from XMP to audio
    m_AudioOutput = new QAudioOutput(m_AudioFormat, this);
    m_AudioOutput->setNotifyInterval(1);
    connect(m_AudioOutput, SIGNAL(notify()), this, SLOT(fetchMoreAudioData()));
    connect(m_AudioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioStateChanged(QAudio::State)));

    bzero(&m_CurrentFrameInfo, sizeof(m_CurrentFrameInfo));
}

XMPlayer::~XMPlayer()
{
    delete m_AudioOutput;

    xmp_free_context(xmp_ctx);
}

void XMPlayer::releaseModule()
{
    if (m_ModuleLoaded)
    {
        xmp_release_module(xmp_ctx);
        m_ModuleLoaded = false;

        emit moduleLoadedChanged(m_ModuleLoaded);
    }
}

void XMPlayer::getModuleInfo()
{
    if (m_ModuleLoaded)
    {
        struct xmp_module_info mi;

        xmp_get_module_info(xmp_ctx, &mi);

        QString name = mi.mod->name;
        QString type = mi.mod->type;

        if (name != m_Name)
        {
            m_Name = name;
            emit nameChanged(m_Name);
        }

        if (type != m_Type)
        {
            m_Type = type;
            emit typeChanged(m_Type);
        }

        if (m_Len != mi.mod->len)
        {
            m_Len = mi.mod->len;
            emit lenChanged(m_Len);
        }
    }
}

bool XMPlayer::load(const QString &fileName)
{
    releaseModule();

    if (xmp_load_module(xmp_ctx, fileName.toLocal8Bit().data()) == 0)
    {
        m_ModuleLoaded = true;
        emit moduleLoadedChanged(m_ModuleLoaded);
        getModuleInfo();
    }

    return m_ModuleLoaded;
}

bool XMPlayer::load(QIODevice *device)
{
    releaseModule();

    QByteArray mem = device->readAll();

    if (xmp_load_module_from_memory(xmp_ctx, mem.data(), mem.length()) == 0)
    {
        m_ModuleLoaded = true;
        emit moduleLoadedChanged(m_ModuleLoaded);
        getModuleInfo();
    }

    return m_ModuleLoaded;
}

bool XMPlayer::loadFromData(const char *data, int length)
{
    releaseModule();

    if (xmp_load_module_from_memory(xmp_ctx, (void *)data, length) == 0)
    {
        m_ModuleLoaded = true;
        emit moduleLoadedChanged(m_ModuleLoaded);
        getModuleInfo();
    }

    return m_ModuleLoaded;
}

bool XMPlayer::loadFromData(const QByteArray &data)
{
    return loadFromData(data.data(), data.length());
}

/*
 * Get called periodically when Audio is playing. It checks if audio buffer can
 * receive more data and, if this is the case, it pulls new frame from XMP.
 *
 * The current module info (bpm, row, pattern etc) are updated and eventually
 * signals emitted.
 */
void XMPlayer::fetchMoreAudioData(void)
{
    if (m_ModuleLoaded && m_AudioStream)
    {
        /* If buffer of frame info is not null, fetch new frame */
        if (m_CurrentFrameInfo.buffer == NULL)
        {
            if (xmp_play_frame(xmp_ctx) == 0)
            {
                xmp_get_frame_info(xmp_ctx, &m_CurrentFrameInfo);
                if (m_CurrentFrameInfo.loop_count > 0)
                {
                    m_LastFrameFetched = true;
                    m_CurrentFrameInfo.buffer = NULL;
                }
            }
            else
            {
                m_LastFrameFetched = true;
            }
        }

        /* Does the frame exist */
        if (m_CurrentFrameInfo.buffer && m_AudioOutput->bytesFree() >= m_CurrentFrameInfo.buffer_size)
        {
            /*
             * If the buffer fits entirely, write it complete. If not, write part of it and update
             * the buffer address and size
             */
            int freeSpace = m_AudioOutput->bytesFree();

            if (m_CurrentFrameInfo.buffer_size > freeSpace)
            {
                m_AudioStream->write((const char *)m_CurrentFrameInfo.buffer, freeSpace);
                m_CurrentFrameInfo.buffer = static_cast<char *>(m_CurrentFrameInfo.buffer) + freeSpace;
                m_CurrentFrameInfo.buffer_size -= freeSpace;
            }
            else
            {
                m_AudioStream->write((const char *)m_CurrentFrameInfo.buffer, m_CurrentFrameInfo.buffer_size);
                m_CurrentFrameInfo.buffer = NULL;
                m_CurrentFrameInfo.buffer_size = 0;
            }

            if (m_Pos != m_CurrentFrameInfo.pos) {
                m_Pos = m_CurrentFrameInfo.pos;
                emit posChanged(m_Pos);
            }

            if (m_Pattern != m_CurrentFrameInfo.pattern) {
                m_Pattern = m_CurrentFrameInfo.pattern;
                emit patternChanged(m_Pattern);
            }

            if (m_Row != m_CurrentFrameInfo.row) {
                m_Row = m_CurrentFrameInfo.row;
                emit rowChanged(m_Row);
            }

            if (m_NumRows != m_CurrentFrameInfo.num_rows) {
                m_NumRows = m_CurrentFrameInfo.num_rows;
                emit numRowsChanged(m_NumRows);
            }

            if (m_BPM != m_CurrentFrameInfo.bpm) {
                m_BPM = m_CurrentFrameInfo.bpm;
                emit bpmChanged(m_BPM);
            }
        }
    }
}

void XMPlayer::audioStateChanged(QAudio::State newState)
{
    switch (newState)
    {
        case QAudio::IdleState:
            //if (!m_IgnoreIdleState)
            {
                qDebug() << "idle state, buffer underrun?";
                if (!m_LastFrameFetched)
                    fetchMoreAudioData();
                else
                    emit playFinished();
                /*
                m_AudioOutput->stop();
                m_AudioStream = NULL;
                emit playStopped();
                if (m_LastFrameFetched)
                    emit playFinished();
                */
            }
            m_IgnoreIdleState = false;
            break;

        case QAudio::StoppedState:
            qDebug() << "stopped state";
            if (m_AudioOutput->error() != QAudio::NoError) {
                qDebug() << "audio error: " << m_AudioOutput->error();
            }
            emit playStopped();
            if (m_LastFrameFetched)
                emit playFinished();
            break;

        default:
            qDebug() << "other state: " << newState;
            break;
    }
}

void XMPlayer::playStart()
{
    if (m_ModuleLoaded)
    {
        m_IgnoreIdleState = true;
        m_LastFrameFetched = false;

        xmp_start_player(xmp_ctx, m_AudioFormat.sampleRate(), 0);
        xmp_set_player(xmp_ctx, XMP_PLAYER_INTERP, XMP_INTERP_SPLINE);
        xmp_set_player(xmp_ctx, XMP_PLAYER_DSP, XMP_DSP_ALL);
        xmp_set_player(xmp_ctx, XMP_PLAYER_MIX, 50);

        m_AudioStream = m_AudioOutput->start();

        emit playStarted();
    }
}

void XMPlayer::playStop()
{
    if (m_ModuleLoaded && m_AudioStream)
    {
        m_AudioOutput->stop();
    }
}

void XMPlayer::playPause()
{
    if (m_ModuleLoaded && m_AudioStream)
    {
        m_AudioOutput->suspend();
    }
}

void XMPlayer::playResume()
{
    if (m_ModuleLoaded && m_AudioStream)
    {
        m_AudioOutput->resume();
    }
}

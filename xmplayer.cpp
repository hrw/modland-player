#include <QDebug>

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

    m_AudioOutput = new QAudioOutput(m_AudioFormat, this);
    m_AudioOutput->setNotifyInterval(1000);
    connect(m_AudioOutput, SIGNAL(notify()), this, SLOT(fetchMoreAudioData()));
    connect(m_AudioOutput, SIGNAL(stateChanged(QAudio::State)), this, SLOT(audioStateChanged(QAudio::State)));
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
    }
}

bool XMPlayer::load(const QString &fileName)
{
    releaseModule();

    if (xmp_load_module(xmp_ctx, fileName.toLocal8Bit().data()) == 0)
    {
        m_ModuleLoaded = true;
        emit moduleLoadedChanged(m_ModuleLoaded);
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
            }
            else
            {
                m_LastFrameFetched = true;
            }
        }

        /* Does the frame exist and fit into audio buffer? If yes, write it */
        if (m_CurrentFrameInfo.buffer && m_AudioOutput->bytesFree() >= m_CurrentFrameInfo.buffer_size)
        {
            m_AudioStream->write((const char *)m_CurrentFrameInfo.buffer, m_CurrentFrameInfo.buffer_size);

            m_CurrentFrameInfo.buffer = NULL;
            m_CurrentFrameInfo.buffer_size = 0;

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

            if (m_Frame != m_CurrentFrameInfo.frame) {
                m_Frame = m_CurrentFrameInfo.frame;
                emit frameChanged(m_Frame);
            }

            if (m_Speed != m_CurrentFrameInfo.speed) {
                m_Speed = m_CurrentFrameInfo.speed;
                emit speedChanged(m_Speed);
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
    (void)newState;
}

#include <QFileDialog>
#include <QObject>
#include <Phonon/AudioOutput>
#include <Phonon/SeekSlider>
#include <Phonon/MediaObject>
#include <Phonon/VolumeSlider>
#include <Phonon/BackendCapabilities>

#include "ui_hrwplayer.h"

class HrwPlayer:public QMainWindow, public Ui_HrwPlayer
{
    Q_OBJECT

    public:
	HrwPlayer();
	~HrwPlayer();

    public slots:
        void OpenFileName();

    private:
	Phonon::MediaObject *mediaObject;
	Phonon::MediaObject *metaInformationResolver;
	Phonon::AudioOutput *audioOutput;

    private slots:
    void StateChanged(Phonon::State newState, Phonon::State oldState);
    void PlaySelected(QListWidgetItem* selectedItem);
    void FinishedPlaying();
    void tick(qint64 time);

};

#include <QFileDialog>
#include <QObject>
#include <Phonon/AudioOutput>
#include <Phonon/SeekSlider>
#include <Phonon/MediaObject>
#include <Phonon/VolumeSlider>
#include <Phonon/BackendCapabilities>

#include "ui_hrwplayer.h"

class HrwPlayer:public QWidget, public Ui_HrwPlayer
{
    Q_OBJECT

    public:
	HrwPlayer(QWidget *parent = 0);
	~HrwPlayer();

    public slots:
	void OpenFileName();

    private:
	Phonon::MediaObject *mediaObject;
	Phonon::MediaObject *metaInformationResolver;
	Phonon::AudioOutput *audioOutput;

};

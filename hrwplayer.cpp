#include <QFileInfo>

#include "hrwplayer.h"

QString moduleFileName = "../daydreams2.mod";

HrwPlayer::HrwPlayer(QWidget *parent):QWidget(parent)
{
    setupUi(this);

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInformationResolver = new Phonon::MediaObject(this);

    mediaObject->setTickInterval(1000); // for remaining time display

    Phonon::createPath(mediaObject, audioOutput);

//    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
//    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
//            this, SLOT(stateChanged(Phonon::State,Phonon::State)));
//    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
//            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
//    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
//            this, SLOT(sourceChanged(Phonon::MediaSource)));
//    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));

    connect(LoadButton, SIGNAL(clicked()), this, SLOT(OpenFileName()));

    seekSlider->setMediaObject(videoPlayer->mediaObject());
    volumeSlider->setAudioOutput(videoPlayer->audioOutput());
};

HrwPlayer::~HrwPlayer() {};

void HrwPlayer::OpenFileName()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, "Select module", moduleFileName);

    if (!fileName.isEmpty())
    {
	videoPlayer->load(fileName);
	QFileInfo fileinfo(fileName);
	songTitle->setText(fileinfo.baseName());
    }
}

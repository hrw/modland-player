#include <QFileInfo>
#include <QMessageBox>

#include "hrwplayer.h"

QString moduleFileName = "../daydreams2.mod";

HrwPlayer::HrwPlayer()
{
    setupUi(this);

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInformationResolver = new Phonon::MediaObject(this);

    mediaObject->setTickInterval(1000); // for remaining time display

    Phonon::createPath(mediaObject, audioOutput);

//    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
	    this, SLOT(StateChanged(Phonon::State,Phonon::State)));
//    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
//            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
//    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
//            this, SLOT(sourceChanged(Phonon::MediaSource)));
//    connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));

    connect(LoadButton,  SIGNAL(clicked()), this,        SLOT(OpenFileName()));
    connect(PlayButton,  SIGNAL(clicked()), mediaObject, SLOT(play()));
    connect(PauseButton, SIGNAL(clicked()), mediaObject, SLOT(pause()) );
    connect(StopButton,  SIGNAL(clicked()), mediaObject, SLOT(stop()));

    seekSlider->setMediaObject(mediaObject);
    volumeSlider->setAudioOutput(audioOutput);
};

HrwPlayer::~HrwPlayer() {};

void HrwPlayer::OpenFileName()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, "Select module", moduleFileName);

    if (!fileName.isEmpty())
    {
	mediaObject->setCurrentSource(fileName);
	QFileInfo fileinfo(fileName);
	TitleLabel->setText(fileinfo.baseName());
    }
}

void HrwPlayer::StateChanged(Phonon::State newState, Phonon::State /* oldState */)
{
    switch (newState) {
        case Phonon::ErrorState:
            if (mediaObject->errorType() == Phonon::FatalError) {
                QMessageBox::warning(this, tr("Fatal Error"),
                mediaObject->errorString());
            } else {
                QMessageBox::warning(this, tr("Error"),
                mediaObject->errorString());
            }
            break;
//![9]
//![10]
        case Phonon::PlayingState:
                PlayButton->setEnabled(false);
                PauseButton->setEnabled(true);
                StopButton->setEnabled(true);
                break;
        case Phonon::StoppedState:
                StopButton->setEnabled(false);
                PlayButton->setEnabled(true);
                PauseButton->setEnabled(false);
//                timeLcd->display("00:00");
                break;
        case Phonon::PausedState:
                PauseButton->setEnabled(false);
                StopButton->setEnabled(true);
                PlayButton->setEnabled(true);
                break;
//![10]
        case Phonon::BufferingState:
                break;
        default:
            ;
    }
}

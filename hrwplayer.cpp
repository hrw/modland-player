#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QTime>

#include "hrwplayer.h"

QString moduleFileName;

HrwPlayer::HrwPlayer()
{
    setupUi(this);

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    mediaObject = new Phonon::MediaObject(this);
    metaInformationResolver = new Phonon::MediaObject(this);

    mediaObject->setTickInterval(1000); // for remaining time display

    Phonon::createPath(mediaObject, audioOutput);

    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
	    this, SLOT(StateChanged(Phonon::State,Phonon::State)));
    //    connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
    //            this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
    //    connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)),
    //            this, SLOT(sourceChanged(Phonon::MediaSource)));
    connect(mediaObject, SIGNAL(finished()), this, SLOT(FinishedPlaying()));

    connect(LoadButton,  SIGNAL(clicked()), this,        SLOT(OpenFileName()));
    connect(SongsList,   SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PlaySelected(QListWidgetItem*)));
    connect(PlayButton,  SIGNAL(clicked()), mediaObject, SLOT(play()));
    connect(PauseButton, SIGNAL(clicked()), mediaObject, SLOT(pause()) );
    connect(StopButton,  SIGNAL(clicked()), mediaObject, SLOT(stop()));

    seekSlider->setMediaObject(mediaObject);
    volumeSlider->setAudioOutput(audioOutput);


    QDir moduleList("./Jester/");
    QStringList filters;
    filters << "*.mod";
    SongsList->insertItems(0, moduleList.entryList(filters));
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
    switch (newState) 
    {
	case Phonon::ErrorState:
	    if (mediaObject->errorType() == Phonon::FatalError) 
	    {
		QMessageBox::warning(this, tr("Fatal Error"),
			mediaObject->errorString());
	    }
	    else
	    {
		QMessageBox::warning(this, tr("Error"),
			mediaObject->errorString());
	    }
	    break;
	case Phonon::PlayingState:
	    PlayButton->setEnabled(false);
	    PauseButton->setEnabled(true);
	    StopButton->setEnabled(true);
	    break;
	case Phonon::StoppedState:
	    StopButton->setEnabled(false);
	    PlayButton->setEnabled(true);
	    PauseButton->setEnabled(false);
	    break;
	case Phonon::PausedState:
	    PauseButton->setEnabled(false);
	    StopButton->setEnabled(true);
	    PlayButton->setEnabled(true);
	    break;
	case Phonon::BufferingState:
	    break;
	default:
	    ;
    }
}

void HrwPlayer::PlaySelected(QListWidgetItem* selectedItem)
{
    QString fileName = "Jester/";
    fileName.append(selectedItem->text());
    mediaObject->setCurrentSource(fileName);
    QFileInfo fileinfo(fileName);
    TitleLabel->setText(fileinfo.baseName());

    TimeLabel->setText("00:00");
    mediaObject->play();
}

void HrwPlayer::FinishedPlaying()
{
    QListWidgetItem* selectedItem;

    if(SongsList->currentRow() == (SongsList->count() - 1))
    {
	selectedItem = SongsList->item(0);
	SongsList->setCurrentRow(0);
    }
    else
    {
	selectedItem =  SongsList->item(SongsList->currentRow() + 1);
	SongsList->setCurrentRow(SongsList->currentRow() + 1);
    }

    PlaySelected(selectedItem);
}

void HrwPlayer::tick(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

    TimeLabel->setText(displayTime.toString("mm:ss"));
}

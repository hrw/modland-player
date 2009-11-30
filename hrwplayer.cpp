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

    DoConnects();
    InitializeSongsList();
}

void HrwPlayer::DoConnects()
{
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
    connect(AuthorsList, SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PopulateSongs(QListWidgetItem*)));
    connect(PlayButton,  SIGNAL(clicked()), mediaObject, SLOT(play()));
    connect(PauseButton, SIGNAL(clicked()), mediaObject, SLOT(pause()) );
    connect(StopButton,  SIGNAL(clicked()), mediaObject, SLOT(stop()));

    seekSlider->setMediaObject(mediaObject);
    volumeSlider->setAudioOutput(audioOutput);
}

void HrwPlayer::InitializeSongsList()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("utwory.sqlite");
    db.open();

    QSqlQuery query("SELECT DISTINCT(author) as author FROM songs ORDER BY author");
    QStringList authors;
    while (query.next()) {
	authors << query.value(0).toString();
    }
    AuthorsList->insertItems(0, authors);
};

HrwPlayer::~HrwPlayer() {};

void HrwPlayer::OpenFileName()
{
    QString fileName = QFileDialog::getOpenFileName(NULL, "Select module", moduleFileName);

    if (!fileName.isEmpty())
    {
	JustPlay(fileName);
    }
}

void HrwPlayer::JustPlay(QString fileName)
{
    mediaObject->setCurrentSource(fileName);
    QFileInfo fileinfo(fileName);
    TitleLabel->setText("Playing \"" + fileinfo.baseName() + "\" by " + CurrentAuthor);
    TimeLabel->setText("00:00");
    mediaObject->play();
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
    QString fileName = buildModuleName(selectedItem->text());

    qDebug() << "PlaySelected - module to play: " << fileName ;

    if(!QFile::exists(fileName))
    {
	qDebug() << "PlaySelected - module not available";

	FetchSong(buildModuleName(selectedItem->text(), false));
    }
    else
    {
	JustPlay(fileName);
    }
}

void HrwPlayer::PopulateSongs(QListWidgetItem* selectedItem)
{
    CurrentAuthor = selectedItem->text();
    QSqlQuery query("SELECT title FROM songs WHERE author = '" + CurrentAuthor + "' ORDER BY title");

    qDebug() << "PopulateSongs - switching to author: " << CurrentAuthor;

    QStringList songs;
    while (query.next()) {
	songs << query.value(0).toString().remove(QRegExp(".mod$"));
    }

    SongsList->clear();
    SongsList->insertItems(0, songs);
}

void HrwPlayer::FinishedPlaying()
{
    QListWidgetItem* selectedItem;

    if(SongsList->currentRow() == (SongsList->count() - 1))
    {
	// for looping over one author
//        selectedItem = SongsList->item(0);
//        SongsList->setCurrentRow(0);
	PopulateSongs(AuthorsList->item(AuthorsList->currentRow() + 1));
	selectedItem =  SongsList->item(0);
    }
    else
    {
	selectedItem =  SongsList->item(SongsList->currentRow() + 1);
	SongsList->setCurrentRow(SongsList->currentRow() + 1);
    }

    qDebug() << "play?";
    PlaySelected(selectedItem);
}

void HrwPlayer::tick(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

    TimeLabel->setText(displayTime.toString("mm:ss"));
}

void HrwPlayer::FetchSong(QString fileName)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QString urlSong = "ftp://ftp.amigascne.org/mirrors/ftp.modland.com/pub/modules/Protracker/" + fileName ;

    qDebug() << "FetchSong - module to fetch: " << urlSong ;

    manager->get(QNetworkRequest(QUrl(urlSong)));
}


void HrwPlayer::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();

    if(reply->error())
    {
	//TODO
	qDebug() << "downloadFinished todo ";
    }
    else
    {
	QDir katalog("modules/");
	katalog.mkpath(CurrentAuthor);

	QFileInfo fileinfo(url.path());

	QString fileName = buildModuleName(fileinfo.baseName());

	QFile file(fileName);
	if(file.open(QIODevice::WriteOnly))
	{
	    file.write(reply->readAll());
	    file.close();

	    qDebug() << "downloadFinished - module fetched";

	    JustPlay(fileName);
	}
    }
}

QString HrwPlayer::buildModuleName(QString title, bool localName)
{
    QString fullName;

    if(localName)
	fullName.append("modules/");

    fullName += CurrentAuthor + "/" + title + ".mod";

    return fullName;
}

#include "hrwplayer.h"

QString moduleFileName;

HrwPlayer::HrwPlayer()
{
    qDebug() << "HrwPlayer::HrwPlayer()";

    mainUI = new DesktopUI();

    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, mainUI);
    mediaObject = new Phonon::MediaObject(mainUI);
    metaInformationResolver = new Phonon::MediaObject(mainUI);

    mediaObject->setTickInterval(1000); // for remaining time display

    Phonon::createPath(mediaObject, audioOutput);

//    PlayButton->setIcon(QIcon::fromTheme("media-playback-start"));
//    StopButton->setIcon(QIcon::fromTheme("media-playback-stop"));
//    PauseButton->setIcon(QIcon::fromTheme("media-playback-pause"));
//    NextButton->setIcon(QIcon::fromTheme("media-skip-forward"));
//    PrevButton->setIcon(QIcon::fromTheme("media-skip-backward"));
//    FavoriteButton->setIcon(QIcon::fromTheme("bookmarks"));

    DoConnects();
    InitializeSongsList();
}

void HrwPlayer::InitializeSongsList()
{
    qDebug() << "HrwPlayer::InitializemainUI->SongsList()";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("utwory.sqlite");
    db.open();

    QSqlQuery query("SELECT id, title FROM authors ORDER BY title");
    QStringList authors;
    while (query.next()) {
	authors << query.value(1).toString();
    }
    mainUI->AuthorsList->insertItems(1, authors);
    PopulateSongs(mainUI->AuthorsList->item(0));
};

HrwPlayer::~HrwPlayer() {};

void HrwPlayer::JustPlay(QString fileName)
{
    qDebug() << "HrwPlayer::JustPlay()";

    mediaObject->setCurrentSource(fileName);
    QFileInfo fileinfo(fileName);
    mainUI->TitleLabel->setText("Playing \"" + fileinfo.baseName() + "\" by " + CurrentAuthor);
    mainUI->TimeLabel->setText("00:00");
    mediaObject->play();
}

void HrwPlayer::StateChanged(Phonon::State newState, Phonon::State /* oldState */)
{
    qDebug() << "HrwPlayer::StateChanged()";

    switch (newState)
    {
	case Phonon::ErrorState:
	    if (mediaObject->errorType() == Phonon::FatalError)
	    {
//                QMessageBox::warning(this, tr("Fatal Error"), mediaObject->errorString());
	    }
	    else
	    {
//                QMessageBox::warning(this, tr("Error"), mediaObject->errorString());
	    }
	    break;
//        case Phonon::PlayingState:
//            PlayButton->setEnabled(false);
//            PauseButton->setEnabled(true);
//            StopButton->setEnabled(true);
//            break;
//        case Phonon::StoppedState:
//            StopButton->setEnabled(false);
//            PlayButton->setEnabled(true);
//            PauseButton->setEnabled(false);
//            break;
//        case Phonon::PausedState:
//            PauseButton->setEnabled(false);
//            StopButton->setEnabled(true);
//            PlayButton->setEnabled(true);
//            break;
	case Phonon::BufferingState:
	    break;
	default:
	    ;
    }
}

void HrwPlayer::PlaySelected(QListWidgetItem* selectedItem)
{
    qDebug() << "HrwPlayer::PlaySelected()";

    QString fileName = buildModuleName(selectedItem->text());

    qDebug() << "\t" << "PlaySelected - module to play: " << fileName ;

    if(!QFile::exists(fileName))
    {
	qDebug() << "\t" << "PlaySelected - module not available";

	FetchSong(buildModuleName(selectedItem->text(), false));
    }
    else
    {
	JustPlay(fileName);
    }
}

void HrwPlayer::PopulateSongs(QListWidgetItem* selectedItem)
{
    qDebug() << "HrwPlayer::PopulateSongs()";

    CurrentAuthor = selectedItem->text();
    qDebug() << "SELECT id FROM authors WHERE title = '" + CurrentAuthor + "'";
    QSqlQuery query("SELECT id FROM authors WHERE title = '" + CurrentAuthor + "'");
    
    query.first();
    query.exec("SELECT title FROM songs WHERE author_id = " + query.value(0).toString() + " ORDER BY title");

    qDebug() << "\t" << "PopulateSongs - switching to author: " << CurrentAuthor;

    qDebug() << "\t" << "SELECT title FROM songs WHERE author_id = " + query.value(0).toString() + " ORDER BY title";

    QStringList songs;
    while (query.next()) {
	songs << query.value(0).toString().remove(QRegExp(".mod$"));
    }

    mainUI->SongsList->clear();
    mainUI->SongsList->insertItems(0, songs);
}

void HrwPlayer::FinishedPlaying()
{
    qDebug() << "HrwPlayer::FinishedPlaying()";

    QListWidgetItem* selectedItem;

    if(mainUI->SongsList->currentRow() == (mainUI->SongsList->count() - 1))
    {
	PopulateSongs(mainUI->AuthorsList->item(mainUI->AuthorsList->currentRow() + 1));
	selectedItem =  mainUI->SongsList->item(0);
	mainUI->AuthorsList->setCurrentRow(mainUI->AuthorsList->currentRow() + 1);
	mainUI->SongsList->setCurrentRow(0);
    }
    else
    {
	selectedItem =  mainUI->SongsList->item(mainUI->SongsList->currentRow() + 1);
	mainUI->SongsList->setCurrentRow(mainUI->SongsList->currentRow() + 1);
    }

    qDebug() << "\t" << "play?";
    PlaySelected(selectedItem);
}

void HrwPlayer::tick(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

    mainUI->TimeLabel->setText(displayTime.toString("mm:ss"));
}

void HrwPlayer::FetchSong(QString fileName)
{
    qDebug() << "HrwPlayer::FetchSong()";

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QString urlSong = "ftp://ftp.amigascne.org/mirrors/ftp.modland.com/pub/modules/Protracker/" + fileName ;

    qDebug() << "\t" << "FetchSong - module to fetch: " << urlSong ;

    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(urlSong)));

    qDebug() << "\t" << "FetchSong - after get" ;
    mainUI->progressBar->reset();
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(handleProgressBar(qint64, qint64)));  
    mainUI->progressBar->setVisible(true);
}

void HrwPlayer::handleProgressBar(qint64 bytesfetched, qint64 bytestotal)
{
    qDebug() << "HrwPlayer::handleProgressBar()";

    mainUI->progressBar->setMaximum(bytestotal);
    mainUI->progressBar->setValue(bytesfetched);
}

void HrwPlayer::downloadFinished(QNetworkReply *reply)
{
    qDebug() << "HrwPlayer::downloadFinished()";

    QUrl url = reply->url();

    if(reply->error())
    {
	//TODO
	qDebug() << "\t" << "downloadFinished todo ";
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

	    qDebug() << "\t" << "downloadFinished - module fetched";

	    JustPlay(fileName);
	}
    }
    mainUI->progressBar->setVisible(false);
}

QString HrwPlayer::buildModuleName(QString title, bool localName)
{
    QString fullName;

    if(localName)
	fullName.append("modules/");

    fullName += CurrentAuthor + "/" + title + ".mod";

    return fullName;
}

void HrwPlayer::handleFavorite()
{
    qDebug() << "HrwPlayer::handleFavorite()";

}

void HrwPlayer::DoConnects()
{
    qDebug() << "HrwPlayer::DoConnects()";

    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
	    this, SLOT(StateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(finished()), this, SLOT(FinishedPlaying()));

    connect(mainUI->SongsList,   SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PlaySelected(QListWidgetItem*)));
    connect(mainUI->AuthorsList, SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PopulateSongs(QListWidgetItem*)));
    connect(mainUI->actionPlay,  SIGNAL(triggered()), mediaObject, SLOT(play()));
    connect(mainUI->actionPause, SIGNAL(triggered()), mediaObject, SLOT(pause()) );
    connect(mainUI->actionStop,  SIGNAL(triggered()), mediaObject, SLOT(stop()));
    connect(mainUI->actionNext,  SIGNAL(triggered()), this, SLOT(FinishedPlaying()));
    connect(mainUI->actionFavorite,  SIGNAL(triggered()), this, SLOT(handleFavorite()));

    mainUI->seekSlider->setMediaObject(mediaObject);
}

void HrwPlayer::show()
{
    mainUI->show();
}

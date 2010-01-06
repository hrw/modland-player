/****************************************************************************
**
** Copyright (C) 2009 Marcin Juszkiewicz
** All rights reserved.
** Contact: Marcin Juszkiewicz <marcin@juszkiewicz.com.pl>
**
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file.  Please review the following information to ensure the GNU
** Lesser General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
****************************************************************************/
#include "hrwplayer.h"

QString moduleFileName;

HrwPlayer::HrwPlayer()
{
    qDebug() << "HrwPlayer::HrwPlayer()";

#ifndef Q_WS_MAEMO_5
    mainUI = new DesktopUI();
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, mainUI);
    mediaObject = new Phonon::MediaObject(mainUI);
    metaInformationResolver = new Phonon::MediaObject(mainUI);
#else
    authorsUI = new MaemoAuthorsUI();
    songsUI   = new MaemoSongsUI(authorsUI);
    playUI    = new MaemoPlayUI(songsUI);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, playUI);
    mediaObject = new Phonon::MediaObject(playUI);
    metaInformationResolver = new Phonon::MediaObject(playUI);
    scroller1 = new QMaemo5KineticScroller(authorsUI->AuthorsList);
    scroller2 = new QMaemo5KineticScroller(songsUI->SongsList);
#endif

    mediaObject->setTickInterval(1000); // for remaining time display

    Phonon::createPath(mediaObject, audioOutput);

    DoConnects();
    InitializeAuthorsList();
}
HrwPlayer::~HrwPlayer() {};

void HrwPlayer::InitializeAuthorsList()
{
    qDebug() << "HrwPlayer::InitializeAuthorsList()";

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("utwory.sqlite");
    db.open();

    QSqlQuery query("SELECT id, title FROM authors ORDER BY title");
    QStringList authors;

    while (query.next()) {
	authors << query.value(1).toString();
    }

    UI_PopulateAuthorsList(authors);
};

void HrwPlayer::UI_PopulateAuthorsList(QStringList authors)
{
    qDebug() << "HrwPlayer::UI_PopulateAuthorsList" ;
#ifndef Q_WS_MAEMO_5
    mainUI->AuthorsList->insertItems(1, authors);
    PopulateSongs(mainUI->AuthorsList->item(0));
#else
    authorsUI->AuthorsList->insertItems(1, authors);
    PopulateSongs(authorsUI->AuthorsList->item(0));
#endif
}

void HrwPlayer::UI_SetSongInfo(QString title)
{
#ifndef Q_WS_MAEMO_5
    mainUI->TitleLabel->setText(title);
    mainUI->TimeLabel->setText("00:00");
#else
    playUI->TitleLabel->setText(title);
    playUI->TimeLabel->setText("00:00");
#endif
}

void HrwPlayer::JustPlay(QString fileName)
{
    qDebug() << "HrwPlayer::JustPlay()";

    mediaObject->setCurrentSource(fileName);
    QFileInfo fileinfo(fileName);
    UI_SetSongInfo("Playing \"" + fileinfo.baseName() + "\" by " + CurrentAuthor);
    mediaObject->play();
#ifdef Q_WS_MAEMO_5
    playUI->show();
#endif
}

void HrwPlayer::StateChanged(Phonon::State newState, Phonon::State /* oldState */)
{
    qDebug() << "HrwPlayer::StateChanged()";

    switch (newState)
    {
	case Phonon::ErrorState:
	    if (mediaObject->errorType() == Phonon::FatalError)
	    {
//                QMessageBox::warning(mainUI, tr("Fatal Error"), mediaObject->errorString());
	    }
	    else
	    {
//                QMessageBox::warning(mainUI, tr("Error"), mediaObject->errorString());
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

    UI_PopulateSongsList(songs);
}

void HrwPlayer::UI_PopulateSongsList(QStringList songs)
{
#ifndef Q_WS_MAEMO_5
    mainUI->SongsList->clear();
    mainUI->SongsList->insertItems(0, songs);
#else
    songsUI->SongsList->clear();
    songsUI->SongsList->insertItems(0, songs);
    songsUI->show();
#endif
}

bool HrwPlayer::UI_IsItLastSong()
{
#ifndef Q_WS_MAEMO_5
    return (mainUI->SongsList->currentRow() == (mainUI->SongsList->count() - 1));
#else
    return (songsUI->SongsList->currentRow() == (songsUI->SongsList->count() - 1));
#endif
}

QListWidgetItem* HrwPlayer::UI_NextAuthorName()
{
#ifndef Q_WS_MAEMO_5
    return mainUI->AuthorsList->item(mainUI->AuthorsList->currentRow() + 1);
#else
    return authorsUI->AuthorsList->item(authorsUI->AuthorsList->currentRow() + 1);
#endif
}

void HrwPlayer::FinishedPlaying()
{
    qDebug() << "HrwPlayer::FinishedPlaying()";

    QListWidgetItem* selectedItem;

    if(UI_IsItLastSong())
    {
	PopulateSongs(UI_NextAuthorName());
#ifndef Q_WS_MAEMO_5
	selectedItem =  mainUI->SongsList->item(0);
	mainUI->AuthorsList->setCurrentRow(mainUI->AuthorsList->currentRow() + 1);
	mainUI->SongsList->setCurrentRow(0);
#else
	selectedItem =  songsUI->SongsList->item(0);
	authorsUI->AuthorsList->setCurrentRow(authorsUI->AuthorsList->currentRow() + 1);
	songsUI->SongsList->setCurrentRow(0);
#endif
    }
    else
    {
#ifndef Q_WS_MAEMO_5
	selectedItem =  mainUI->SongsList->item(mainUI->SongsList->currentRow() + 1);
	mainUI->SongsList->setCurrentRow(mainUI->SongsList->currentRow() + 1);
#else
	selectedItem =  songsUI->SongsList->item(songsUI->SongsList->currentRow() + 1);
	songsUI->SongsList->setCurrentRow(songsUI->SongsList->currentRow() + 1);
#endif
    }

    qDebug() << "\t" << "play?";
    PlaySelected(selectedItem);
}

void HrwPlayer::UI_TotalTime(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

#ifndef Q_WS_MAEMO_5
    mainUI->TotalTimeLabel->setText(displayTime.toString("mm:ss"));
#else
    playUI->TotalTimeLabel->setText(displayTime.toString("mm:ss"));
#endif
}

void HrwPlayer::UI_tick(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

#ifndef Q_WS_MAEMO_5
    mainUI->TimeLabel->setText(displayTime.toString("mm:ss"));
#else
    playUI->TimeLabel->setText(displayTime.toString("mm:ss"));
#endif
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

#ifndef Q_WS_MAEMO_5
    mainUI->progressBar->reset();
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(handleProgressBar(qint64, qint64)));  
    mainUI->progressBar->setVisible(true);
#else
    playUI->progressBar->reset();
    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(handleProgressBar(qint64, qint64)));  
    playUI->progressBar->setVisible(true);
#endif
}

void HrwPlayer::handleProgressBar(qint64 bytesfetched, qint64 bytestotal)
{
    qDebug() << "HrwPlayer::handleProgressBar()";

#ifndef Q_WS_MAEMO_5
    mainUI->progressBar->setMaximum(bytestotal);
    mainUI->progressBar->setValue(bytesfetched);
#else
    playUI->progressBar->setMaximum(bytestotal);
    playUI->progressBar->setValue(bytesfetched);
#endif
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
#ifndef Q_WS_MAEMO_5
    mainUI->progressBar->setVisible(false);
#else
    playUI->progressBar->setVisible(false);
#endif
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

#ifndef Q_WS_MAEMO_5
    mainUI->seekSlider->setMediaObject(mediaObject);
    connect(mainUI->SongsList,   SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PlaySelected(QListWidgetItem*)));
    connect(mainUI->AuthorsList, SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PopulateSongs(QListWidgetItem*)));
    connect(mainUI->actionPlay,  SIGNAL(triggered()), mediaObject, SLOT(play()));
    connect(mainUI->actionPause, SIGNAL(triggered()), mediaObject, SLOT(pause()) );
    connect(mainUI->actionStop,  SIGNAL(triggered()), mediaObject, SLOT(stop()));
    connect(mainUI->actionNext,  SIGNAL(triggered()), this, SLOT(FinishedPlaying()));
    connect(mainUI->actionFavorite,  SIGNAL(triggered()), this, SLOT(handleFavorite()));
#else
    playUI->seekSlider->setMediaObject(mediaObject);
    connect(songsUI->SongsList,   SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PlaySelected(QListWidgetItem*)));
    connect(authorsUI->AuthorsList, SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PopulateSongs(QListWidgetItem*)));
#endif

    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(UI_tick(qint64)));
    connect(mediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(UI_TotalTime(qint64)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
	    this, SLOT(StateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(finished()), this, SLOT(FinishedPlaying()));

}

void HrwPlayer::show()
{
#ifndef Q_WS_MAEMO_5
    mainUI->show();
#else
    authorsUI->show();
#endif
}

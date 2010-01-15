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
#include "modland-player.h"

QString moduleFileName;

ModlandPlayer::ModlandPlayer()
{
    qDebug() << "ModlandPlayer::ModlandPlayer()";

#ifndef Q_WS_MAEMO_5
    mainUI = new DesktopUI();
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, mainUI);
    mediaObject = new Phonon::MediaObject(mainUI);
    metaInformationResolver = new Phonon::MediaObject(mainUI);
    progressDialog = new QProgressDialog(mainUI);
#else
    authorsUI = new MaemoAuthorsUI();
    playUI    = new MaemoPlayUI(authorsUI);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, playUI);
    mediaObject = new Phonon::MediaObject(playUI);
    metaInformationResolver = new Phonon::MediaObject(playUI);
    scroller1 = new QMaemo5KineticScroller(authorsUI->AuthorsList);
    scroller2 = new QMaemo5KineticScroller(playUI->SongsList);
    progressDialog = new QProgressDialog(playUI);
#endif

    mediaObject->setTickInterval(1000); // for remaining time display
    progressDialog->setCancelButton(0);	// hide cancel button

    Phonon::createPath(mediaObject, audioOutput);

    DoConnects();
    InitializeAuthorsList();
}
ModlandPlayer::~ModlandPlayer() {};

void ModlandPlayer::InitializeAuthorsList()
{
    qDebug() << "ModlandPlayer::InitializeAuthorsList()";

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

void ModlandPlayer::UI_PopulateAuthorsList(QStringList authors)
{
    qDebug() << "ModlandPlayer::UI_PopulateAuthorsList" ;
#ifndef Q_WS_MAEMO_5
    mainUI->AuthorsList->insertItems(1, authors);
    PopulateSongs(mainUI->AuthorsList->item(0));
#else
    authorsUI->AuthorsList->insertItems(1, authors);
    PopulateSongs(authorsUI->AuthorsList->item(0));
#endif
}

void ModlandPlayer::UI_SetSongInfo(QString title)
{
#ifndef Q_WS_MAEMO_5
    mainUI->TitleLabel->setText(title);
    mainUI->TimeLabel->setText("00:00");
#else
    playUI->TitleLabel->setText(title);
    playUI->TimeLabel->setText("00:00");
#endif
}

void ModlandPlayer::JustPlay(QString fileName)
{
    qDebug() << "ModlandPlayer::JustPlay()";

    mediaObject->setCurrentSource(fileName);
    QFileInfo fileinfo(fileName);
    UI_SetSongInfo("Playing \"" + fileinfo.baseName() + "\" by " + CurrentAuthor);
    mediaObject->play();
#ifdef Q_WS_MAEMO_5
    playUI->show();
#endif
}

void ModlandPlayer::StateChanged(Phonon::State newState, Phonon::State /* oldState */)
{
    qDebug() << "ModlandPlayer::StateChanged()";

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

void ModlandPlayer::PlaySelected(QListWidgetItem* selectedItem)
{
    qDebug() << "ModlandPlayer::PlaySelected()";

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

void ModlandPlayer::PopulateSongs(QListWidgetItem* selectedItem)
{
    qDebug() << "ModlandPlayer::PopulateSongs()";

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

void ModlandPlayer::UI_PopulateSongsList(QStringList songs)
{
#ifndef Q_WS_MAEMO_5
    mainUI->SongsList->clear();
    mainUI->SongsList->insertItems(0, songs);
#else
    playUI->SongsList->clear();
    playUI->SongsList->insertItems(0, songs);
    playUI->show();
#endif
}

bool ModlandPlayer::UI_IsItLastSong()
{
#ifndef Q_WS_MAEMO_5
    return (mainUI->SongsList->currentRow() == (mainUI->SongsList->count() - 1));
#else
    return (playUI->SongsList->currentRow() == (playUI->SongsList->count() - 1));
#endif
}

QListWidgetItem* ModlandPlayer::UI_NextAuthorName()
{
#ifndef Q_WS_MAEMO_5
    return mainUI->AuthorsList->item(mainUI->AuthorsList->currentRow() + 1);
#else
    return authorsUI->AuthorsList->item(authorsUI->AuthorsList->currentRow() + 1);
#endif
}

void ModlandPlayer::FinishedPlaying()
{
    qDebug() << "ModlandPlayer::FinishedPlaying()";

    QListWidgetItem* selectedItem;

    if(UI_IsItLastSong())
    {
	PopulateSongs(UI_NextAuthorName());
#ifndef Q_WS_MAEMO_5
	selectedItem =  mainUI->SongsList->item(0);
	mainUI->AuthorsList->setCurrentRow(mainUI->AuthorsList->currentRow() + 1);
	mainUI->SongsList->setCurrentRow(0);
#else
	selectedItem =  playUI->SongsList->item(0);
	authorsUI->AuthorsList->setCurrentRow(authorsUI->AuthorsList->currentRow() + 1);
	playUI->SongsList->setCurrentRow(0);
#endif
    }
    else
    {
#ifndef Q_WS_MAEMO_5
	selectedItem =  mainUI->SongsList->item(mainUI->SongsList->currentRow() + 1);
	mainUI->SongsList->setCurrentRow(mainUI->SongsList->currentRow() + 1);
#else
	selectedItem =  playUI->SongsList->item(playUI->SongsList->currentRow() + 1);
	playUI->SongsList->setCurrentRow(playUI->SongsList->currentRow() + 1);
#endif
    }

    qDebug() << "\t" << "play?";
    PlaySelected(selectedItem);
}

void ModlandPlayer::UI_TotalTime(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

#ifndef Q_WS_MAEMO_5
    mainUI->TotalTimeLabel->setText(displayTime.toString("mm:ss"));
#else
    playUI->TotalTimeLabel->setText(displayTime.toString("mm:ss"));
#endif
}

void ModlandPlayer::UI_tick(qint64 time)
{
    QTime displayTime(0, (time / 60000) % 60, (time / 1000) % 60);

#ifndef Q_WS_MAEMO_5
    mainUI->TimeLabel->setText(displayTime.toString("mm:ss"));
#else
    playUI->TimeLabel->setText(displayTime.toString("mm:ss"));
#endif
}

void ModlandPlayer::FetchSong(QString fileName)
{
    qDebug() << "ModlandPlayer::FetchSong()";

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QString urlSong = "http://ftp.amigascne.org/mirrors/ftp.modland.com/pub/modules/Protracker/" + fileName ;

    qDebug() << "\t" << "FetchSong - module to fetch: " << urlSong ;

    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(urlSong)));

    qDebug() << "\t" << "FetchSong - after get" ;

    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(handleProgressBar(qint64, qint64)));  
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleError(QNetworkReply::NetworkError)));
    progressDialog->setLabelText("Fetching " + fileName);
    progressDialog->show();
    qDebug() << "\t" << "FetchSong - end" ;
}

void ModlandPlayer::handleError(QNetworkReply::NetworkError code)
{
    qDebug() << "ModlandPlayer::handleError()";
//    progressDialog->hide();
//    QErrorMessage message;
//    
//    message.showMessage(reply->errorString());
}

void ModlandPlayer::handleProgressBar(qint64 bytesfetched, qint64 bytestotal)
{
    qDebug() << "ModlandPlayer::handleProgressBar()";

    progressDialog->setMaximum(bytestotal);
    progressDialog->setValue(bytesfetched);
}

void ModlandPlayer::downloadFinished(QNetworkReply *reply)
{
    qDebug() << "ModlandPlayer::downloadFinished()";

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
}

QString ModlandPlayer::buildModuleName(QString title, bool localName)
{
    QString fullName;

    if(localName)
	fullName.append("modules/");

    fullName += CurrentAuthor + "/" + title + ".mod";

    return fullName;
}

void ModlandPlayer::handleFavorite()
{
    qDebug() << "ModlandPlayer::handleFavorite()";

}

void ModlandPlayer::DoConnects()
{
    qDebug() << "ModlandPlayer::DoConnects()";

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
    connect(playUI->SongsList,   SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PlaySelected(QListWidgetItem*)));
    connect(authorsUI->AuthorsList, SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PopulateSongs(QListWidgetItem*)));
#endif

    connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(UI_tick(qint64)));
    connect(mediaObject, SIGNAL(totalTimeChanged(qint64)), this, SLOT(UI_TotalTime(qint64)));
    connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
	    this, SLOT(StateChanged(Phonon::State,Phonon::State)));
    connect(mediaObject, SIGNAL(finished()), this, SLOT(FinishedPlaying()));

}

void ModlandPlayer::show()
{
#ifndef Q_WS_MAEMO_5
    mainUI->show();
#else
    authorsUI->show();
#endif
}

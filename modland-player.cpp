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

    mainUI = new DesktopUI();
    modulePath = "modules/";

    DoConnects();
    InitializeAuthorsList();

    sound_init(44100, 2);

    xmp_ctx = xmp_create_context();
    playerThread = new QThread();
}

ModlandPlayer::~ModlandPlayer()
{
    sound_deinit();
}

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
    mainUI->AuthorsList->insertItems(1, authors);
    PopulateSongs(mainUI->AuthorsList->item(0));
}

void ModlandPlayer::UI_SetSongInfo(const xmp_module* mi)
{
    mainUI->TitleInfo->setText(mi->name);
    mainUI->TypeInfo->setText(mi->type);
    mainUI->playBar->setMaximum(mi->len);

    QString instruments;

    for(int i = 0; i < mi->ins; i++)
    {
            struct xmp_instrument *ins = &mi->xxi[i];
            instruments += QString::asprintf("%02d: ", i);
            instruments += ins->name;
            instruments += "\n";
    }
    mainUI->InstrumentsList->setPlainText(instruments);
}

void ModlandPlayer::JustPlay(QString fileName)
{
    qDebug() << "ModlandPlayer::JustPlay()";

    struct xmp_module_info mi;

    QByteArray ba = fileName.toLocal8Bit();
    xmp_load_module(xmp_ctx, ba.data());
    /* Show module data */

    xmp_get_module_info(xmp_ctx, &mi);
    UI_SetSongInfo(mi.mod);

    if( playerThread->isRunning() )
    {
        playerThread->exit();
        playerThread->wait();
    }

    playerThread = QThread::create(PlayModule, xmp_ctx, mainUI->playBar);
    playerThread->setObjectName(mi.mod->name);
    connect(playerThread, SIGNAL(finished()), this, SLOT(FinishedPlaying()));
    connect(playerThread, SIGNAL(finished()), playerThread, SLOT (deleteLater()));
    playerThread->start();
}

void PlayModule(xmp_context xmp_ctx, QProgressBar* playBar)
{
    struct xmp_frame_info fi;

    qDebug() << "PlayModule() start";
    if (xmp_start_player(xmp_ctx, 44100, 0) == 0) {

	    /* Play module */

	    int row = -1;
	    while (xmp_play_frame(xmp_ctx) == 0) {
		    xmp_get_frame_info(xmp_ctx, &fi);
		    if (fi.loop_count > 0)
			    break;

                    if ( QThread::currentThread()->isInterruptionRequested() )
                    {
                        qDebug() << "PlayModule asked to quit";
                        break;
                    }
		    sound_play(fi.buffer, fi.buffer_size);

		    if (fi.row != row) {
                            playBar->setValue(fi.pos);
			    row = fi.row;
		    }
	    }
	    xmp_end_player(xmp_ctx);
    }
    qDebug() << "PlayModule() exit";

    // xmp_release_module(xmp_ctx);
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
    QSqlQuery query("SELECT id FROM authors WHERE title = '" + CurrentAuthor + "'");

    query.first();
    query.exec("SELECT title FROM songs WHERE author_id = " + query.value(0).toString() + " ORDER BY title");

    QStringList songs;
    while (query.next()) {
	songs << query.value(0).toString().remove(QRegExp(".mod$"));
    }

    UI_PopulateSongsList(songs);
}

void ModlandPlayer::UI_PopulateSongsList(QStringList songs)
{
    mainUI->SongsList->clear();
    mainUI->SongsList->insertItems(0, songs);
}

bool ModlandPlayer::UI_IsItLastSong()
{
    return (mainUI->SongsList->currentRow() == (mainUI->SongsList->count() - 1));
}

QListWidgetItem* ModlandPlayer::UI_NextAuthorName()
{
    if(mainUI->AuthorsList->currentRow() < mainUI->AuthorsList->count() - 1)
    {
            return mainUI->AuthorsList->item(mainUI->AuthorsList->currentRow() + 1);
    }
    else
    {
            return mainUI->AuthorsList->item(0);
    }
}

void ModlandPlayer::FinishedPlaying()
{
    qDebug() << "ModlandPlayer::FinishedPlaying()";

    QListWidgetItem* selectedItem;

    if(UI_IsItLastSong())
    {
	PopulateSongs(UI_NextAuthorName());
	selectedItem =  mainUI->SongsList->item(0);
	if(mainUI->AuthorsList->currentRow() < mainUI->AuthorsList->count())
        {
	        mainUI->AuthorsList->setCurrentRow(mainUI->AuthorsList->currentRow() + 1);
	}
        else
        {
	        mainUI->AuthorsList->setCurrentRow(0);
        }
	mainUI->SongsList->setCurrentRow(0);
    }
    else
    {
	selectedItem =  mainUI->SongsList->item(mainUI->SongsList->currentRow() + 1);
	mainUI->SongsList->setCurrentRow(mainUI->SongsList->currentRow() + 1);
    }

    PlaySelected(selectedItem);
}

void ModlandPlayer::FetchSong(QString fileName)
{
    qDebug() << "ModlandPlayer::FetchSong()";

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));

    QString urlSong = "http://ftp.amigascne.org/mirrors/ftp.modland.com/pub/modules/Protracker/" + fileName ;

    qDebug() << "\t" << "FetchSong - module to fetch: " << urlSong ;

    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(urlSong)));

    connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(handleProgressBar(qint64, qint64)));  
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(handleError(QNetworkReply::NetworkError)));
    mainUI->progressBar->setFormat("Fetching " + fileName);
    mainUI->progressBar->show();
    qDebug() << "\t" << "FetchSong - end" ;
}

void ModlandPlayer::handleError(QNetworkReply::NetworkError errorcode)
{
    qDebug() << "ModlandPlayer::handleError()";
    mainUI->progressBar->hide();
    
    if (errorcode != QNetworkReply::NoError) 
    {
	QErrorMessage message;
	QString errorValue;
	QMetaObject meta = QNetworkReply::staticMetaObject;

	for (int i=0; i < meta.enumeratorCount(); ++i) 
	{
	    QMetaEnum m = meta.enumerator(i);
	    if (m.name() == QLatin1String("NetworkError")) 
	    {
		errorValue = QLatin1String(m.valueToKey(errorcode));
		break;
	    }
	}

	message.showMessage(errorValue);
    }
}

void ModlandPlayer::handleProgressBar(qint64 bytesfetched, qint64 bytestotal)
{
    qDebug() << "ModlandPlayer::handleProgressBar()";

    mainUI->progressBar->setMaximum(bytestotal);
    mainUI->progressBar->setValue(bytesfetched);
}

void ModlandPlayer::downloadFinished(QNetworkReply *reply)
{
    qDebug() << "ModlandPlayer::downloadFinished()";

    QUrl url = reply->url();

    if(reply->error())
    {
	//TODO
	qDebug() << "\t" << "downloadFinished error: " + reply->errorString();
    }
    else
    {
	QDir* katalog = new QDir();
	katalog->mkpath(modulePath + CurrentAuthor);

	QFileInfo fileinfo(url.path());

	QString fileName = buildModuleName(fileinfo.baseName());

	qDebug() << "\t" << "downloadFinished - module will be " + fileName;
	QFile file(fileName);
	if(file.open(QIODevice::WriteOnly))
	{
	    file.write(reply->readAll());
	    file.close();

	    qDebug() << "\t" << "downloadFinished - module fetched";
            mainUI->progressBar->resetFormat();
            mainUI->progressBar->reset();

	    JustPlay(fileName);
	}
	else
	{
	    qDebug() << "\t downloadFinished with error: " + file.errorString();
	}
    }
}

QString ModlandPlayer::buildModuleName(QString title, bool localName)
{
    QString fullName;

    if(localName)
	fullName.append(modulePath);

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

    connect(mainUI->SongsList,   SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PlaySelected(QListWidgetItem*)));
    connect(mainUI->AuthorsList, SIGNAL(itemClicked(QListWidgetItem*)), this,        SLOT(PopulateSongs(QListWidgetItem*)));
    connect(mainUI->actionNext,  SIGNAL(triggered()), this, SLOT(FinishedPlaying()));
    connect(mainUI->actionFavorite,  SIGNAL(triggered()), this, SLOT(handleFavorite()));
}

void ModlandPlayer::show()
{
    mainUI->show();
}

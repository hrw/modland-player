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
#include <Phonon/AudioOutput>
#include <Phonon/BackendCapabilities>
#include <Phonon/MediaObject>
#include <Phonon/SeekSlider>
#include <Phonon/VolumeSlider>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTime>
#include <QtSql>
#include <QListWidget>
#include <QProgressDialog>
#include <QErrorMessage>
#include <QDebug>

#ifndef Q_WS_MAEMO_5
	#include "desktopui.h"
#else
	#include "maemo-authorsui.h"
	#include "maemo-playui.h"
	#include <QtMaemo5>
#endif

class ModlandPlayer:public QObject
{
    Q_OBJECT

    public:
	ModlandPlayer();
	~ModlandPlayer();
	void show();

    public slots:

    private:
#ifndef Q_WS_MAEMO_5
	DesktopUI *mainUI;
#else
	MaemoAuthorsUI *authorsUI;
	MaemoPlayUI    *playUI;
	QMaemo5KineticScroller *scroller1;
	QMaemo5KineticScroller *scroller2;
#endif
	QProgressDialog *progressDialog;
	Phonon::MediaObject *mediaObject;
	Phonon::MediaObject *metaInformationResolver;
	Phonon::AudioOutput *audioOutput;
	QString CurrentAuthor;
	void InitializeAuthorsList();
	void FetchSong(QString fileName);
	void JustPlay(QString fileName);
	QString buildModuleName(QString title, bool localName = true);
	void DoConnects();

	void UI_PopulateAuthorsList(QStringList authors);
	void UI_PopulateSongsList(QStringList songs);
	void UI_SetSongInfo(QString title);
	bool UI_IsItLastSong();
	QListWidgetItem* UI_NextAuthorName();
	QString modulePath;

    private slots:
    void StateChanged(Phonon::State newState, Phonon::State oldState);
    void PlaySelected(QListWidgetItem* selectedItem);
    void PopulateSongs(QListWidgetItem* selectedItem);
    void FinishedPlaying();
    void downloadFinished(QNetworkReply* reply);
    void handleProgressBar(qint64 bytesfetched, qint64 bytestotal);
    void handleError(QNetworkReply::NetworkError code);
    void handleFavorite();
    void UI_tick(qint64 time);
    void UI_TotalTime(qint64 time);

};

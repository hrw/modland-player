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

#include <xmp.h>

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
#include <QThread>
#include <QAudioOutput>

#include "xmplayer.h"

#include "desktopui.h"

#if 0
class PlayThread:public QThread
{
    Q_OBJECT

    private:
    xmp_context xmp_ctx;
    QAudioOutput *audio;

public slots:
    void audio_out_notify();

    public:
        PlayThread(xmp_context xmp_ctx);
        virtual ~PlayThread();
        void run();

    signals:
        void setPosition(int position);
};
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
    XMPlayer    player;
    QThread     playerThread;

	DesktopUI *mainUI;
	QString CurrentAuthor;
	void InitializeAuthorsList();
	void FetchSong(QString fileName);
	void JustPlay(QString fileName);
    void JustPlay(QByteArray file);
	QString buildModuleName(QString title, bool localName = true);
	void DoConnects();

	void UI_PopulateAuthorsList(QStringList authors);
	void UI_PopulateSongsList(QStringList songs);
    void UI_SetSongInfo();
	bool UI_IsItLastSong();
	QListWidgetItem* UI_NextAuthorName();
	void StopPlayerThread();
	QString modulePath;
    //xmp_context xmp_ctx;
    //QThread *playerThread;

    private slots:
    void PlaySelected(QListWidgetItem* selectedItem);
    void PopulateSongs(QListWidgetItem* selectedItem);
    void FinishedPlaying();
    void downloadFinished(QNetworkReply* reply);
    void handleProgressBar(qint64 bytesfetched, qint64 bytestotal);
    void handleError(QNetworkReply::NetworkError code);
    void handleFavorite();
    void UI_UpdatePosition(int pos);
};

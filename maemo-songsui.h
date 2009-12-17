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
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTime>
#include <QtSql>
#include <QMessageBox>

#include "ui_maemo-songs.h"

class MaemoSongsUI:public QMainWindow, public Ui_MaemoSongsUI
{
    Q_OBJECT

    public:
	MaemoSongsUI(QWidget *parent = 0);
	~MaemoSongsUI();

    public slots:

    private:

    private slots:
};

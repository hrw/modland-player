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
#include <QtDBus>

#include <mce/mode-names.h>
#include <mce/dbus-names.h>

#include "ui_maemo-authors.h"

class MaemoAuthorsUI:public QMainWindow, public Ui_MaemoAuthorsUI
{
    Q_OBJECT

    public:
	MaemoAuthorsUI(QWidget *parent = 0);
	~MaemoAuthorsUI();

    public slots:

    private:

    private slots:

    protected:
	bool event(QEvent *ev);
};

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
#include "maemo-authorsui.h"

MaemoAuthorsUI::MaemoAuthorsUI(QWidget *parent): QMainWindow(parent)
{
    qDebug() << "MaemoAuthorsUI::MaemoAuthorsUI()";

    setupUi(this);
    setAttribute(Qt::WA_Maemo5StackedWindow);
}

MaemoAuthorsUI::~MaemoAuthorsUI() {};

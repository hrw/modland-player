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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "xmplayer.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    XMPlayer player;
//    QThread playerThread;
    int retval;

//    player.moveToThread(&playerThread);
//    playerThread.start(QThread::HighestPriority);

    qmlRegisterType<XMPlayer>("XMPlayer", 1, 0, "XMPlayer");

    app.setApplicationName("Modland Player");

    engine.rootContext()->setContextProperty("player", &player);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    if (engine.rootObjects().isEmpty())
        return -1;

    retval = app.exec();

//    playerThread.terminate();
//    playerThread.wait();

    return retval;
}

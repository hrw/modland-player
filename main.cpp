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
#include <QQuickStyle>
#include <QDir>
#include <QFontDatabase>
#include <QNetworkAccessManager>

#include "xmplayer.h"
#include "modland.h"
#include "vumeter.h"

QNetworkAccessManager manager;

int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    QDir dir(":/fonts/");
    dir.setNameFilters(QStringList() << "*.otf" << "*.ttf");
    for (auto file: dir.entryList(QDir::Files))
    {
        QFontDatabase::addApplicationFont(":/fonts/" + file);
    }

    QQuickStyle::setStyle("Material");

    qmlRegisterType<XMPlayer>("XMPlayer", 1, 0, "XMPlayer");
    qmlRegisterType<Modland>("XMPlayer", 1, 0, "Modland");
    qmlRegisterType<VUMeter>("XMPlayer", 1, 0, "VUMeter");

    app.setApplicationName("Modland Player");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}

#include "modland.h"

#include <QVariant>
#include <QSqlQuery>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>

extern QNetworkAccessManager manager;

Modland::Modland(QObject *parent) : QObject(parent)
{
    m_Database = QSqlDatabase::addDatabase("QSQLITE");
    m_Database.setDatabaseName("utwory.sqlite");
    m_Database.open();

    QSqlQuery query("SELECT id, title FROM authors ORDER BY title COLLATE NOCASE ASC");

    while (query.next()) {
        m_Authors.append(query.value(1).toString());
    }

    emit authorsChanged();
}

QStringList Modland::getAuthorsModules(QString author)
{
    QStringList modules;

    QSqlQuery query("SELECT id FROM authors WHERE title = '" + author + "'");

            query.first();
    qDebug() << query.value(0).toString();

    query.exec("SELECT s.filename, s.title FROM songs s, song_author_map m WHERE s.id = m.song_id AND m.author_id = " +
               query.value(0).toString() + " ORDER BY title COLLATE NOCASE ASC");

    while (query.next()) {
        QStringList path = query.value(0).toString().split('/');
        modules.append(path[0] + "/" + path[2]);
    }

    qDebug() << modules;

    return modules;
}

QStringList Modland::getAuthorsModules(int authorIndex)
{
    return getAuthorsModules(m_Authors[authorIndex]);
}

#include <QCoreApplication>

QByteArray Modland::downloadModule(QString author, QString module)
{
    QStringList list = module.split('/');
    QNetworkReply * reply;
    list.insert(1, author);
    QString fileName = list.join('/');

    qDebug() << fileName;

    QString urlSong = "http://ftp.amigascne.org/mirrors/ftp.modland.com/pub/modules/" + fileName ;
    QString urlEnc = urlSong.replace("#", "%23");
    QUrl url = QUrl(urlEnc);

    reply =  manager.get(QNetworkRequest(url));

    qDebug() << reply;

    while (!reply->isFinished())
    {
        QCoreApplication::processEvents();
    }

    QByteArray data = reply->readAll();

    return data;
}

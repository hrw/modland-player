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

    QSqlQuery query("SELECT id, title FROM authors ORDER BY RANDOM() COLLATE NOCASE ASC");

    while (query.next()) {
        m_Authors.append(new AuthorObject(query.value(0).toInt(), query.value(1).toString()));
    }

    emit authorsChanged();
}

void Modland::getListForAuthor(int authorId)
{
    while (!m_AuthorsModules.isEmpty()) {
        ModuleObject *module = static_cast<ModuleObject *>(m_AuthorsModules.takeLast());
        delete module;
    }

    qDebug() << "Modland:getListForAuthor(" << authorId << ")";

    QSqlQuery query(QString("SELECT s.id, s.filename, s.title FROM songs s, song_author_map m WHERE s.id = m.song_id AND m.author_id = %1 ORDER BY title COLLATE NOCASE ASC").arg(authorId));

    while (query.next()) {
        m_AuthorsModules.append(new ModuleObject(query.value(0).toInt(), authorId, query.value(2).toString().section(".", 0, -2), query.value(1).toString()));
    }
}

#include <QCoreApplication>

QByteArray Modland::downloadModule(QString fileName)
{
    QNetworkReply * reply;

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

Modland::~Modland()
{
    while (!m_AuthorsModules.isEmpty()) {
        ModuleObject *module = static_cast<ModuleObject *>(m_AuthorsModules.takeLast());
        delete module;
    }

    while (!m_Authors.isEmpty()) {
        AuthorObject *author = static_cast<AuthorObject *>(m_Authors.takeLast());
        delete author;
    }
}

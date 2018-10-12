#ifndef MODLAND_H
#define MODLAND_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QIODevice>

class Modland : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList authors READ authors NOTIFY authorsChanged)

public:
    explicit Modland(QObject *parent = nullptr);
    QStringList authors() { return m_Authors; }

signals:
    void authorsChanged();

public slots:
    QStringList getAuthorsModules(QString author);
    QStringList getAuthorsModules(int authorIndex);
    QByteArray downloadModule(QString author, QString module);

private:
    QSqlDatabase m_Database;
    QStringList m_Authors;
};

#endif // MODLAND_H

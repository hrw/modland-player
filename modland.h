#ifndef MODLAND_H
#define MODLAND_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QIODevice>
#include <QVariant>

class AuthorObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
    explicit AuthorObject(int id, QString name) : QObject(nullptr), m_Name(name), m_ID(id) {}
    int id() { return m_ID; }
    QString name() { return m_Name; }

signals:
    void idChanged();
    void nameChanged();

private:
    QString m_Name;
    int     m_ID;
};

class ModuleObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(int authorId READ authorId NOTIFY authorIdChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString path READ path NOTIFY pathChanged)

public:
    explicit ModuleObject(int id, int authorId, QString name, QString path) : QObject(nullptr), m_ID(id), m_AuthorID(authorId), m_Name(name), m_Path(path) {}
    int id() { return m_ID; }
    int authorId() { return m_AuthorID; }
    QString name() { return m_Name; }
    QString path() { return m_Path; }

signals:
    void idChanged();
    void authorIdChanged();
    void nameChanged();
    void pathChanged();

private:
    int m_ID;
    int m_AuthorID;
    QString m_Name;
    QString m_Path;
};

class Modland : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant authors READ authors NOTIFY authorsChanged)
    Q_PROPERTY(QVariant authorsModules READ authorsModules NOTIFY authorsModulesChanged)

public:
    explicit Modland(QObject *parent = nullptr);
    virtual ~Modland();
    QVariant authors() { return QVariant::fromValue(m_Authors); }
    QVariant authorsModules() { return QVariant::fromValue(m_AuthorsModules); }

signals:
    void authorsChanged();
    void authorsModulesChanged();

public slots:
    void getListForAuthor(int authorId);
    QByteArray downloadModule(QString path);

private:
    QSqlDatabase m_Database;
    QList<QObject *> m_Authors;
    QList<QObject *> m_AuthorsModules;
};

#endif // MODLAND_H

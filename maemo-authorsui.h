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

#include "ui_maemo-authors.h"

class MaemoAuthorsUI:public QMainWindow, public Ui_MaemoAuthorsUI
{
    Q_OBJECT

    public:
	MaemoAuthorsUI();
	~MaemoAuthorsUI();

    public slots:

    private:

    private slots:
};

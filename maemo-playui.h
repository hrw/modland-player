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

#include "ui_maemo-play.h"

class MaemoPlayUI:public QMainWindow, public Ui_MaemoPlayUI
{
    Q_OBJECT

    public:
	MaemoPlayUI(QWidget *parent = 0);
	~MaemoPlayUI();

    public slots:

    private:

    private slots:
};

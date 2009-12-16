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

#include "ui_maemo-songs.h"

class MaemoSongsUI:public QMainWindow, public Ui_MaemoSongsUI
{
    Q_OBJECT

    public:
	MaemoSongsUI(QWidget *parent = 0);
	~MaemoSongsUI();

    public slots:

    private:

    private slots:
};

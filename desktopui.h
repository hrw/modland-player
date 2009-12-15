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

#include "ui_desktop.h"

class DesktopUI:public QMainWindow, public Ui_DesktopUI
{
    Q_OBJECT

    public:
	DesktopUI();
	~DesktopUI();

    public slots:

    private:

    private slots:
};

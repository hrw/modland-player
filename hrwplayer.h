#include <Phonon/AudioOutput>
#include <Phonon/BackendCapabilities>
#include <Phonon/MediaObject>
#include <Phonon/SeekSlider>
#include <Phonon/VolumeSlider>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTime>
#include <QtSql>

#include <QDebug>

#include "ui_hrwplayer.h"

class HrwPlayer:public QMainWindow, public Ui_HrwPlayer
{
    Q_OBJECT

    public:
	HrwPlayer();
	~HrwPlayer();

    public slots:
        void OpenFileName();

    private:
	Phonon::MediaObject *mediaObject;
	Phonon::MediaObject *metaInformationResolver;
	Phonon::AudioOutput *audioOutput;
	QString CurrentAuthor;
	void DoConnects();
	void InitializeSongsList();
	bool FetchSong(QString fileName);

    private slots:
    void StateChanged(Phonon::State newState, Phonon::State oldState);
    void PlaySelected(QListWidgetItem* selectedItem);
    void PopulateSongs(QListWidgetItem* selectedItem);
    void FinishedPlaying();
    void tick(qint64 time);
    void downloadFinished(QNetworkReply* reply);
    QString buildModuleName(QString title, bool localName = true);

};

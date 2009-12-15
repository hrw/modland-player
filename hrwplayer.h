#include <Phonon/AudioOutput>
#include <Phonon/BackendCapabilities>
#include <Phonon/MediaObject>
#include <Phonon/SeekSlider>
#include <Phonon/VolumeSlider>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QTime>
#include <QtSql>
#include <QListWidget>

#include <QDebug>

#include "desktopui.h"

class HrwPlayer:public QObject
{
    Q_OBJECT

    public:
	HrwPlayer();
	~HrwPlayer();
	void show();

    public slots:

    private:
	DesktopUI *mainUI;
	Phonon::MediaObject *mediaObject;
	Phonon::MediaObject *metaInformationResolver;
	Phonon::AudioOutput *audioOutput;
	QString CurrentAuthor;
	void InitializeAuthorsList();
	void FetchSong(QString fileName);
	void JustPlay(QString fileName);
	QString buildModuleName(QString title, bool localName = true);
	void DoConnects();

	void UI_PopulateAuthorsList(QStringList authors);
	void UI_PopulateSongsList(QStringList songs);
	void UI_SetSongInfo(QString title);
	bool UI_IsItLastSong();
	QString UI_NextAuthorName();


    private slots:
    void StateChanged(Phonon::State newState, Phonon::State oldState);
    void PlaySelected(QListWidgetItem* selectedItem);
    void PopulateSongs(QListWidgetItem* selectedItem);
    void FinishedPlaying();
    void downloadFinished(QNetworkReply* reply);
    void handleProgressBar(qint64 bytesfetched, qint64 bytestotal);
    void handleFavorite();
    void UI_tick(qint64 time);

};

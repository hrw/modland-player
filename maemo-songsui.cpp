#include "maemo-songsui.h"

MaemoSongsUI::MaemoSongsUI(QWidget *parent): QMainWindow(parent)
{
    qDebug() << "MaemoSongsUI::MaemoSongsUI()";

    setupUi(this);
    setAttribute(Qt::WA_Maemo5StackedWindow);
}

MaemoSongsUI::~MaemoSongsUI() {};

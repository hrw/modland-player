#include "maemo-playui.h"

MaemoPlayUI::MaemoPlayUI(QWidget *parent): QMainWindow(parent)
{
    qDebug() << "MaemoPlayUI::MaemoPlayUI()";

    setupUi(this);
    setAttribute(Qt::WA_Maemo5StackedWindow);
}

MaemoPlayUI::~MaemoPlayUI() {};

#include "maemo-authorsui.h"

MaemoAuthorsUI::MaemoAuthorsUI(QWidget *parent): QMainWindow(parent)
{
    qDebug() << "MaemoAuthorsUI::MaemoAuthorsUI()";

    setupUi(this);
    setAttribute(Qt::WA_Maemo5StackedWindow);
}

MaemoAuthorsUI::~MaemoAuthorsUI() {};

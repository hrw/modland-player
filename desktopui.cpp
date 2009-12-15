#include "desktopui.h"

DesktopUI::DesktopUI()
{
    qDebug() << "DesktopUI::DesktopUI()";

    setupUi(this);

    progressBar->setVisible(false);
}

DesktopUI::~DesktopUI() {};


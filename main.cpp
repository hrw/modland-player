
#include <QApplication>

#include "hrwplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Hrw Qt Player");
    QWidget *widget = new QWidget;
    HrwPlayer ui(widget);
    //    ui.setupUi(widget);

    widget->show();

    //    ui.videoPlayer->load(moduleFileName);
    //    ui.videoPlayer->play();
    app.exec();

    return 0;
}

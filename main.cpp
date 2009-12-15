
#include <QApplication>

#include "hrwplayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("Hrw Qt Player");
    HrwPlayer player;

    player.show();
    return app.exec();
}

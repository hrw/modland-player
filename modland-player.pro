######################################################################
# Automatically generated by qmake (2.01a) ?r. lis 25 17:43:14 2009
######################################################################

TEMPLATE = app
TARGET = modland-player
DEPENDPATH += .
INCLUDEPATH += . /usr/include/qt5/QtWidgets
LIBS += -lasound -lxmp
QT += network sql widgets

FORMS += desktop.ui
HEADERS += desktopui.h modland-player.h
SOURCES += desktopui.cpp modland-player.cpp main.cpp alsa.cc

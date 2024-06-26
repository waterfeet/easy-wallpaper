#-------------------------------------------------
#
# Project created by QtCreator 2023-08-04T20:52:09
#
#-------------------------------------------------

QT       += core gui multimediawidgets  network
#axcontainer
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_ICONS = ./image/tubiao.ico

TARGET = wallpaper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        Src/loadingwidget.cpp \
        Src/myqlabel.cpp \
        Src/picwidget.cpp \
        Src/webwidget.cpp \
        Src\main.cpp \
        Src\widget.cpp\
        Src\SwitchButton.cpp

HEADERS += \
        Src/loadingwidget.h \
        Src/myqlabel.h \
        Src/picwidget.h \
        Src/webwidget.h \
        Src\widget.h\
        Src\SwitchButton.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



#-------------------------------------------------
#
# Project created by QtCreator 2018-02-22T22:08:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "SEFMediaPreparer"
TEMPLATE = app
RC_ICONS = seflogo.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += static

SOURCES += \
        main.cpp \
        MediaPreparer.cpp \
    File.cpp \
    VideoLibrary.cpp \
    Settings.cpp

HEADERS += \
        MediaPreparer.h \
    VideoLibrary.h \
    File.h \
    Settings.h \
    SEFMediaPreparer_resource.rc

FORMS += \
        MediaPreparer.ui

DISTFILES += \
    ffmpeg.exe \
    ffprobe.exe \
    seflogo.ico \
    Qt5Core.dll \
    Qt5Gui.dll \
    Qt5Widgets.dll \
    seflogo.png \
    README.md \
    LICENSE \
    .clang-format \
    .gitignore \
    deploy.bat \
    build.bat \
    commit.bat

INCLUDEPATH += \
    ./include

LIBS += -L./include/libs

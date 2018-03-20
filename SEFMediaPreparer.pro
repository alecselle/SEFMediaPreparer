#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T13:19:37
#
#-------------------------------------------------

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SEFMediaPreparer
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += static

SOURCES += \
    ./src/*.cpp

HEADERS += \
    ./src/*.hpp

FORMS += \
    ./src/*.ui

DISTFILES += \
    ./tools/* \
    ./seflogo.png \
    ./README.md \
    ./.gitignore

unix:!macx|win32: LIBS += \
    -L./lib/ -llibboost_filesystem-mgw53-mt-s-x32-1_66 \
    -L./lib/ -llibboost_container-mgw53-mt-s-x32-1_66 \
    -L./lib/ -llibboost_system-mgw53-mt-s-x32-1_66

INCLUDEPATH += \
    ./include
DEPENDPATH += \
    ./include

win32:!win32-g++: PRE_TARGETDEPS += \
   ./ lib/libboost_filesystem-mgw53-mt-s-x32-1_66.a \
    ./lib/libboost_container-mgw53-mt-s-x32-1_66.a \
    ./lib/libboost_system-mgw53-mt-s-x32-1_66.a

QT += core gui webchannel

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SEFMediaPreparer
TEMPLATE = app
ICON = seflogo.png
RC_ICONS = seflogo.ico

CONFIG += static staticlib embed_manifest_dll embed_manifest_exe

QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++

SOURCES += \
    $$PWD/src/main.cpp \
    $$PWD/src/Settings.cpp \
    $$PWD/src/File.cpp \
    $$PWD/src/Library.cpp \
    $$PWD/src/MediaPreparer.cpp

HEADERS += \
    $$PWD/src/Settings.hpp \
    $$PWD/src/File.hpp \
    $$PWD/src/Library.hpp \
    $$PWD/src/MediaPreparer.hpp

FORMS += \
    $$PWD/src/MediaPreparer.ui

DISTFILES += \
    $$PWD/seflogo.png \
    $$PWD/seflogo.ico \
    $$PWD/tools/ffmpeg.exe \
    $$PWD/tools/ffprobe.exe \
    $$PWD/README.md \
    $$PWD/.gitignore \
    version.txt

win32: LIBS += \
    -L$$PWD/lib/ -lboost_filesystem-mgw53-mt-s-x32-1_66 \
    -L$$PWD/lib/ -lboost_container-mgw53-mt-s-x32-1_66 \
    -L$$PWD/lib/ -lboost_system-mgw53-mt-s-x32-1_66 \
    -L$$PWD/lib/ -lboost_program_options-mgw53-mt-s-x32-1_66

INCLUDEPATH += \
    $$PWD/include
DEPENDPATH += \
    $$PWD/include
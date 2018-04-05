QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SEFMediaPreparer
TEMPLATE = app
ICON = seflogo.png
RC_ICONS = seflogo.ico

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
    $$PWD/ffmpeg/ffmpeg.exe \
    $$PWD/ffmpeg/ffprobe.exe \
    $$PWD/README.md \
    $$PWD/.gitignore \
    $$PWD/version.txt

win32: LIBS += \
    -L$$PWD/lib/ -lboost_filesystem-mgw53-mt-x32-1_66 \
    -L$$PWD/lib/ -lboost_container-mgw53-mt-x32-1_66 \
    -L$$PWD/lib/ -lboost_system-mgw53-mt-x32-1_66 \
    -L$$PWD/lib/ -lboost_program_options-mgw53-mt-x32-1_66
	
INCLUDEPATH += \
    $$PWD/include
DEPENDPATH += \
    $$PWD/include
    
QMAKE_CXXFLAGS += -Wno-conversion-null -Wno-sign-compare -Wno-return-type -Wno-pointer-arith
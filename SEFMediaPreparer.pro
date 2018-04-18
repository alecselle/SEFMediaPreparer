QT += core gui widgets
TEMPLATE = app

CONFIG += c++17 c++14 embed_manifest_exe
QMAKE_CXXFLAGS += -std=c++17 -static-libstdc++ -static-libgcc

!defined(VER_MAJ):VER_MAJ = 3
!defined(VER_MIN):VER_MIN = 2
!defined(VER_PAT):VER_PAT = 1
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DESTDIR = $$PWD/bin
TARGET = SEFMediaPreparer-$${VERSION}

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
    $$PWD/src/MediaPreparer.hpp \
    $$PWD/src/product_info.hpp

FORMS += \
    $$PWD/src/MediaPreparer.ui

DISTFILES += \
    $$PWD/src/SEFMediaPreparer.exe.rc \
    $$PWD/SEFMediaPreparer.iss \
    $$PWD/seflogo.png \
    $$PWD/seflogo.ico \
    $$PWD/README.md \
    $$PWD/.gitignore \
    $$PWD/version.txt

LIBS += \
    -LC:/MSYS/mingw32/lib/ -lboost_filesystem-mt \
    -LC:/MSYS/mingw32/lib/ -lboost_container-mt \
    -LC:/MSYS/mingw32/lib/ -lboost_system-mt \
    -L$$PWD/lib/

INCLUDEPATH += \
    C:/MSYS/mingw32/include

DEPENDPATH += \
    C:/MSYS/mingw32/include

DEFINES += \
    VER_MAJ=$$VER_MAJ \
    VER_MIN=$$VER_MIN \
    VER_PAT=$$VER_PAT \
    VERSION=$$VERSION

RC_FILE = $$PWD/src/SEFMediaPreparer.exe.rc
system(echo|set /p="$${VERSION}" >"$$PWD/version.txt")

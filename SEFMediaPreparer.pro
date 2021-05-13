QT += core gui widgets
TEMPLATE = app

CONFIG += c++17 embed_manifest_exe static
QMAKE_CXXFLAGS += -std=c++17 -static-libstdc++ -static-libgcc -Wpedantic -Wno-unknown-pragmas -Wno-suggest-override -Wno-conversion-null -Wno-return-type -Wno-unused-function -Wno-unused-variable -Wno-sign-compare -Wno-deprecated-declarations -Wno-class-memaccess

!defined(VER_MAJ):VER_MAJ = 3
!defined(VER_MIN):VER_MIN = 7
!defined(VER_PAT):VER_PAT = 0
VERSION = $${VER_MAJ}.$${VER_MIN}.$${VER_PAT}

DESTDIR = $$PWD/bin
TARGET = SEFMediaPreparer

SOURCES += \
        $$PWD/src/main.cpp \
		$$PWD/src/MediaPreparer.cpp \
		$$PWD/src/Controller.cpp \
		$$PWD/src/Global.cpp \
		$$PWD/src/Settings.cpp

HEADERS += \
        $$PWD/src/product_info.hpp \
		$$PWD/src/MediaPreparer.hpp \
		$$PWD/src/Controller.hpp \
		$$PWD/src/Global.hpp \
		$$PWD/src/Event.hpp \
		$$PWD/src/Settings.hpp \
		$$PWD/src/File.hpp \
		$$PWD/src/Library.hpp \
		$$PWD/src/Worker.hpp

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
        -L$$PWD/lib -lboost_filesystem-mt


INCLUDEPATH += \
        $$PWD/src \
		$$PWD/include

DEPENDPATH += \
        $$PWD/src \
		$$PWD/include

DEFINES += \
        VER_MAJ=$$VER_MAJ \
		VER_MIN=$$VER_MIN \
		VER_PAT=$$VER_PAT \
		VERSION=$$VERSION

RC_FILE = $$PWD/src/SEFMediaPreparer.exe.rc
system(echo|set /p="$${VERSION}">"$$PWD/version.txt")

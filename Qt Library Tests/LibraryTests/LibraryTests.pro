#-------------------------------------------------
#
# Project created by QtCreator 2016-04-17T18:52:58
#
#-------------------------------------------------

QT       += widgets

TARGET = LibraryTests
TEMPLATE = lib

DEFINES += LIBRARYTESTS_LIBRARY

SOURCES += librarytests.cpp

HEADERS += librarytests.h\
        librarytests_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

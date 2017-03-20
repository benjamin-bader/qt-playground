#-------------------------------------------------
#
# Project created by QtCreator 2017-03-19T13:41:26
#
#-------------------------------------------------

QT       += sql

QT       -= gui

TARGET = gallery-core
TEMPLATE = lib

DEFINES += GALLERYCORE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += Album.cpp \
    Picture.cpp \
    AlbumDao.cpp \
    DatabaseManager.cpp \
    PictureDao.cpp \
    AlbumModel.cpp \
    PictureModel.cpp

HEADERS += Album.h\
        gallery-core_global.h \
    Picture.h \
    AlbumDao.h \
    DatabaseManager.h \
    PictureDao.h \
    AlbumModel.h \
    PictureModel.h

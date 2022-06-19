#-------------------------------------------------
#
# Project created by QtCreator 2022-06-11T10:48:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pacman
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    base.cpp \
    pacman.cpp \
    game.cpp \
    ghost.cpp

HEADERS  += mainwindow.h \
    base.h \
    game.h \
    pacman.h \
    ghost.h
FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc

#------------------------------
#
# Project created by QtCreator
#
#------------------------------

QT       += core gui

TARGET = IntroductionOpenCV
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    histogramwindow.cpp

HEADERS  += mainwindow.h \
    histogramwindow.h

FORMS    += mainwindow.ui \
    histogramwindow.ui

unix:{
CONFIG += link_pkgconfig
PKGCONFIG += opencv
}

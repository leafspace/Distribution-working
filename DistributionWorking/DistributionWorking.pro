#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T09:01:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DistributionWorking
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    member.cpp \

HEADERS  += mainwindow.h \
    member.h \

FORMS    += mainwindow.ui

RESOURCES += \
    mainresources.qrc

RC_FILE = titleicon.rc

CONFIG += qaxcontainer

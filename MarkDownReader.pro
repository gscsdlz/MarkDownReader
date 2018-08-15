#-------------------------------------------------
#
# Project created by QtCreator 2018-07-22T15:17:55
#
#-------------------------------------------------

QT       += core gui webenginewidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarkDownReader
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    recentfile.cpp \
    webview.cpp

HEADERS  += mainwindow.h \
    recentfile.h \
    webview.h

FORMS    += mainwindow.ui \
    recentfile.ui

RC_ICONS = res/edit.ico

RESOURCES += \
    res.qrc

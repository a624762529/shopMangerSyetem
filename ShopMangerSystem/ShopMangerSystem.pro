#-------------------------------------------------
#
# Project created by QtCreator 2020-03-10T13:43:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShopMangerSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    loadview.cpp \
    sockclient.cpp \
    chargeview.cpp \
    good.cpp \
    service.cpp \
    sendassis.cpp \
    logon.cpp \
    file.cpp \
    stylesheetcom.cpp

HEADERS += \
        mainwindow.h \
    loadview.h \
    sockclient.h \
    head.h \
    chargeview.h \
    good.h \
    service.h \
    sendassis.h \
    logon.h \
    file.h \
    stylesheetcom.h

FORMS += \
        mainwindow.ui \
    loadview.ui \
    chargeview.ui \
    logon.ui

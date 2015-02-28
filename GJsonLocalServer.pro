#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T10:09:29
#
#-------------------------------------------------

QT      += testlib

QT      -= gui

TARGET   = tst_gjsonlocalservertest
CONFIG  += console
CONFIG  -= app_bundle

TEMPLATE = app

DEFINES += SRCDIR=\\\"$$PWD/\\\"

SOURCES += \
    tst_gjsonlocalserver.cpp

include(gjsonlocalserver/gecholocalserver.pri)
include(gjsonlocalserver/gjsonaccessmanager.pri)
include(gjsonlocalserver/gjsonlocalserver.pri)

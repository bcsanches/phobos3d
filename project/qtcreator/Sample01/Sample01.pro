#-------------------------------------------------
#
# Project created by QtCreator 2011-09-01T11:27:19
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Sample01
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    ../../../src/sample_01/main.cpp

INCLUDEPATH += \
    ../../../include/kernel/

DEFINES += \
    PH_LINUX \
    PH_DEBUG \
    PH_CHECK_ASSERT \
    PH_GCC \
    PH_SDL

#-------------------------------------------------
#
# Project created by QtCreator 2011-06-22T13:46:57
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = epsilon-video-player
TEMPLATE = app

LIBS += -lavcodec -lavformat -lswscale

SOURCES += main.cpp\
        mainwindow.cpp \
    VideoLoader.cpp

HEADERS  += mainwindow.h \
    VideoLoader.h \
    VideoDrawer.h \
    VideoGL.h

FORMS    += mainwindow.ui

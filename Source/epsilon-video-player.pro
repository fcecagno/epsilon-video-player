# -------------------------------------------------
# Project created by QtCreator 2011-06-22T13:46:57
# -------------------------------------------------
QT += core \
    gui \
    opengl
TARGET = epsilon-video-player
TEMPLATE = app
INCLUDEPATH += ../Third/RXEngine/Inc
LIBS += -L../Third/RXEngine/Source -lRXEngine
LIBS += -lavcodec \
    -lavformat \
    -lswscale
SOURCES += main.cpp \
    mainwindow.cpp \
    VideoLoader.cpp \
    defines.cpp \
    Homography2.cpp \
    VideoGL.cpp
HEADERS += mainwindow.h \
    VideoLoader.h \
    VideoDrawer.h \
    VideoGL.h \
    Homography2.h \
    defines.h
FORMS += mainwindow.ui

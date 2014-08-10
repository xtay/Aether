#-------------------------------------------------
#
# Project created by QtCreator 2014-08-09T18:19:20
#
#-------------------------------------------------

QT       += core gui
QT       *= xml opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Aether
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    modelviewer.cpp

HEADERS  += mainwindow.h \
    modelviewer.h

FORMS    += mainwindow.ui

LIBS     += -lGLU -lQGLViewer

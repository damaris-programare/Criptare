#-------------------------------------------------
#
# Project created by QtCreator 2024-07-10T13:19:56
#
#-------------------------------------------------

QT       += core gui
QT       += printsupport
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = test_grila
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    criptare.cpp \
    dbtestgrila.cpp \
    logadmin.cpp \
    admin.cpp

HEADERS  += mainwindow.h \
    criptare.h \
    struct_questions.h \
    dbtestgrila.h \
    logadmin.h \
    admin.h

FORMS    += mainwindow.ui \
    logadmin.ui \
    admin.ui

#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T07:11:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MailBox
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    accountmanager.cpp \
    lib/pop3.cpp \
    lib/smtp.cpp \
    sendwindow.cpp \
    receive.cpp \
    email.cpp \
    lib/decodestrategy.cpp \
    lib/typestrategy.cpp \
    email.cpp \
    #accountsetting.cpp \
    showmessage.cpp \
    creataccount.cpp \
    newfolder.cpp

HEADERS  += mainwindow.h \
    accountmanager.h \
    initdb.h \
    lib/pop3.h \
    lib/smtp.h \
    sendwindow.h \
    receive.h \
    email.h \
    lib/decodestrategy.h \
    lib/typestrategy.h \
    email.h \
    #accountsetting.h \
    showmessage.h \
    creataccount.h \
    newfolder.h

FORMS    += mainwindow.ui \
    sendwindow.ui \
    #accountsetting.ui \
    creataccount.ui \
    newfolder.ui

RESOURCES += \
    icon.qrc
RC_FILE = icon.rc

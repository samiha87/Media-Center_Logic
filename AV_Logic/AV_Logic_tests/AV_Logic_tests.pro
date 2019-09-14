QT += testlib
QT += network
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_pjlinktester.cpp \
    ../pjlink.cpp \
    ../tcpsocket.cpp \
    pjlinktestserver.cpp

HEADERS += \
    ../pjlink.h \
    ../tcpsocket.h \
    pjlinkserver.h \
    pjlinktestserver.h

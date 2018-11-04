
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

CONFIG += c++11

SOURCES += \
    block.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    block.h \
    mainwindow.h \
    public.h

RESOURCES += \
    myqrc.qrc

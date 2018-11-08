
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia

CONFIG += c++11

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    framework_base.cpp \
    sutiation_judgement.cpp \
    board.cpp \
    block.cpp

HEADERS += \
    mainwindow.h \
    public.h \
    framework_base.h \
    number_block.h \
    board.h

RESOURCES += \
    myqrc.qrc

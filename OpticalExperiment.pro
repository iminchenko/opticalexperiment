#-------------------------------------------------
#
# Project created by QtCreator 2017-10-15T15:29:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++1z

TARGET = OpticalExperiment
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    ui/mainwindow.cpp \
    ui/constructareawidget.cpp \
    ui/shielditem.cpp \
    ui/opticalgratingitem.cpp \
    ui/vertexitem.cpp \
    ui/constructoritem.cpp \
    ui/connectionitem.cpp \
    instrumentconfig.cpp \
    ui/laseritem.cpp \
    ui/bloweritem.cpp \
    properties.cpp \
    propertyobserver.cpp \
    ui/lambdaitem.cpp \
    ui/polarizeritem.cpp \
    ui/phasemodulatoritem.cpp \
    deviceconfigs/device.cpp \
    deviceconfigs/devicelist.cpp \
    deviceconfigs/deviceviewconfig.cpp \
    deviceconfigs/deviceviewlist.cpp \
    ui/genericitem.cpp

HEADERS += \
    ui/mainwindow.h \
    ui/constructareawidget.h \
    ui/shielditem.h \
    ui/opticalgratingitem.h \
    ui/vertexitem.h \
    ui/constructoritem.h \
    ui/connectionitem.h \
    singleton.hpp \
    instrumentconfig.h \
    ui/laseritem.h \
    ui/bloweritem.h \
    properties.h \
    propertyobserver.h \
    ui/lambdaitem.h \
    ui/polarizeritem.h \
    ui/phasemodulatoritem.h \
    utility/singleton.hpp \
    deviceconfigs/device.h \
    deviceconfigs/devicelist.h \
    deviceconfigs/deviceviewconfig.h \
    deviceconfigs/deviceviewlist.h \
    globaldefines.h \
    ui/genericitem.h

FORMS += \
    ui/mainwindow.ui

RESOURCES += \
    recource.qrc

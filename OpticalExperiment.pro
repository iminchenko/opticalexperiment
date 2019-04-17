#-------------------------------------------------
#
# Project created by QtCreator 2017-10-15T15:29:56
#
#-------------------------------------------------

QT       += core gui
QT       += charts
QT += datavisualization

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
    command/command.cpp \
    command/commandhandler.cpp \
    command/commandhanlerglobal.cpp \
    deviceconfigs/commandhadlermodel.cpp \
    deviceconfigs/device.cpp \
    deviceconfigs/deviceconfig.cpp \
    deviceconfigs/deviceconfiglist.cpp \
    deviceconfigs/display.cpp \
    deviceconfigs/laser.cpp \
    deviceconfigs/ordinalfunction.cpp \
    deviceconfigs/wave.cpp \
    ui/commandhandlerview.cpp \
    ui/connectionitem.cpp \
    ui/constructareawidget.cpp \
    ui/constructoritem.cpp \
    ui/genericitem.cpp \
    ui/laseritem.cpp \
    ui/mainwindow.cpp \
    ui/shielditem.cpp \
    ui/vertexitem.cpp \
    utility/parser.cpp \
    devicemanager.cpp \
    instrumentconfig.cpp \
    main.cpp \
    properties.cpp \
    propertyobserver.cpp \
    utility/idgenerator.cpp \
    ui/commandhandlerchart.cpp \
    ui/parametersmanager.cpp \
    ui/chartitem.cpp \
    ui/chartview.cpp \
    ui/chartwidget.cpp \
    deviceconfigs/diffractiongrating.cpp \
    ui/diffractiongratingitem.cpp \
    ui/basechartitem.cpp \
    ui/diffractiogratingchartitem.cpp

HEADERS += \
    command/command.h \
    command/commandhandler.h \
    command/commandhanlerglobal.h \
    deviceconfigs/commandhadlermodel.h \
    deviceconfigs/device.h \
    deviceconfigs/deviceconfig.h \
    deviceconfigs/deviceconfiglist.h \
    deviceconfigs/devicedefines.h \
    deviceconfigs/display.h \
    deviceconfigs/laser.h \
    deviceconfigs/ordinalfunction.h \
    deviceconfigs/wave.h \
    ui/commandhandlerview.h \
    ui/connectionitem.h \
    ui/constructareawidget.h \
    ui/constructoritem.h \
    ui/genericitem.h \
    ui/laseritem.h \
    ui/mainwindow.h \
    ui/shielditem.h \
    ui/vertexitem.h \
    utility/parser.h \
    utility/singleton.hpp \
    devicemanager.h \
    globaldefines.h \
    instrumentconfig.h \
    matrix.h \
    properties.h \
    propertyobserver.h \
    utility/idgenerator.h \
    ui/commandhandlerchart.h \
    ui/parametersmanager.h \
    ui/chartitem.h \
    ui/chartview.h \
    ui/chartwidget.h \
    deviceconfigs/diffractiongrating.h \
    ui/diffractiongratingitem.h \
    ui/basechartitem.h \
    ui/diffractiogratingchartitem.h

FORMS += \
    ui/mainwindow.ui

RESOURCES += \
    recource.qrc

win32: LIBS += -lopengl32

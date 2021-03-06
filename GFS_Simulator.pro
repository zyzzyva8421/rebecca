#-------------------------------------------------
#
# Project created by QtCreator 2015-03-02T23:59:29
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GFS_Simulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    category.cpp \
    project.cpp \
    information.cpp \
    casting.cpp \
    mold.cpp \
    entrance.cpp \
    simulation.cpp \
    computing.cpp \
    output.cpp \
    material.cpp \
    treeviewcombobox.cpp \
    addmaterial.cpp \
    addstl.cpp \
    copymaterial.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    category.h \
    project.h \
    information.h \
    casting.h \
    mold.h \
    entrance.h \
    simulation.h \
    computing.h \
    output.h \
    material.h \
    treeviewcombobox.h \
    addmaterial.h \
    addstl.h \
    copymaterial.h \
    dialog.h

FORMS    += mainwindow.ui \
    addmaterial.ui \
    addstl.ui \
    copymaterial.ui \
    dialog.ui

RESOURCES += \
    GFS_Resource.qrc

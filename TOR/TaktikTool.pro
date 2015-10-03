#-------------------------------------------------
#
# Project created by QtCreator 2014-12-09T10:10:50
#
#-------------------------------------------------


QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svg printsupport

TARGET = TaktikTool
TEMPLATE = app

SOURCES += main.cpp \
    ui/mainwindow.cpp \
    ui/grouppropertiesdialog.cpp \
    ui/fieldsizedialog.cpp \
    items/fieldproperties.cpp \
    fieldscene.cpp \
    items/fielditem.cpp \
    items/field.cpp \
    items/arrowitem.cpp \
    items/groupitem.cpp \
    items/fielditembase.cpp \
    ui/numberdialog.cpp \
    items/fieldscenelistitem.cpp \
    items/fieldlistitem.cpp \
    items/preconditions.cpp


HEADERS  +=  ui/mainwindow.h \
    ui/grouppropertiesdialog.h \
    ui/fieldsizedialog.h \
    items/fieldproperties.h \
    fieldscene.h \
    items/fielditem.h \
    items/field.h \
    items/arrowitem.h \
    items/groupitem.h \
    items/fielditembase.h \
    items/preconditions.h \
    ui/numberdialog.h \
    items/fieldscenelistitem.h \
    items/fieldlistitem.h

FORMS    += ui/mainwindow.ui \
    ui/fieldsizedialog.ui \
    ui/grouppropertiesdialog.ui \
    ui/numberdialog.ui

RESOURCES += \
    gfx/gfx.qrc

OTHER_FILES +=
    gfx/appicon.ico

DISTFILES += \
    GraphicsItem-Types.txt

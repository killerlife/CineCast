#-------------------------------------------------
#
# Project created by QtCreator 2015-09-01T15:09:12
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LEONIS_CINEMA_CineCast
TEMPLATE = app


SOURCES += main.cpp\
        leoniscinemacinecast.cpp \
    statueform.cpp \
    setupform.cpp \
    maintainform.cpp \
    contentmanagementform.cpp \
    UIControl/maintaincontrol.cpp \
    UIControl/statuecontrol.cpp \
    UIControl/contentmanagementcontrol.cpp \
    UIControl/setupcontrol.cpp

HEADERS  += leoniscinemacinecast.h \
    statueform.h \
    setupform.h \
    maintainform.h \
    contentmanagementform.h \
    UIControl/maintaincontrol.h \
    UIControl/statuecontrol.h \
    UIControl/contentmanagementcontrol.h \
    UIControl/setupcontrol.h

FORMS    += leoniscinemacinecast.ui \
    statueform.ui \
    setupform.ui \
    maintainform.ui \
    contentmanagementform.ui

RESOURCES += \
    Image.qrc

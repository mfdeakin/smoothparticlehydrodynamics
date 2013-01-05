#-------------------------------------------------
#
# Project created by QtCreator 2012-12-16T02:45:11
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = smoothparticlehydrodynamics
TEMPLATE = app

QMAKE_CFLAGS += -std=c99

SOURCES += main.cpp\
        sphwindow.cpp \
    sphrender.cpp \
    simulator.cpp \
    matrix.c

HEADERS  += sphwindow.h \
    sphrender.h \
    simulator.h \
    matrix.h

FORMS    += sphwindow.ui

OTHER_FILES += \
    README.txt \
    lgpl-3.0.txt \
    gpl-3.0.txt

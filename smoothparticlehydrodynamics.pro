#-------------------------------------------------
#
# Project created by QtCreator 2012-12-16T02:45:11
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = smoothparticlehydrodynamics
TEMPLATE = app


SOURCES += main.cpp\
        sphwindow.cpp \
    sphrender.cpp \
    simulator.cpp

HEADERS  += sphwindow.h \
    sphrender.h \
    simulator.h

FORMS    += sphwindow.ui

OTHER_FILES += \
    README.txt \
    lgpl-3.0.txt \
    gpl-3.0.txt

TARGET = apt-colorm
INCLUDEPATH += ../image
INCLUDEPATH += ../aptcode
LIBS += -L../image -limage
LIBS += -L../aptcode -laptcode

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        apt-colorm.c


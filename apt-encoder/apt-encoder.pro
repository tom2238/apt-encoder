TARGET = apt-encoder
INCLUDEPATH += ../image
INCLUDEPATH += ../aptcode
LIBS += -L../image -limage
LIBS += -L../aptcode -laptcode
LIBS += -lpthread -lm -lreadline -ltermcap

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.c \      
        audioset.c \
        wavwrite.c

HEADERS += \
    main.h \
    audioset.h \
    wavwrite.h

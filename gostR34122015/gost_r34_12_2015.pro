TEMPLATE = app
CONFIG += console c11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    generating_tables.c \
    r34122015_core.c \
    gost_2015.c

HEADERS += \
    generating_tables.h \
    r34122015_types.h \
    r34122015_core.h \
    gost_2015.h

DISTFILES +=

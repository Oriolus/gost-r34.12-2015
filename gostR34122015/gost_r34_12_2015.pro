TEMPLATE = app
CONFIG += console c11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    r34122015_64.c \
    r34122015_128.c \
    generating_tables.c

HEADERS += \
    r34122015.h \
    generating_tables.h

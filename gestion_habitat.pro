TEMPLATE = app
TARGET = gestion_habitat

QT += core gui widgets sql printsupport

CONFIG -= console

SOURCES += \
    main.cpp \
    consommation.cpp \
    mainwindow.cpp \
    connexion.cpp \
    habitat.cpp

HEADERS += \
    mainwindow.h \
    connexion.h \
    consommation.h \
    habitat.h

FORMS += \
    mainwindow.ui

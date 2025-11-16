# ----------------------------
# Projet Qt pour gestion_habitat
# ----------------------------
TEMPLATE = app
TARGET = gestion_habitat

# Active les modules Qt nécessaires
QT += core gui widgets sql

# Supprime la console (évite WinMain error)
CONFIG -= console

# Chemins d'inclusion
INCLUDEPATH += C:/instantclient-basic-windows.x64-12.2.0.1.0/instantclient_12_2

# Sources, headers et forms
SOURCES += main.cpp \
           consommation.cpp \
           mainwindow.cpp \
           connexion.cpp \
           habitat.cpp

HEADERS += mainwindow.h \
           connexion.h \
           consommation.h \
           habitat.h

FORMS += mainwindow.ui

# Librairies Oracle Instant Client (64-bit)
LIBS += -LC:/instantclient-basic-windows.x64-12.2.0.1.0/instantclient_12_2 -loci

# Définitions Windows
win32 {
    DEFINES += UNICODE
}
QT += printsupport

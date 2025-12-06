QT += core gui serialport sql network charts printsupport
QT += core gui sql charts network serialport



# AJOUTE CES LIGNES :
CONFIG += c++11
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    connection.cpp \
    consommationeau.cpp \
    graph.cpp \
    habitantcrud.cpp \
    habitat.cpp \
    main.cpp \
    mainwindow.cpp \
    personnel.cpp \
    poubelle.cpp \
    vehiculecrud.cpp \
    demandecrud.cpp \
    satisfaction.cpp \
    zone.cpp \
    affectationp.cpp

HEADERS += \
    arduino.h \
    connection.h \
    consommationeau.h \
    graph.h \
    habitantcrud.h \
    habitat.h \
    mainwindow.h \
    personnel.h \
    poubelle.h \
    vehiculecrud.h \
    demandecrud.h \
    satisfaction.h \
    zone.h \
    affectationp.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

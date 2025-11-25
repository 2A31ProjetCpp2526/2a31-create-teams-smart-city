QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connection.cpp \
    consomation.cpp \
    demandecrud.cpp \
    habitantcrud.cpp \
    main.cpp \
    maintenance.cpp \
    mainwindow.cpp \
    personnel.cpp \
    poubelle.cpp \
    satisfaction.cpp \
    vehiculecrud.cpp \
    zone.cpp

HEADERS += \
    connection.h \
    consomation.h \
    demandecrud.h \
    habitantcrud.h \
    maintenance.h \
    mainwindow.h \
    personnel.h \
    poubelle.h \
    satisfaction.h \
    vehiculecrud.h \
    zone.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

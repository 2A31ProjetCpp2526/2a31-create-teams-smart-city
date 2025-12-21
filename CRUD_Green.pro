QT += core gui widgets sql
QT       +=  charts
CONFIG -= debug
DEFINES += QT_NO_DEBUG_OUTPUT

QT += serialport
QT += core
QT += core gui widgets network


QT += core5compat  # For Qt 6 compatibility
RESOURCES += resources.qrc





greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ChartDialog.cpp \
    DemandeDialog.cpp \
    SortingGameWidget.cpp \
    SportsChallengeDialog.cpp \
    arduino.cpp \
    calendrierzones.cpp \
    connection.cpp \
    main.cpp \
    maintenance.cpp \
    mainwindow.cpp \
    poubelles.cpp \
    rainsensor.cpp \
    smartmetal.cpp \
    sortdialog.cpp \
    vehiculecrud.cpp \
    zone.cpp

HEADERS += \
    ChartDialog.h \
    DemandeDialog.h \
    SortingGameWidget.h \
    SportsChallengeDialog.h \
    arduino.h \
    calendrierzones.h \
    connection.h \
    maintenance.h \
    mainwindow.h \
    poubelles.h \
    rainsensor.h \
    smartmetal.h \
    sortdialog.h \
    vehiculecrud.h \
    zone.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT_FOR_CONFIG += charts

DISTFILES +=

RESOURCES += \
    resources.qrc



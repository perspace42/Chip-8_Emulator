QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ApplicationLoop.cpp \
    Chip8.cpp \
    bindkeys.cpp \
    keybinds.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ApplicationLoop.h \
    Chip8.h \
    bindkeys.h \
    keybinds.h \
    mainwindow.h

FORMS += \
    bindkeys.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    ../../Pictures/Icons/Pause.PNG \
    ../../Pictures/Icons/Play.PNG \
    Icons/ColorPicker.png \
    Icons/Pause.PNG \
    Icons/Play.PNG

RESOURCES += \
    IconsResource.qrc

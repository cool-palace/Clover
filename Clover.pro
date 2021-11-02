QT       += core gui
QT  += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bullet.cpp \
    button.cpp \
    clovergame.cpp \
    dialogbox.cpp \
    drinkgame.cpp \
    game.cpp \
    main.cpp \
    npc.cpp \
    player.cpp \
    speechline.cpp

HEADERS += \
    bullet.h \
    button.h \
    clovergame.h \
    dialogbox.h \
    drinkgame.h \
    game.h \
    npc.h \
    player.h \
    speechline.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

QT_REQ_MAJOR_VERSION = 5
QT_REQ_MINOR_VERSION = 3
QT_REQ_VERSION = "$$QT_REQ_MAJOR_VERSION"."$$QT_REQ_MINOR_VERSION"

lessThan(QT_MAJOR_VERSION, $$QT_REQ_MAJOR_VERSION) {
    error(Minimum supported Qt version is $$QT_REQ_VERSION!)
}
equals(QT_MAJOR_VERSION, $$QT_REQ_MAJOR_VERSION):lessThan(QT_MINOR_VERSION, $$QT_REQ_MINOR_VERSION) {
    error(Minimum supported Qt version is $$QT_REQ_VERSION!)
}


QT       += core gui widgets
TARGET   = leelagtp
CONFIG   += c++14
RC_FILE  += LeelaGTP/leelagtp.rc

TEMPLATE = app

DEFINES += LEELA_GTP

SOURCES += main.cpp \
    Game.cpp \
    Worker.cpp \
    Order.cpp \
    Job.cpp \
    Management.cpp \
    LeelaGTP/LeelaGTP.cpp \
    LeelaGTP/MovLancifolium.cpp \
    LeelaGTP/GTPConfig.cpp

HEADERS += \
    Game.h \
    Worker.h \
    Job.h \
    Order.h \
    Result.h \
    Management.h \
    LeelaGTP/LeelaGTP.h \
    LeelaGTP/MovLancifolium.h \
    LeelaGTP/GTPConfig.h

DISTFILES += \
    LeelaGTP/leelagtp.rc

RESOURCES += \
    LeelaGTP/leelagtp.qrc

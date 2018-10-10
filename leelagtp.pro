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
RC_FILE  += leelagtp.rc

TEMPLATE = app

SOURCES += Game.cpp \
    Worker.cpp \
    Order.cpp \
    Job.cpp \
    Management.cpp \
    LeelaGTP.cpp \
    MovLancifolium.cpp \
    GTPConfig.cpp

HEADERS += \
    Game.h \
    Worker.h \
    Job.h \
    Order.h \
    Result.h \
    Management.h \
    LeelaGTP.h \
    MovLancifolium.h \
    GTPConfig.h

DISTFILES += \
    leelagtp.rc

RESOURCES += \
    leelagtp.qrc

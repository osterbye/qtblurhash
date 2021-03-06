VERSION = 1.0.0
TEMPLATE = lib
DEFINES += QTBLURHASH_LIBRARY

CONFIG += c++11
QMAKE_CXXFLAGS += -Wall -Wextra
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    decoder.cpp \
    encoder.cpp \
    qtblurhash.cpp

HEADERS += \
    common.h \
    decoder.h \
    encoder.h \
    qtblurhash.h \
    qtblurhash_global.h

DESTDIR = $$_PRO_FILE_PWD_/../bin/
OBJECTS_DIR = $$_PRO_FILE_PWD_/../build/.obj
MOC_DIR = $$_PRO_FILE_PWD_/../build/.moc
RCC_DIR = $$_PRO_FILE_PWD_/../build/.qrc
UI_DIR = $$_PRO_FILE_PWD_/../build/.ui

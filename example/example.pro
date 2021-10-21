TARGET = test_app

CONFIG += c++11 console
CONFIG -= app_bundle

INCLUDEPATH += $$_PRO_FILE_PWD_/../qtblurhash
LIBS += -L$$_PRO_FILE_PWD_/../bin
LIBS += -lqtblurhash

SOURCES += \
        main.cpp

DESTDIR = $$_PRO_FILE_PWD_/../bin/

TEMPLATE = subdirs

SUBDIRS = \
    example \
    qtblurhash

example.file = example/example.pro
qtblurhash.file = qtblurhash/qtblurhash.pro

example.depends = qtblurhash

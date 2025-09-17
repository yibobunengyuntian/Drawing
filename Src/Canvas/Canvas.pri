INCLUDEPATH += $$PWD

include($$PWD/ShapeItem/ShapeItem.pri)

HEADERS += \
    $$PWD/canvas.h \
    $$PWD/textmenu.h \
    $$PWD/utils.h

SOURCES += \
    $$PWD/canvas.cpp \
    $$PWD/textmenu.cpp \
    $$PWD/utils.cpp

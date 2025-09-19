INCLUDEPATH += $$PWD

include($$PWD/Items/Items.pri)

HEADERS += \
    $$PWD/canvas.h \
    $$PWD/canvascommand.h \
    $$PWD/textmenu.h \
    $$PWD/utils.h

SOURCES += \
    $$PWD/canvas.cpp \
    $$PWD/canvascommand.cpp \
    $$PWD/textmenu.cpp \
    $$PWD/utils.cpp

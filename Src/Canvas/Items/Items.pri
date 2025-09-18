INCLUDEPATH += $$PWD

include($$PWD/ShapeItem/ShapeItem.pri)
include($$PWD/ToolItem/ToolItem.pri)

HEADERS += \
    $$PWD/baseitem.h \
    $$PWD/rectopitem.h

SOURCES += \
    $$PWD/baseitem.cpp \
    $$PWD/rectopitem.cpp

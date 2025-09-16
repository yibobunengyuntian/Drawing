INCLUDEPATH += $$PWD

FORMS += \
    $$PWD/mainwin.ui

HEADERS += \
    $$PWD/drawingwgt.h \
    $$PWD/mainwin.h

SOURCES += \
    $$PWD/drawingwgt.cpp \
    $$PWD/mainwin.cpp


include($$PWD/Canvas/Canvas.pri)
include($$PWD/FlowLayout/FlowLayout.pri)
include($$PWD/ColorButton/ColorButton.pri)

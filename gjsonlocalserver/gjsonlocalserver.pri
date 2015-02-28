QT      *= core-private dbus

CONFIG  *= no_private_qt_headers_warning

INCLUDEPATH *= $$PWD

HEADERS += \
    $$PWD/gjsonstandardstatus.h \
    $$PWD/gjsonlocalserver.h \
    $$PWD/gjsonlocalserver_p.h \
    $$PWD/gjsonlocalserverdatas.h

SOURCES *= \
    $$PWD/gjsonlocalserver.cpp \
    $$PWD/gjsonlocalserverdatas.cpp


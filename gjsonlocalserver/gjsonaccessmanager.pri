QT      *= core-private dbus

CONFIG  *= no_private_qt_headers_warning

INCLUDEPATH *= $$PWD

HEADERS += \
    $$PWD/gjsonstandardstatus.h \
    $$PWD/gjsonaccessmanager.h \
    $$PWD/gjsonaccessmanager_p.h \
    $$PWD/gjsonaccessmanagerdatas.h

SOURCES *= \
    $$PWD/gjsonaccessmanager.cpp \
    $$PWD/gjsonaccessmanagerdatas.cpp

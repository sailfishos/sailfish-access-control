MODULENAME = Sailfish/AccessControl
DEFINES *= MODULENAME=\"\\\"\"$${MODULENAME}\"\\\"\"
TEMPLATE = lib
TARGET  = sailfishaccesscontrolplugin
TARGET = $$qtLibraryTarget($$TARGET)
TARGETPATH = $$[QT_INSTALL_QML]/$$MODULENAME

QT -= gui
QT += qml
CONFIG += \
    plugin \
    link_pkgconfig \
    hide_symbols \
    c++14

PKGCONFIG += sailfishaccesscontrol

HEADERS += accesscontrol.h

SOURCES += plugin.cpp \
            accesscontrol.cpp

import.files = *.qml *.js qmldir plugins.qmltypes
import.path = $$TARGETPATH
target.path = $$TARGETPATH
INSTALLS += target import

QMAKE_CXXFLAGS += -Wall
OTHER_FILES += qmldir *.qml *.js

QT += widgets network

TEMPLATE = lib
DEFINES += TCPSEARCHPLUGIN_LIBRARY
TARGET = $$qtLibraryTarget(tcp-search-plugin)
CONFIG += c++11 plugin lrelease link_pkgconfig
PKGCONFIG += gsettings-qt ukui-search

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
TRANSLATIONS += \
    translations/zh_CN.ts

qm_files.path = /usr/share/ukui-search-plugins/tcp-search-plugin/translations/
qm_files.files = $$OUT_PWD/.qm/*.qm
INSTALLS += qm_files

SOURCES += \
    tcp-search-plugin.cpp

HEADERS += \
    tcp-search-plugin_global.h \
    tcp-search-plugin.h

# Default rules for deployment.

target.path = $$[QT_INSTALL_LIBS]/ukui-search-plugins
INSTALLS += target

DISTFILES += \
    common.json

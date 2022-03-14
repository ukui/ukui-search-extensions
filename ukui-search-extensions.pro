TEMPLATE = subdirs

VERSION = 1.1.0
DEFINES += VERSION='\\"$${VERSION}\\"'

SUBDIRS += \
    ukui-search-music-plugin/musicPlugin.pro \
    tcp-search-plugin/tcp-search-plugin.pro

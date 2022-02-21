TEMPLATE = subdirs

VERSION = 0.1.0
DEFINES += VERSION='\\"$${VERSION}\\"'

SUBDIRS += \
    ukui-search-music-plugin/musicPlugin.pro \
    tcp-search-plugin/tcp-search-plugin.pro

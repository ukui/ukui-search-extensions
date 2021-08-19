/*
 * Author: maoyiming<maoym99@outlook.com>
 */
#ifndef MUSICPLUGIN_GLOBAL_H
#define MUSICPLUGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MUSICPLUGIN_LIBRARY)
#  define MUSICPLUGIN_EXPORT Q_DECL_EXPORT
#else
#  define MUSICPLUGIN_EXPORT Q_DECL_IMPORT
#endif

#endif // MUSICPLUGIN_GLOBAL_H

# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = CuteXeytanClient
DESTDIR = ./debug
QT += core network gui widgets
CONFIG += debug
DEFINES += _WINDOWS _UNICODE _ENABLE_EXTENDED_ALIGNED_STORAGE WIN64 QT_DEPRECATED_WARNINGS QT_WIDGETS_LIB QT_NETWORK_LIB
INCLUDEPATH += ./GeneratedFiles/$(ConfigurationName) \
    ./GeneratedFiles \
    . \
    ./debug \
    $(QTDIR)/mkspecs/win32-msvc
DEPENDPATH += .
MOC_DIR += $(Configuration)
OBJECTS_DIR += debug
UI_DIR += .
RCC_DIR += ./GeneratedFiles
include(CuteXeytanClient.pri)
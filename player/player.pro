# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

TEMPLATE = app
TARGET = player
DESTDIR = ./debug
QT += core xml network gui multimedia widgets multimediawidgets
CONFIG += debug
DEFINES += _WINDOWS _UNICODE WIN64 QT_MULTIMEDIAWIDGETS_LIB QT_MULTIMEDIA_LIB QT_WIDGETS_LIB QT_NETWORK_LIB QT_XML_LIB
INCLUDEPATH += ../../../../../../../cryptopp565 \
    . \
    ./debug \
    $(QTDIR)/mkspecs/win32-msvc
LIBS += -L"../../../../../../../cryptopp565" \
    -L"../../../../../../../utils/my_sql/my_sql/lib" \
    -L"../../../../../../../utils/postgresql/pgsql/lib" \
    -lshell32
DEPENDPATH += .
MOC_DIR += $(ConfigurationName)
OBJECTS_DIR += debug
UI_DIR += ./GeneratedFiles
RCC_DIR += ./GeneratedFiles
include(player.pri)

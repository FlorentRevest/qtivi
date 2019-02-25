TARGET = media_mopidy
QT_FOR_CONFIG += ivimedia-private

PLUGIN_TYPE = qtivi
PLUGIN_EXTENDS = qtivi
PLUGIN_CLASS_NAME = QIviServiceInterface

QT       += core ivicore ivimedia websockets

load(qt_plugin)

DISTFILES += media_mopidy.json

HEADERS += \
    mediaplugin.h \
    mediaplayerbackend.h \
    searchandbrowsebackend.h \
    mediadiscoverybackend.h \
    mediaindexerbackend.h \
    mopidy/basecontroller_p.h \
    mopidy/modeltranslator.h \
    mopidy/eventhandler_p.h \
    mopidy/mopidyclient.h \
    mopidy/mopidyclient_p.h \
    mopidy/jsonrpchandler_p.h \
    mopidy/jsonrpchandler.h \
    mopidy/basecontroller.h \
    mopidy/playbackcontroller.h \
    mopidy/playlistscontroller.h \
    mopidy/librarycontroller.h \
    mopidy/tracklistcontroller.h \
    mopidy/eventhandler.h \
    mopidy/models.h

SOURCES += \
    mediaplugin.cpp \
    mediaplayerbackend.cpp \
    searchandbrowsebackend.cpp \
    mediadiscoverybackend.cpp \
    mediaindexerbackend.cpp \
    mopidy/basecontroller.cpp \
    mopidy/modeltranslator.cpp \
    mopidy/eventhandler.cpp \
    mopidy/mopidyclient.cpp \
    mopidy/jsonrpchandler.cpp \
    mopidy/playbackcontroller.cpp \
    mopidy/playlistscontroller.cpp \
    mopidy/librarycontroller.cpp \
    mopidy/tracklistcontroller.cpp

TEMPLATE = subdirs

QT_FOR_CONFIG += ivimedia-private

qtConfig(tuner_simulation_backend): SUBDIRS += tuner_simulator
qtConfig(media_simulation_backend): SUBDIRS += media_simulator
qtConfig(media_mopidy_backend):     SUBDIRS += media_mopidy
qtConfig(mpris_backend):qtHaveModule(dbus): SUBDIRS += media_simulator_mpris

OTHER_FILES += \
    $$PWD/doc/*.qdocconf \
    $$PWD/doc/src/*.qdoc

##################################################################################################
# Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
#
# This software, including documentation, is protected by copyright controlled by Pelagicore AG.
# All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
# all of this material requires prior written consent of Pelagicore AG Corporation. This material
# also contains confidential information which may not be disclosed to others without the prior
# written consent of Pelagicore AG.
##################################################################################################

QT       += testlib QtIVICore

TARGET = tst_servicemanagertest
CONFIG   += testcase

TEMPLATE = app

SOURCES += \
    tst_servicemanagertest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

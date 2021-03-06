{#
# Copyright (C) 2018 Pelagicore AG.
# Contact: https://www.qt.io/licensing/
#
# This file is part of the QtIvi module of the Qt Toolkit.
#
# $QT_BEGIN_LICENSE:LGPL-QTAS$
# Commercial License Usage
# Licensees holding valid commercial Qt Automotive Suite licenses may use
# this file in accordance with the commercial license agreement provided
# with the Software or, alternatively, in accordance with the terms
# contained in a written agreement between you and The Qt Company.  For
# licensing terms and conditions see https://www.qt.io/terms-conditions.
# For further information use the contact form at https://www.qt.io/contact-us.
#
# GNU Lesser General Public License Usage
# Alternatively, this file may be used under the terms of the GNU Lesser
# General Public License version 3 as published by the Free Software
# Foundation and appearing in the file LICENSE.LGPL3 included in the
# packaging of this file. Please review the following information to
# ensure the GNU Lesser General Public License version 3 requirements
# will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 2.0 or (at your option) the GNU General
# Public license version 3 or any later version approved by the KDE Free
# Qt Foundation. The licenses are as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file. Please review the following
# information to ensure the GNU General Public License requirements will
# be met: https://www.gnu.org/licenses/gpl-2.0.html and
# https://www.gnu.org/licenses/gpl-3.0.html.
#
# $QT_END_LICENSE$
#
# SPDX-License-Identifier: LGPL-3.0
#}
{% import 'qtivi_macros.j2' as ivi %}
{% set class = '{0}Module'.format(module.module_name) %}
{% include 'generated_comment.cpp.tpl' %}

#include "{{class|lower}}.h"
{% for interface in module.interfaces %}
#include "{{interface|lower}}.h"
{% endfor %}
#include <QQmlEngine>
#include <QDebug>
#include <QDataStream>
#include <QtSimulator>

QT_BEGIN_NAMESPACE

/*! \internal */
QObject* {{class|lower}}_singletontype_provider(QQmlEngine*, QJSEngine*)
{
    return nullptr; //new {{class}}Factory();
}

/*!
    \class {{class}}
    \inmodule {{module}}

{{ ivi.format_comments(module.comment) }}
*/
{{class}}::{{class}}(QObject *parent)
    : QObject(parent)
{
}

{% for enum in module.enums %}
{{class}}::{{enum}} {{class}}::to{{enum}}(quint8 v, bool *ok) {
    if (ok) {
        *ok = true;
    }
    switch (v) {
{% for member in enum.members %}
    case {{member.value}}: return {{member.name}};
{% endfor %}
    default:
        if (ok) {
            *ok = false;
        }
        return {{enum.members|first}};
    }
}
{% endfor %}

/*! \internal */
void {{class}}::registerTypes()
{
{% for enum in module.enums %}
    qRegisterMetaType<{{class}}::{{enum|flag_type}}>();
    qRegisterMetaTypeStreamOperators<{{class}}::{{enum|flag_type}}>();
{% endfor %}
{% for struct in module.structs %}
    qRegisterMetaType<{{struct}}>();
{% endfor %}
}

/*! \internal */
void {{class}}::registerQmlTypes(const QString& uri, int majorVersion, int minorVersion)
{
    qmlRegisterSingletonType<{{class}}>(uri.toLatin1(), majorVersion, minorVersion,
                                        "{{module.module_name}}Module",
                                        {{class|lower}}_singletontype_provider);
{% for interface in module.interfaces %}
    {{interface}}::registerQmlTypes(uri, majorVersion, minorVersion);
{% endfor %}
}

QSimulatorServer *{{class}}::simulationServer()
{
    static QSimulatorServer *server = nullptr;

    if (!server) {
        server = new QSimulatorServer;
        QString error;
        server->startServer(0xbeef+3, &error);
        if (!error.isEmpty())
            qWarning("ERROR: %s", qPrintable(error));
    }

    return server;
}

{% for enum in module.enums %}
QDataStream &operator<<(QDataStream &out, {{class}}::{{enum|flag_type}} var)
{
    out << (quint8)var;
    return out;
}
QDataStream &operator>>(QDataStream &in, {{class}}::{{enum|flag_type}} &var)
{
    bool ok;
    quint8 val;
    in >> val;
    var = {{class}}::to{{enum}}(val, &ok);
    if (!ok) {
        qWarning() << "Received an invalid enum value for type {{class}}::{{enum|flag_type}}, value =" << var;
    }
    return in;
}
{% endfor %}

QT_END_NAMESPACE

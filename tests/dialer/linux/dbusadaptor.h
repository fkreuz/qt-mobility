/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DBUSADAPTOR_H_1275967717
#define DBUSADAPTOR_H_1275967717

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "message.h"
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface org.freedesktop.Telepathy.Connection.Interface.Requests
 */
class TelepathyAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Telepathy.Connection.Interface.Requests")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"org.freedesktop.Telepathy.Connection.Interface.Requests\" >\n"
"    <signal name=\"NewChannels\" >\n"
"      <arg direction=\"out\" type=\"a(i)\" name=\"channelsarray\" />\n"
"      <annotation value=\"ChannelsArray\" name=\"com.trolltech.QtDBus.QtTypeName.In0\" />\n"
"    </signal>\n"
"    <signal name=\"StatusChanged\">\n"
"     <arg name=\"status\" type=\"s\" direction=\"out\"/>\n"
"    </signal>\n"
"    <method name=\"createNewChannels\" >\n"
"      <arg direction=\"in\" type=\"a(i)\" name=\"channelsarray\" />\n"
"      <annotation value=\"ChannelsArray\" name=\"com.trolltech.QtDBus.QtTypeName.In0\" />\n"
"    </method>\n"
"    <method name=\"emitStatusChanged\" >\n"
"      <arg direction=\"in\" type=\"s\" name=\"status\" />\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    TelepathyAdaptor(QObject *parent);
    virtual ~TelepathyAdaptor();

public Q_SLOTS: // METHODS
    void createNewChannels(const ChannelsArray& channelsarray);
    void emitStatusChanged(const QString& status);
   
Q_SIGNALS: // SIGNALS
    void NewChannels(const ChannelsArray& channelsarray);
    void StatusChanged(const QString& status);
};

#endif //DBUSADAPTOR_H_1275967717
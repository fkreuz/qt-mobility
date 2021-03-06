/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef RFCOMMCLIENT_H
#define RFCOMMCLIENT_H

#include <qbluetoothserviceinfo.h>
#include <qbluetoothsocket.h>

#include <QtCore/QObject>
#include <QtCore/QTime>
#include <QtCore/QTimer>
#include <QBluetoothLocalDevice.h>
#include <QThread>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c9");

QTM_BEGIN_NAMESPACE
class QBluetoothSocket;

QTM_END_NAMESPACE

class QDataStream;

QTM_USE_NAMESPACE

class MyThread : public QThread
{
public:
    void sleep(int seconds);
};


 
//! [declaration]
class RfCommClient : public QObject
{
    Q_OBJECT

    enum States {
        listening, 
        pendingConnections,
        dataTransfer
    };
public:
    explicit RfCommClient(QObject *parent = 0);
    ~RfCommClient();

    void startClient(const QBluetoothServiceInfo &remoteService);
    void stopClient();

public slots:        
    void error(QBluetoothSocket::SocketError);

signals:
    void connected(const QString &name);
    void disconnected();
    void done();
    void lag(int ms);

private slots:
    void readSocket();
    void connected();
    void sendText();
    void socketDisconnected();
    void hostModeStateChanged(QBluetoothLocalDevice::HostMode);
    
private:
    bool powerOn();

private:
    QBluetoothSocket *socket;
    QDataStream *stream;
    QByteArray buffer;
    QBluetoothServiceInfo serviceInfo;
    QBluetoothLocalDevice localDevice;
    QTime *elapsed;
    QTimer lagTimer;
    int lagTimeout;
    RfCommClient::States state;
};
//! [declaration]

#endif // RFCOMMCLIENT_H

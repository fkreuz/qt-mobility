/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGEOTILEDMAPREPLY_H
#define QGEOTILEDMAPREPLY_H

#include "qmobilityglobal.h"

#include <QObject>

QTM_BEGIN_NAMESPACE

class QGeoTiledMapRequest;
class QGeoTiledMapReplyPrivate;

class Q_LOCATION_EXPORT QGeoTiledMapReply : public QObject
{
    Q_OBJECT

public:
    enum Error {
        NoError,
        CommunicationError,
        ParseError,
        UnknownError
    };

    QGeoTiledMapReply(const QGeoTiledMapRequest &request, QObject *parent = 0);
    QGeoTiledMapReply(Error error, const QString &errorString, QObject *parent = 0);
    virtual ~QGeoTiledMapReply();

    bool isFinished() const;
    Error error() const;
    QString errorString() const;

    bool isCached() const;

    QGeoTiledMapRequest request() const;

    QByteArray mapImageData() const;
    QString mapImageFormat() const;

    virtual void abort();

Q_SIGNALS:
    void finished();
    void error(QGeoTiledMapReply::Error error, const QString &errorString = QString());

protected:
    void setError(Error error, const QString &errorString);
    void setFinished(bool finished);

    void setCached(bool cached);

    void setMapImageData(const QByteArray &data);
    void setMapImageFormat(const QString &format);

private:
    QGeoTiledMapReplyPrivate *d_ptr;
    Q_DISABLE_COPY(QGeoTiledMapReply)
};

QTM_END_NAMESPACE

#endif

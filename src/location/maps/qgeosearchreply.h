/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGEOSEARCHREPLY_H
#define QGEOSEARCHREPLY_H

#include "qgeoplace.h"

#include <QObject>
#include <QList>

QTM_BEGIN_NAMESPACE

class QGeoSearchReplyPrivate;

class Q_LOCATION_EXPORT QGeoSearchReply : public QObject
{
    Q_OBJECT

public:
    enum Error {
        NoError,
        EngineNotSetError,
        CommunicationError,
        ParseError,
        UnsupportedOptionError,
        UnknownError
    };

    QGeoSearchReply(Error error, const QString &errorString, QObject *parent = 0);
    virtual ~QGeoSearchReply();

    bool isFinished() const;
    Error error() const;
    QString errorString() const;

    QGeoBoundingBox bounds() const;
    QList<QGeoPlace> places() const;

public slots:
    virtual void abort();

signals:
    void finished();
    void error(QGeoSearchReply::Error error, const QString &errorString = QString());

protected:
    QGeoSearchReply(QObject *parent = 0);

    void setError(Error error, const QString &errorString);
    void setFinished(bool finished);

    void setBounds(const QGeoBoundingBox& bounds);
    void addPlace(const QGeoPlace &place);
    void setPlaces(const QList<QGeoPlace> &places);

private:
    QGeoSearchReplyPrivate *d_ptr;
    Q_DISABLE_COPY(QGeoSearchReply)
};

QTM_END_NAMESPACE

#endif
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

#include "qnearfieldtagtype4.h"

QTM_BEGIN_NAMESPACE

/*!
    \class QNearFieldTagType4
    \brief The QNearFieldTagType4 class provides an interface for communicating with an NFC Tag
           Type 4 tag.

    \ingroup connectivity-nfc
    \inmodule QtConnectivity
*/

/*!
    \fn Type QNearFieldTagType4::type() const
    \reimp
*/

/*!
    Constructs a new tag type 4 near field target with \a parent.
*/
QNearFieldTagType4::QNearFieldTagType4(QObject *parent)
:   QNearFieldTarget(parent)
{
}

/*!
    Destroys the tag type 4 near field target.
*/
QNearFieldTagType4::~QNearFieldTagType4()
{
}

/*!
    Returns the NFC Tag Type 4 specification version number that the tag supports.
*/
quint8 QNearFieldTagType4::version()
{
    return 0;
}

/*!
    Requests that the file specified by \a name be selected. Upon success calls to read() and
    write() will act on the selected file. Returns a request id which can be used to track the
    completion status of the request.

    Once the request completes the response can be retrieved from the requestResponse() function.
    The response of this request will be a boolean value, true for success; otherwise false.

    \sa requestCompleted(), waitForRequestCompleted()
*/
QNearFieldTarget::RequestId QNearFieldTagType4::select(const QByteArray &name)
{
    Q_UNUSED(name);

    return RequestId();
}

/*!
    Requests that the file specified by \a fileIdentifier be selected. Upon success calls to read()
    and write() will act on the selected file. Returns a request id which can be used to track the
    completion status of the request.

    Once the request completes the response can be retrieved from the requestResponse() function.
    The response of this request will be a boolean value, true for success; otherwise false.

    \sa requestCompleted(), waitForRequestCompleted()
*/
QNearFieldTarget::RequestId QNearFieldTagType4::select(quint16 fileIdentifier)
{
    Q_UNUSED(fileIdentifier);

    return RequestId();
}

/*!
    Requests that \a length bytes be read from the currently selected file starting from
    \a startOffset. If \a length is 0 all data or the maximum read size bytes will be read,
    whichever is smaller. Returns a request id which can be used to track the completion status of
    the request.

    Once the request completes successfully the response can be retrieved from the
    requestResponse() function. The response of this request will be a QByteArray.

    \sa requestCompleted(), waitForRequestCompleted()
*/
QNearFieldTarget::RequestId QNearFieldTagType4::read(quint16 length, quint16 startOffset)
{
    Q_UNUSED(length);
    Q_UNUSED(startOffset);

    return RequestId();
}

/*!
    Writes \a data to the currently selected file starting at \a startOffset. Returns a request id
    which can be used to track the completion status of the request.

    Once the request completes the response can be retrieved from the requestResponse() function.
    The response of this request will be a boolean value, true for success; otherwise false.

    \sa requestCompleted(), waitForRequestCompleted()
*/
QNearFieldTarget::RequestId QNearFieldTagType4::write(const QByteArray &data, quint16 startOffset)
{
    Q_UNUSED(data);
    Q_UNUSED(startOffset);

    return RequestId();
}

/*!
    \reimp
*/
bool QNearFieldTagType4::handleResponse(const QNearFieldTarget::RequestId &id,
                                        const QByteArray &response)
{
    return QNearFieldTarget::handleResponse(id, response);
}

#include "moc_qnearfieldtagtype4.cpp"

QTM_END_NAMESPACE
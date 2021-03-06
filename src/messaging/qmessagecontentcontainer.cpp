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
#include "qmessagecontentcontainer.h"
#ifdef Q_OS_SYMBIAN
#include "qmessagecontentcontainer_symbian_p.h"
#elif defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
#include "qmessagecontentcontainer_maemo_p.h"
#else
#include "qmessagecontentcontainer_p.h"
#endif

QTM_BEGIN_NAMESPACE

/*!
    \class QMessageContentContainer

    \brief The QMessageContentContainer class provides an interface for internet media (MIME)
    and Transport Neutral Encapsulation Format (TNEF) content that is possibly only
    partially retrieved.


    \inmodule QtMessaging

    \ingroup messaging
    \since 1.0

    A QMessageContentContainer object can either directly contain media such as text,
    image, audio, video, application or message data, or contain multiple parts of content,
    but can not contain both media and multiple parts directly.

    Container objects can be obtained via their QMessageContentContainerId
    identifier, using the find() function of the containing QMessage object.

    For textual content using a recognized charset encoding textContent() will
    return the content as a unicode string.

    For non-multipart content content() will return the content data after decoding any
    transfer encoding used to represent binary data using 7-bit ASCII characters, such as
    quoted-printable and base64.

    The internet media (MIME) type of the container is returned by contentType(),
    the content subtype is returned by contentSubType(), the content type charset parameter
    is returned by contentCharset(), and the content suggested filename by suggestedFileName().

    The type of a container that contains multiple parts of content must be "multipart" (case
    insensitive).  A list of identifiers for directly contained parts of content is returned
    by contentIds().

    An indication of the size of the container and its contents on the originating server is
    given by size(). If the content is entirely available on the device
    isContentAvailable() will return true.

    Non-multipart content can be serialized to a QDataStream using
    writeContentTo(), or to a QTextStream using writeTextContentTo().

    A container also stores name-value pairs known as header fields. Names are ASCII strings,
    while values are charset encoded unicode strings.  A list of the header fields present
    in a container is returned by headerFields(). The unicode string values associated
    with a particular header field name are returned by headerFieldValues().

    \sa QMessage, QMessageContentContainerId
*/

/*!
    \fn QMessageContentContainer::QMessageContentContainer()

    Constructs an empty container object.
*/

/*!
    \fn QMessageContentContainer::QMessageContentContainer(const QMessageContentContainer &other)

    Constructs a copy of \a other.
    \since 1.0
*/

/*!
    \internal
    \fn QMessageContentContainer::operator=(const QMessageContentContainer& other)
    \since 1.0
*/

/*!
    \fn QMessageContentContainer::~QMessageContentContainer()

    Destroys the container object.
*/

/*!
    \fn QMessageContentContainer::contentType() const

    Returns the content type of the container. Common types are "text", "image", "audio",
    "video", "application", "message" and "multipart".

    The internet media (MIME) type of the container is "multipart" if the container directly
    contains multiple parts rather than directly contains media.

    \since 1.0
    \sa contentSubType(), contentCharset()
*/

/*!
    \fn QMessageContentContainer::contentSubType() const

    Returns the internet media (MIME) subtype of the content.

    \since 1.0
    \sa contentType(), contentCharset()
*/

/*!
    \fn QMessageContentContainer::contentCharset() const

    Returns the internet media (MIME) content charset, when defined;
    otherwise an empty array is returned.

    \since 1.0
    \sa contentType(), contentSubType()
*/

/*!
    \fn QMessageContentContainer::suggestedFileName() const

    Returns the suggested filename for the attachment, when defined;
    otherwise an empty array is returned.
    
    On Symbian platform returned file name is UTF-8 encoded.

    The default is an empty array.
    \since 1.0
*/

/*!
    \fn QMessageContentContainer::isContentAvailable() const

    Returns true if the entirety of the content contained is available on the device;
    otherwise returns false.
    \since 1.0
*/

/*!
    \fn QMessageContentContainer::size() const

    If the size of the container is known then returns an indication of the size of the container
    on the originating server, including contents; otherwise returns 0.
    \since 1.0
*/

/*!
    \fn QMessageContentContainer::textContent() const

    For textual content encoded with a recognized charset, returns the content as a unicode string;
    otherwise a null string is returned.
    \since 1.0
*/

/*!
    \fn QMessageContentContainer::content() const

    Return the content after decoding any transfer encoding used to represent binary data
    using 7-bit ASCII characters, such as quoted-printable and base64.

    For textual content any text charset encoding such as Shift-JIS, ISO 2022-JP, KOI8-R,
    Windows-1251 etc will not be decoded.

    \since 1.0
    \sa textContent()
*/

/*!
    \fn QMessageContentContainer::writeTextContent(QTextStream& out) const

    For a non-multipart container, writes the content as returned by textContent()
    to the stream \a out; otherwise does nothing.
    \since 1.0
*/

/*!
    \fn QMessageContentContainer::writeContent(QDataStream& out) const

    For a non-multipart container, writes the content as returned by content()
    to the stream \a out; otherwise does nothing.
    \since 1.0
*/

/*!
    \fn QMessageContentContainer::contentIds() const

    For a multipart container returns a list of identifiers for all content directly contained by
    the container; otherwise returns an empty list.

    \since 1.0
    \sa find(), contains()
*/

/*!
    \fn QMessageContentContainer::find(const QMessageContentContainerId &id) const

    If the container contains another container with identifier \a id either directly or
    recursively, then returns the value of that other container; otherwise returns an
    empty container constructed with the default constructor.

    \since 1.0
    \sa contains(), contentIds()
*/

/*!
    \fn bool QMessageContentContainer::contains(const QMessageContentContainerId &id) const;

    If the container contains content with the identifier \a id, either directly or recursively
    then returns true; otherwise returns false.

    \since 1.0
    \sa find(), contentIds()
*/

/*!
    \fn QMessageContentContainer::headerFieldValue(const QByteArray &name) const

    Returns the value of the first header field of the container with the name \a name, if any;
    otherwise returns a null string.

    \since 1.0
    \sa headerFields(), headerFieldValues()
*/

/*!
    \fn QMessageContentContainer::headerFieldValues(const QByteArray &name) const

    Returns a list of values of header fields with the name \a name, if any;
    otherwise returns an empty list.

    \since 1.0
    \sa headerFields(), headerFieldValue()
*/

/*!
    \fn QMessageContentContainer::headerFields() const

    Returns a list of names of header fields of the container.

    \since 1.0
    \sa headerFieldValue(), headerFieldValues()
*/

QTM_END_NAMESPACE

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

#include "qmediaobject_p.h"
#include <qaudiocapturesource.h>
#include "qaudioendpointselector.h"

QT_BEGIN_NAMESPACE

/*!
    \class QAudioCaptureSource
    \brief The QAudioCaptureSource class provides an interface to query and select an audio input endpoint.
    \inmodule QtMultimediaKit
    \ingroup multimedia
    \since 1.0



    QAudioCaptureSource provides access to the audio inputs available on your system.

    You can query these inputs and select one to use.

    A typical implementation example:
    \snippet doc/src/snippets/multimedia-snippets/media.cpp Audio capture source

    The audiocapturesource interface is then used to:

    - Get and Set the audio input to use.

    The capture interface is then used to:

    - Set the destination using setOutputLocation()

    - Set the format parameters using setAudioCodec(),

    - Control the recording using record(),stop()

    \sa QMediaRecorder
*/

class QAudioCaptureSourcePrivate : public QMediaObjectPrivate
{
public:
    Q_DECLARE_PUBLIC(QAudioCaptureSource)

    void initControls()
    {
        Q_Q(QAudioCaptureSource);

        if (service != 0)
            audioEndpointSelector = qobject_cast<QAudioEndpointSelector*>(service->requestControl(QAudioEndpointSelector_iid));

        if (audioEndpointSelector) {
            q->connect(audioEndpointSelector, SIGNAL(activeEndpointChanged(const QString&)),
                       SIGNAL(activeAudioInputChanged(const QString&)));
            q->connect(audioEndpointSelector, SIGNAL(availableEndpointsChanged()),
                       SIGNAL(availableAudioInputsChanged()));
            q->connect(audioEndpointSelector, SIGNAL(availableEndpointsChanged()),
                       SLOT(statusChanged()));
            errorState = QtMultimediaKit::NoError;
        }
    }

    QAudioCaptureSourcePrivate():provider(0), audioEndpointSelector(0), errorState(QtMultimediaKit::ServiceMissingError) {}
    QMediaServiceProvider *provider;
    QAudioEndpointSelector   *audioEndpointSelector;
    QtMultimediaKit::AvailabilityError errorState;
};

/*!
    Construct a QAudioCaptureSource using the QMediaService from \a provider, with \a parent.
    \since 1.0
*/

QAudioCaptureSource::QAudioCaptureSource(QObject *parent, QMediaServiceProvider *provider):
    QMediaObject(*new QAudioCaptureSourcePrivate, parent, provider->requestService(Q_MEDIASERVICE_AUDIOSOURCE))
{
    Q_D(QAudioCaptureSource);

    d->provider = provider;
    d->initControls();
}

/*!
    Destroys the audiocapturesource object.
*/

QAudioCaptureSource::~QAudioCaptureSource()
{
    Q_D(QAudioCaptureSource);

    if (d->service && d->audioEndpointSelector)
        d->service->releaseControl(d->audioEndpointSelector);

    if (d->provider)
        d->provider->releaseService(d->service);
}

/*!
    Returns the error state of the audio capture service.
    \since 1.0
*/

QtMultimediaKit::AvailabilityError QAudioCaptureSource::availabilityError() const
{
    Q_D(const QAudioCaptureSource);

    return d->errorState;
}

/*!
    Returns true if the audio capture service is available, otherwise returns false.
    \since 1.0
*/
bool QAudioCaptureSource::isAvailable() const
{
    Q_D(const QAudioCaptureSource);

    if (d->service != NULL) {
        if (d->audioEndpointSelector && d->audioEndpointSelector->availableEndpoints().size() > 0)
            return true;
    }
    return false;
}


/*!
    Returns a list of available audio inputs
    \since 1.0
*/

QList<QString> QAudioCaptureSource::audioInputs() const
{
    Q_D(const QAudioCaptureSource);

    QList<QString> list;
    if (d && d->audioEndpointSelector)
        list <<d->audioEndpointSelector->availableEndpoints();

    return list;
}

/*!
    Returns the description of the audio input device with \a name.
    \since 1.0
*/

QString QAudioCaptureSource::audioDescription(const QString& name) const
{
    Q_D(const QAudioCaptureSource);

    if(d->audioEndpointSelector)
        return d->audioEndpointSelector->endpointDescription(name);
    else
        return QString();
}

/*!
    Returns the default audio input name.
    \since 1.0
*/

QString QAudioCaptureSource::defaultAudioInput() const
{
    Q_D(const QAudioCaptureSource);

    if(d->audioEndpointSelector)
        return d->audioEndpointSelector->defaultEndpoint();
    else
        return QString();
}

/*!
    Returns the active audio input name.
    \since 1.0
*/

QString QAudioCaptureSource::activeAudioInput() const
{
    Q_D(const QAudioCaptureSource);

    if(d->audioEndpointSelector)
        return d->audioEndpointSelector->activeEndpoint();
    else
        return QString();
}

/*!
    Set the active audio input to \a name.
    \since 1.0
*/

void QAudioCaptureSource::setAudioInput(const QString& name)
{
    Q_D(const QAudioCaptureSource);

    if(d->audioEndpointSelector)
        return d->audioEndpointSelector->setActiveEndpoint(name);
}

/*!
    \fn QAudioCaptureSource::activeAudioInputChanged(const QString& name)

    Signal emitted when active audio input changes to \a name.
    \since 1.0
*/

/*!
    \fn QAudioCaptureSource::availableAudioInputsChanged()

    Signal is emitted when the available audio inputs change.
    \since 1.0
*/

/*!
  \internal
  \since 1.0
*/
void QAudioCaptureSource::statusChanged()
{
    Q_D(QAudioCaptureSource);

    if (d->audioEndpointSelector) {
        if (d->audioEndpointSelector->availableEndpoints().size() > 0) {
            d->errorState = QtMultimediaKit::NoError;
            emit availabilityChanged(true);
        } else {
            d->errorState = QtMultimediaKit::BusyError;
            emit availabilityChanged(false);
        }
    } else {
        d->errorState = QtMultimediaKit::ServiceMissingError;
        emit availabilityChanged(false);
    }
}

#include "moc_qaudiocapturesource.cpp"
QT_END_NAMESPACE


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

#ifndef QAUDIOENCODERCONTROL_H
#define QAUDIOENCODERCONTROL_H

#include "qmediacontrol.h"
#include "qmediarecorder.h"
#include <QtCore/qlist.h>
#include <QtCore/qpair.h>

QT_BEGIN_NAMESPACE
class QStringList;
class QAudioFormat;
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE

class Q_MULTIMEDIA_EXPORT QAudioEncoderControl : public QMediaControl
{
    Q_OBJECT

public:
    virtual ~QAudioEncoderControl();

    virtual QStringList supportedAudioCodecs() const = 0;
    virtual QString codecDescription(const QString &codecName) const = 0;

    virtual QList<int> supportedSampleRates(const QAudioEncoderSettings &settings,
                                            bool *continuous = 0) const = 0;

    virtual QAudioEncoderSettings audioSettings() const = 0;
    virtual void setAudioSettings(const QAudioEncoderSettings&) = 0;

    virtual QStringList supportedEncodingOptions(const QString &codec) const = 0;
    virtual QVariant encodingOption(const QString &codec, const QString &name) const = 0;
    virtual void setEncodingOption(
            const QString &codec, const QString &name, const QVariant &value) = 0;

protected:
    QAudioEncoderControl(QObject *parent = 0);
};

#define QAudioEncoderControl_iid "com.nokia.Qt.QAudioEncoderControl/1.0"
Q_MEDIA_DECLARE_CONTROL(QAudioEncoderControl, QAudioEncoderControl_iid)

QT_END_NAMESPACE

#endif // QAUDIOCAPTUREPROPERTIESCONTROL_H

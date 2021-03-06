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

#include "simulatorvideorenderercontrol.h"
#include "simulatorcamerasettings.h"
#include "simulatorcamerasession.h"
#include <qabstractvideosurface.h>
#include <qvideoframe.h>
#include <qvideosurfaceformat.h>
#include <QFile>
#include <QColor>
#include <QPainter>

SimulatorVideoRendererControl::SimulatorVideoRendererControl(SimulatorCameraSession *session, QObject *parent) :
    QVideoRendererControl(parent)
  , mSession(session)
  , mSurface(0)
  , mRunning(false)
{
}

SimulatorVideoRendererControl::~SimulatorVideoRendererControl()
{
}

QAbstractVideoSurface *SimulatorVideoRendererControl::surface() const
{
    return mSurface;
}

void SimulatorVideoRendererControl::setSurface(QAbstractVideoSurface *surface)
{
    mSurface = surface;
}

void SimulatorVideoRendererControl::setImagePath(const QString &imagePath)
{
    if (QFile::exists(imagePath)) {
        mImage = QImage(imagePath);
    } else {
        mImage = QImage(800, 600, QImage::Format_RGB32);
        mImage.fill(qRgb(200, 50, 50));
        QPainter painter(&mImage);
        painter.drawText(0, 0, 800, 600, Qt::AlignCenter, imagePath);
    }
    if (mRunning)
        showImage();
}

void SimulatorVideoRendererControl::showImage()
{
    if (!mSurface)
        return;
    stop();
    mShownImage = mImage;
    SimulatorCameraSettings *settings = mSession->settings();
    qreal colorDiff = .0;
    colorDiff += settings->aperture() - settings->defaultAperture();
    colorDiff += (settings->shutterSpeed() - settings->defaultShutterSpeed()) * 400;
    colorDiff += ((qreal)settings->isoSensitivity() - settings->defaultIsoSensitivity()) / 100;
    colorDiff += settings->exposureCompensation();
    int diffToApply = qAbs(colorDiff * 20);
    QPainter painter(&mShownImage);
    if (colorDiff < 0)
        painter.fillRect(mShownImage.rect(), QColor(0, 0, 0, qMin(diffToApply, 255)));
    else
        painter.fillRect(mShownImage.rect(), QColor(255, 255, 255, qMin(diffToApply, 255)));
    QVideoFrame::PixelFormat pixelFormat = QVideoFrame::pixelFormatFromImageFormat(mShownImage.format());
    if (pixelFormat == QVideoFrame::Format_Invalid) {
        mShownImage = mShownImage.convertToFormat(QImage::Format_RGB32);
        pixelFormat = QVideoFrame::Format_RGB32;
    }
    QVideoSurfaceFormat format(mShownImage.size(), pixelFormat);
    mSurface->start(format);
    mSurface->present(mShownImage);
    mRunning = true;
}

void SimulatorVideoRendererControl::stop()
{
    if (!mSurface)
        return;

    if (mSurface->isActive())
        mSurface->stop();
    mRunning = false;
}

const QImage * SimulatorVideoRendererControl::image() const
{
    return &mShownImage;
}

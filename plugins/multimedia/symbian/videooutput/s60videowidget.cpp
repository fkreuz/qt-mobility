/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "s60videowidget.h"

#ifdef USE_PRIVATE_QWIDGET_METHODS
#if QT_VERSION >= 0x040601 && !defined(__WINSCW__)
#define USE_QWIDGET_NATIVE_PAINT_MODE
#endif // >= Qt 4.6.1 and not Emulator
#include <QtGui/private/qwidget_p.h>
#include <QtGui/private/qt_s60_p.h>
#endif // USE_PRIVATE_QWIDGET_METHODS

#include <QtCore/QDebug>

#include <coemain.h>    // CCoeEnv
#include <coecntrl.h>   // CCoeControl
#include <w32std.h>

const int NullOrdinalPosition = -1;

S60VideoWidget::S60VideoWidget(QWidget *parent)
:   QWidget(parent)
,   m_pixmap(NULL)
,   m_paintingEnabled(false)
,   m_topWinId(0)
,   m_ordinalPosition(NullOrdinalPosition)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setPalette(QPalette(Qt::black));
    setAutoFillBackground(false);
    if (!parent)
        setProperty("_q_DummyWindowSurface", true);
#ifdef USE_PRIVATE_QWIDGET_METHODS
    // Warning: if this flag is not set, the application may crash due to
    // CGraphicsContext being called from within the context of
    // QGraphicsVideoItem::paint(), when the video widget is shown.
    static_cast<QSymbianControl *>(winId())->setIgnoreFocusChanged(true);
#endif
}

S60VideoWidget::~S60VideoWidget()
{

}

bool S60VideoWidget::event(QEvent *event)
{
    if (event->type() == QEvent::WinIdChange)
        updateOrdinalPosition();
    return QWidget::event(event);
}

void S60VideoWidget::paintEvent(QPaintEvent *event)
{
    if (m_paintingEnabled && m_pixmap) {
        QPainter painter(this);
        if (m_pixmap->size() != m_contentRect.size())
            qWarning() << "pixmap size does not match expected value";
        painter.drawPixmap(m_contentRect.topLeft(), *m_pixmap);
    }
}

void S60VideoWidget::setVisible(bool visible)
{
    queueReactivateWindow();
    QWidget::setVisible(visible);
}


WId S60VideoWidget::videoWinId() const
{
    WId wid = 0;
    if (internalWinId())
        wid = internalWinId();
    else if (parentWidget() && effectiveWinId())
        wid = effectiveWinId();
    return wid;
}

void S60VideoWidget::setPixmap(const QPixmap *pixmap)
{
    m_pixmap = pixmap;
    update();
}

void S60VideoWidget::setContentRect(const QRect &rect)
{
    if (m_contentRect != rect) {
        m_contentRect = rect;
        update();
    }
}

void S60VideoWidget::setWindowBackgroundColor()
{
    if (WId wid = internalWinId())
        static_cast<RWindow *>(wid->DrawableWindow())->SetBackgroundColor(TRgb(0, 0, 0, 255));
}

WId S60VideoWidget::topWinId() const
{
    return m_topWinId;
}

void S60VideoWidget::setTopWinId(WId id)
{
    m_topWinId = id;
    updateOrdinalPosition();
}

void S60VideoWidget::setOrdinalPosition(int ordinalPosition)
{
    m_ordinalPosition = ordinalPosition;
    updateOrdinalPosition();
}

int S60VideoWidget::ordinalPosition() const
{
    return m_ordinalPosition;
}

void S60VideoWidget::updateOrdinalPosition()
{
    if ((m_ordinalPosition != NullOrdinalPosition) && m_topWinId) {
        if (WId wid = effectiveWinId()) {
            int topOrdinalPosition = m_topWinId->DrawableWindow()->OrdinalPosition();
            queueReactivateWindow();
            wid->DrawableWindow()->SetOrdinalPosition(m_ordinalPosition + topOrdinalPosition);
        }
    }
}

void S60VideoWidget::queueReactivateWindow()
{
    if (!parent()) {
        if (QWidget *activeWindow = QApplication::activeWindow())
            QMetaObject::invokeMethod(this, "reactivateWindow", Qt::QueuedConnection,
                                      Q_ARG(QWidget *, activeWindow));
    }
}

void S60VideoWidget::reactivateWindow(QWidget *widget)
{
    widget->activateWindow();
}

void S60VideoWidget::beginNativePaintEvent(const QRect &rect)
{
    Q_UNUSED(rect)
    emit beginVideoWidgetNativePaintEvent();
}

void S60VideoWidget::endNativePaintEvent(const QRect &rect)
{
    Q_UNUSED(rect)
    CCoeEnv::Static()->WsSession().Flush();
    emit endVideoWidgetNativePaintEvent();
}

void S60VideoWidget::setPaintingEnabled(bool enabled)
{
#ifdef USE_QWIDGET_NATIVE_PAINT_MODE
    qt_widget_private(this)->createExtra();
#endif // USE_QWIDGET_NATIVE_PAINT_MODE
    if (enabled) {
#ifndef VIDEOOUTPUT_GRAPHICS_SURFACES
        setAttribute(Qt::WA_OpaquePaintEvent, false);
        setAttribute(Qt::WA_NoSystemBackground, false);
#endif // VIDEOOUTPUT_GRAPHICS_SURFACES
#ifdef USE_QWIDGET_NATIVE_PAINT_MODE
        qt_widget_private(this)->extraData()->nativePaintMode = QWExtra::Default;
#endif // USE_QWIDGET_NATIVE_PAINT_MODE
    } else {
#ifndef VIDEOOUTPUT_GRAPHICS_SURFACES
        setAttribute(Qt::WA_OpaquePaintEvent, true);
        setAttribute(Qt::WA_NoSystemBackground, true);
#endif // !VIDEOOUTPUT_GRAPHICS_SURFACES
#ifdef USE_QWIDGET_NATIVE_PAINT_MODE
#ifdef VIDEOOUTPUT_GRAPHICS_SURFACES
        qt_widget_private(this)->extraData()->nativePaintMode = QWExtra::Disable;
#else // VIDEOOUTPUT_GRAPHICS_SURFACES
        qt_widget_private(this)->extraData()->nativePaintMode = QWExtra::ZeroFill;
        qt_widget_private(this)->extraData()->receiveNativePaintEvents = true;
#endif // VIDEOOUTPUT_GRAPHICS_SURFACES
#endif // USE_QWIDGET_NATIVE_PAINT_MODE
        winId(); // Create native window handle
    }
    m_paintingEnabled = enabled;
    setWindowBackgroundColor();
}

void S60VideoWidget::setFullScreen(bool enabled)
{
    if (enabled)
        showFullScreen();
    else
        showMaximized();
}

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

#ifndef QWMPPLAYERSERVICE_H
#define QWMPPLAYERSERVICE_H

#include "qwmpevents.h"

#include <qmediaservice.h>

QT_BEGIN_NAMESPACE
class QMediaMetaData;
class QMediaPlayerControl;
class QMediaPlaylist;
class QVideoWindowControl;
QT_END_NAMESPACE

#ifdef QWMP_EVR
class QEvrVideoOverlay;
#endif

class QWmpMetaData;
class QWmpPlayerControl;
class QWmpPlaylist;
class QWmpPlaylistControl;
class QWmpVideoOverlay;

QT_USE_NAMESPACE
class QWmpPlayerService
    : public QMediaService
    , public IOleClientSite
    , public IServiceProvider
    , public IWMPRemoteMediaServices
{
    Q_OBJECT
public:
    enum EmbedMode
    {
        LocalEmbed,
        RemoteEmbed
    };

    QWmpPlayerService(EmbedMode mode, QObject *parent = 0);
    ~QWmpPlayerService();

    QMediaControl *requestControl(const char *name);
    void releaseControl(QMediaControl *control);

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void **object);
    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();

    // IOleClientSite
    HRESULT STDMETHODCALLTYPE SaveObject();
    HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker **ppmk);
    HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer **ppContainer);
    HRESULT STDMETHODCALLTYPE ShowObject();
    HRESULT STDMETHODCALLTYPE OnShowWindow(BOOL fShow);
    HRESULT STDMETHODCALLTYPE RequestNewObjectLayout();

    // IServiceProvider
    HRESULT STDMETHODCALLTYPE QueryService(REFGUID guidService, REFIID riid, void **ppvObject);

    // IWMPRemoteMediaServices
    HRESULT STDMETHODCALLTYPE GetServiceType(BSTR *pbstrType);
    HRESULT STDMETHODCALLTYPE GetApplicationName(BSTR *pbstrName);
    HRESULT STDMETHODCALLTYPE GetScriptableObject(BSTR *pbstrName, IDispatch **ppDispatch);
    HRESULT STDMETHODCALLTYPE GetCustomUIMode(BSTR *pbstrFile);

private:
    volatile LONG m_ref;
    const EmbedMode m_embedMode;
    IWMPPlayer4 *m_player;
    IOleObject *m_oleObject;
    QWmpEvents *m_events;
    QWmpPlayerControl *m_control;
    QWmpMetaData *m_metaData;
    QWmpPlaylistControl *m_playlist;
    QVideoWindowControl *m_activeVideoOverlay;
    QWmpVideoOverlay *m_oleVideoOverlay;
#ifdef QWMP_EVR
    QEvrVideoOverlay *m_evrVideoOverlay;
#endif
};

#endif

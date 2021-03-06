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
** This file is part of the Ovi services plugin for the Maps and
** Navigation API.  The use of these services, whether by use of the
** plugin or by other means, is governed by the terms and conditions
** described by the file OVI_SERVICES_TERMS_AND_CONDITIONS.txt in
** this package, located in the directory containing the Ovi services
** plugin source code.
**
****************************************************************************/

#ifndef QGEOMAPPINGMANAGERENGINE_NOKIA_H
#define QGEOMAPPINGMANAGERENGINE_NOKIA_H

#include "qgeoserviceproviderplugin_nokia.h"

#include <qgeoserviceprovider.h>
#include <qgeotiledmappingmanagerengine.h>

class QNetworkAccessManager;
class QNetworkDiskCache;

#ifdef  USE_CHINA_NETWORK_REGISTRATION
#include "../../../src/systeminfo/qsystemnetworkinfo.h"
#endif

QTM_USE_NAMESPACE

class QGeoMappingManagerEngineNokia : public QGeoTiledMappingManagerEngine
{
    Q_OBJECT
public:
    QGeoMappingManagerEngineNokia(const QMap<QString, QVariant> &parameters,
                                  QGeoServiceProvider::Error *error,
                                  QString *errorString);
    ~QGeoMappingManagerEngineNokia();

    QGeoMapData* createMapData();
    QGeoTiledMapReply* getTileImage(const QGeoTiledMapRequest &request);

    int logoPosition() const;
    const QString& host() const;
    QChar firstSubdomain() const;
    unsigned char maxSubdomains() const;
    const QString& token() const;
    const QString& applicationId() const;
    const QString& referer() const;

private:
    Q_DISABLE_COPY(QGeoMappingManagerEngineNokia)

    QString getRequestString(const QGeoTiledMapRequest &request) const;

    static QString sizeToStr(const QSize &size);
    static QString mapTypeToStr(QGraphicsGeoMap::MapType type);

    void setupServiceInfo();
    void setHost(const QString& host);

    bool isValidParameter(const QString& param);

#if defined(Q_OS_SYMBIAN)
    QChar findFirstInternalFlashDrive();
#endif //Q_OS_SYMBIAN

private slots:
    void currentMobileCountryCodeChanged(const QString& mcc);

private:
    QNetworkAccessManager *m_networkManager;
    QNetworkDiskCache *m_cache;
    QString m_host;
    QChar m_firstSubdomain;
    unsigned char m_maxSubdomains;
    QString m_token;
    QString m_referer;
    QString m_applicationId;
    int m_logoPosition;

#ifdef USE_CHINA_NETWORK_REGISTRATION
    QSystemNetworkInfo m_networkInfo;
#endif
};

#endif

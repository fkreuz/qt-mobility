/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#ifndef QCONTACTGEOLOCATION_H
#define QCONTACTGEOLOCATION_H

#include <QString>

#include "qtcontactsglobal.h"
#include "qcontactdetail.h"
#include "qcontact.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_CONTACTS_EXPORT QContactGeolocation : public QContactDetail
{
public:
#ifdef Q_QDOC
    const char* DefinitionName;
    const char* FieldLabel;
    const char* FieldLatitude;
    const char* FieldLongitude;
    const char* FieldAccuracy;
    const char* FieldAltitude;
    const char* FieldAltitudeAccuracy;
    const char* FieldHeading;
    const char* FieldSpeed;
    const char* FieldTimestamp;
#else
    Q_DECLARE_CUSTOM_CONTACT_DETAIL(QContactGeolocation, "Geolocation")
    Q_DECLARE_LATIN1_LITERAL(FieldLabel, "Label");
    Q_DECLARE_LATIN1_LITERAL(FieldLatitude, "Latitude");
    Q_DECLARE_LATIN1_LITERAL(FieldLongitude, "Longitude");
    Q_DECLARE_LATIN1_LITERAL(FieldAccuracy, "Accuracy");
    Q_DECLARE_LATIN1_LITERAL(FieldAltitude, "Altitude");
    Q_DECLARE_LATIN1_LITERAL(FieldAltitudeAccuracy, "AltitudeAccuracy");
    Q_DECLARE_LATIN1_LITERAL(FieldHeading, "Heading");
    Q_DECLARE_LATIN1_LITERAL(FieldSpeed, "Speed");
    Q_DECLARE_LATIN1_LITERAL(FieldTimestamp, "Timestamp");
#endif

    void setLabel(const QString& label) {setValue(FieldLabel, label);}
    QString label() const {return value(FieldLabel);}
    void setLatitude(double latitude) {setValue(FieldLatitude, latitude);}
    double latitude() const {return variantValue(FieldLatitude).toDouble();}
    void setLongitude(double longitude) {setValue(FieldLongitude, longitude);}
    double longitude() const {return variantValue(FieldLongitude).toDouble();}
    void setAccuracy(double accuracy) {setValue(FieldAccuracy, accuracy);}
    double accuracy() const {return variantValue(FieldAccuracy).toDouble();}
    void setAltitude(double altitude) {setValue(FieldAltitude, altitude);}
    double altitude() const {return variantValue(FieldAltitude).toDouble();}
    void setAltitudeAccuracy(double altitudeAccuracy) {setValue(FieldAltitudeAccuracy, altitudeAccuracy);}
    double altitudeAccuracy() const {return variantValue(FieldAltitudeAccuracy).toDouble();}
    void setHeading(double heading) {setValue(FieldHeading, heading);}
    double heading() const {return variantValue(FieldHeading).toDouble();}
    void setSpeed(double speed) {setValue(FieldSpeed, speed);}
    double speed() const {return variantValue(FieldSpeed).toDouble();}
    void setTimestamp(const QDateTime& timestamp) {setValue(FieldTimestamp, timestamp);}
    QDateTime timestamp() const {return variantValue(FieldTimestamp).toDateTime();}
};

QTM_END_NAMESPACE

#endif

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

#ifndef QTRACKERCONTACTSAVEREQUEST_H_
#define QTRACKERCONTACTSAVEREQUEST_H_

#include <QObject>
#include <qtrackercontactasyncrequest.h>
#include <QPair>
#include <QList>
#include <QtTracker/QLive>
#include <QtTracker/ontologies/nco.h>
#include <qtcontacts.h>

class QContactAbstractRequest;
class QContactManagerEngine;

class QTrackerContactSaveRequest: public QObject, public QTrackerContactAsyncRequest
{
    Q_OBJECT
public:
    QTrackerContactSaveRequest(QContactAbstractRequest* req, QContactManagerEngine* parent);
    virtual ~QTrackerContactSaveRequest();

private:
    /* worker methods*/
    void saveContact(QContact &contact);
    void computeProgress();
    void addAffiliation(SopranoLive::RDFServicePtr service, QContactLocalId contactId);
    void saveContactDetails(SopranoLive::RDFServicePtr service,SopranoLive::Live<SopranoLive::nco::PersonContact>& ncoContact,const QContact &contact);
    void saveAddresses(SopranoLive::RDFServicePtr service, SopranoLive::RDFVariable &var, const QList<QContactDetail> &details );
    void saveEmails(SopranoLive::RDFServicePtr service, SopranoLive::RDFVariable &var, const QList<QContactDetail> &details );
    void saveUrls(SopranoLive::RDFServicePtr service, SopranoLive::RDFVariable &var, const QList<QContactDetail> &details );
    void savePhoneNumbers(SopranoLive::RDFServicePtr service, SopranoLive::RDFVariable &var, const QList<QContactDetail> &details );
    void deletePhoneNumbers(SopranoLive::RDFServicePtr service, const SopranoLive::RDFVariable& rdfContactIn);
    void addTag(SopranoLive::RDFServicePtr service, SopranoLive::RDFVariable &var, const QString &tag);
    void createTagIfItDoesntExistAlready(SopranoLive::RDFServicePtr service, const QString &tag);

private:
    /*holding the data about status of async operation*/
    QList<QContact> contactsFinished;
    QList<QContactManager::Error> errorsOfContactsFinished;

    /* extracted utilities */
    static QStringList detailsDefinitionsInContact(const QContact &c);
    static bool contactHasWorkRelatedDetails(const QContact &c);

};

#endif /* QTRACKERCONTACTSAVEREQUEST_H_ */
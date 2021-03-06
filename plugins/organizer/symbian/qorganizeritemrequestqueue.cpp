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

// User includes
#include "qorganizeritemrequestqueue.h"

QOrganizerItemRequestQueue* QOrganizerItemRequestQueue::instance(
        QOrganizerItemSymbianEngine& aOrganizerItemManagerEngine)
{
    return new QOrganizerItemRequestQueue(aOrganizerItemManagerEngine);
}

QOrganizerItemRequestQueue::~QOrganizerItemRequestQueue()
{
    // Cleanup, delete all the pointers from the hash map
    QMapIterator<QOrganizerAbstractRequest*, 
    COrganizerItemRequestsServiceProvider*> iter(m_abstractRequestMap);
    // Delete all the asynch requests one by one
    while (iter.hasNext()) {
        // Advance to next item
        iter.next();
        // It deletes the asynch request service provider
        delete iter.value();
    }
    // Clear the abstract request map
    m_abstractRequestMap.clear();
}

bool QOrganizerItemRequestQueue::startRequest(
        QOrganizerAbstractRequest* req)
{
    // Find m_abstractRequestMap if an asynchronous service provider for request
    // req already exists
    COrganizerItemRequestsServiceProvider* requestServiceProvider(
        m_abstractRequestMap[req]);
    // asynchronous service provider does not exist, create a new one
    if (!requestServiceProvider) {
        TRAPD(err, requestServiceProvider = 
            COrganizerItemRequestsServiceProvider::NewL(
                m_organizerItemManagerEngine));
        if (err) {
            // Request service provider not instantiated successfully
            return false;
        }
        m_abstractRequestMap.insert(req, requestServiceProvider);
    }
    // Start the request
    return requestServiceProvider->StartRequest(req);
}

// To cancel aReq request
bool QOrganizerItemRequestQueue::cancelRequest(
        QOrganizerAbstractRequest* req)
{
    COrganizerItemRequestsServiceProvider* requestServiceProvider(
            m_abstractRequestMap[req]);
    //Cancel the request
    if (requestServiceProvider) {
            return requestServiceProvider->CancelRequest();
    }
    else {
        return false;
    }
}

// Wait for request to complete 
bool QOrganizerItemRequestQueue::waitForRequestFinished(
        QOrganizerAbstractRequest* req, int msecs)
{
    // Verify that request exists in this manager
    if (!m_abstractRequestMap.keys().contains(req)) 
        return false;
    
    // Verify that request is active
    if (req->state() != QOrganizerAbstractRequest::ActiveState)
        return false;
    
    // Create an event loop
    QEventLoop *loop = new QEventLoop(this);
    
    // If request state changes quit the loop
    QObject::connect(req, SIGNAL(stateChanged(QOrganizerAbstractRequest::State)), loop, SLOT(quit()));

    // Set a timeout for the request
    // NOTE: zero means wait forever
    if (msecs > 0)
        QTimer::singleShot(msecs, loop, SLOT(quit()));

    // Start the loop
    loop->exec();
    
    loop->disconnect();
    loop->deleteLater();

    return (req->state() == QOrganizerAbstractRequest::FinishedState);
}

// Request is not more a valid request
void QOrganizerItemRequestQueue::requestDestroyed(
        QOrganizerAbstractRequest* req)
{
    // Get the pointer to the Asynchronous service provider for req request
    COrganizerItemRequestsServiceProvider* requestServiceProvider(
                m_abstractRequestMap[req]);
    if (requestServiceProvider) {
        // Remove req request & asynchronous service provider from the map 
        // count is used only for the debugging purpose. Count should always be 
        // 1, there is a serious programming mistake if not so.
        int count(m_abstractRequestMap.remove(req));
		// Delete the asynchronous service provider
        delete requestServiceProvider;
        }
} 

QOrganizerItemRequestQueue::QOrganizerItemRequestQueue(
        QOrganizerItemSymbianEngine& aOrganizerItemManagerEngine) : 
        m_organizerItemManagerEngine(aOrganizerItemManagerEngine)
{

}

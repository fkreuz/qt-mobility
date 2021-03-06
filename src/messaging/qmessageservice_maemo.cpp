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
#include "qmessageservice.h"
#include "qmessageservice_maemo_p.h"

#include "maemohelpers_p.h"
#include "modestengine_maemo_p.h"
#include "telepathyengine_maemo_p.h"
#include "eventloggerengine_maemo_p.h"
#include <QUrl>
#include "hildon-uri.h"

QTM_BEGIN_NAMESPACE
#define EVENTLOGGER_THREAD


QMessageServicePrivate::QMessageServicePrivate(QMessageService* parent)
 : q_ptr(parent),
   _state(QMessageService::InactiveState),
   _error(QMessageManager::NoError),
   _active(false), _actionId(-1),
   _pendingRequestCount(0)
{
}

QMessageServicePrivate::~QMessageServicePrivate()
{
}

QMessageServicePrivate* QMessageServicePrivate::implementation(const QMessageService &service)
{
    return service.d_ptr;
}

bool QMessageServicePrivate::queryMessages(QMessageService &messageService,
                                           const QMessageFilter &filter,
                                           const QMessageSortOrder &sortOrder,
                                           uint limit, uint offset,
                                           EnginesToCall enginesToCall)
{
    if (_active) {
        return false;
    }

    _filter = filter;
    MessagingHelper::handleNestedFiltersFromMessageFilter(_filter);

    _ids.clear();
    _sorted = true;
    _filtered = true;

    _active = true;
    _error = QMessageManager::NoError;

    _pendingRequestCount = 0;

    bool preFiltered = false;

    if (enginesToCall & EnginesToCallTelepathy) {
        if (MessagingHelper::preFilter(_filter, QMessage::Sms)) {
#ifndef EVENTLOGGER_THREAD
            _ids = EventLoggerEngine::instance()->filterAndOrderMessages(filter,sortOrder,QString(),QMessageDataComparator::MatchFlags());
            QMetaObject::invokeMethod(this, "messagesFoundSlot", Qt::QueuedConnection);
#else
            EventLoggerEngine::instance()->filterMessages(this, _filter,sortOrder,QString(),QMessageDataComparator::MatchFlags());
#endif
            _pendingRequestCount++;
        } else {
            preFiltered = true;
        }
    }

    if (enginesToCall & EnginesToCallModest) {
        if (MessagingHelper::preFilter(_filter, QMessage::Email)) {
            if (ModestEngine::instance()->queryMessages(messageService, _filter, sortOrder, limit, offset)) {
                _pendingRequestCount++;
            }
        } else {
            preFiltered = true;
        }
    } 

    if (_pendingRequestCount > 0) {
        _sortOrder = sortOrder;
        _limit = limit;
        _offset = offset;

        stateChanged(QMessageService::ActiveState);
    } else {
        _filter = QMessageFilter();
        if (preFiltered) {
            stateChanged(QMessageService::ActiveState);
            _pendingRequestCount = 1;
            QMetaObject::invokeMethod(this, "messagesFoundSlot", Qt::QueuedConnection);
            QMetaObject::invokeMethod(this, "finishedSlot", Qt::QueuedConnection);
            return true;
        } else {
            setFinished(false);
        }
    }

    return _active;
}

bool QMessageServicePrivate::queryMessages(QMessageService &messageService,
                                           const QMessageFilter &filter,
                                           const QString &body,
                                           QMessageDataComparator::MatchFlags matchFlags,
                                           const QMessageSortOrder &sortOrder,
                                           uint limit, uint offset,
                                           EnginesToCall enginesToCall)
{
    if (_active) {
        return false;
    }

    _filter = filter;
    MessagingHelper::handleNestedFiltersFromMessageFilter(_filter);

    _ids.clear();
    _sorted = true;
    _filtered = true;

    _active = true;
    _error = QMessageManager::NoError;

    _pendingRequestCount = 0;

    bool preFiltered = false;

    if (enginesToCall & EnginesToCallTelepathy) {
        if (MessagingHelper::preFilter(_filter, QMessage::Sms)) {
#ifndef EVENTLOGGER_THREAD
            _ids= EventLoggerEngine::instance()->filterAndOrderMessages(filter,sortOrder,body,matchFlags);
            QMetaObject::invokeMethod(this, "messagesFoundSlot", Qt::QueuedConnection);
#else
            EventLoggerEngine::instance()->filterMessages(this, _filter,sortOrder,body,matchFlags);
#endif
            _pendingRequestCount++;
        } else {
            preFiltered = true;
        }
    }

    if (enginesToCall & EnginesToCallModest) {
        if (MessagingHelper::preFilter(_filter, QMessage::Email)) {
            if (ModestEngine::instance()->queryMessages(messageService, _filter, body, matchFlags,
                                                        sortOrder, limit, offset)) {
                _pendingRequestCount++;
            }
        } else {
            preFiltered = true;
        }
    }

    if (_pendingRequestCount > 0) {
        _sortOrder = sortOrder;
        _limit = limit;
        _offset = offset;

        stateChanged(QMessageService::ActiveState);
    } else {
        _filter = QMessageFilter();
        if (preFiltered) {
            stateChanged(QMessageService::ActiveState);
            _pendingRequestCount = 1;
            QMetaObject::invokeMethod(this, "messagesFoundSlot", Qt::QueuedConnection);
            QMetaObject::invokeMethod(this, "finishedSlot", Qt::QueuedConnection);
            return true;
        } else {
            setFinished(false);
        }
    }

    return _active;
}

bool QMessageServicePrivate::countMessages(QMessageService &messageService,
                                           const QMessageFilter &filter,
                                           EnginesToCall enginesToCall)
{
    if (_active) {
        return false;
    }

    QMessageFilter handledFilter = filter;
    MessagingHelper::handleNestedFiltersFromMessageFilter(handledFilter);

    _count = 0;

    _active = true;
    _error = QMessageManager::NoError;

    _pendingRequestCount = 0;

    bool preFiltered = false;

    //TODO: SMS count support
    //if (enginesToCall & EnginesToCallTelepathy) {
    //}

    if (enginesToCall & EnginesToCallModest) {
        if (MessagingHelper::preFilter(handledFilter, QMessage::Email)) {
            if (ModestEngine::instance()->countMessages(messageService, handledFilter)) {
                _pendingRequestCount++;
            }
        } else {
            preFiltered = true;
        }
    }

    if (_pendingRequestCount > 0) {
        stateChanged(QMessageService::ActiveState);
    } else {
        _filter = QMessageFilter();
        if (preFiltered) {
            stateChanged(QMessageService::ActiveState);
            _pendingRequestCount = 1;
            QMetaObject::invokeMethod(this, "messagesCountedSlot", Qt::QueuedConnection);
            QMetaObject::invokeMethod(this, "finishedSlot", Qt::QueuedConnection);
            return true;
        } else {
            setFinished(false);
        }
    }

    return _active;
}


void QMessageServicePrivate::setFinished(bool successful)
{
    if (!successful && _pendingRequestCount > 0) {
        _pendingRequestCount--;
    }

    if (_pendingRequestCount == 0) {
        if (!successful && (_error == QMessageManager::NoError)) {
            // We must report an error of some sort
            _error = QMessageManager::RequestIncomplete;
        }

        stateChanged(QMessageService::FinishedState);
    }
}

void QMessageServicePrivate::stateChanged(QMessageService::State state)
{
    if (state == QMessageService::FinishedState) {
        _active = false;
    }

    _state = state;
    emit q_ptr->stateChanged(_state);
}

void QMessageServicePrivate::messagesFound(const QMessageIdList &ids, bool isFiltered, bool isSorted)
{
    _pendingRequestCount--;

    if (!isFiltered) {
        _filtered = false;
    }

    if (!isSorted) {
        _sorted = false;
    } else {
        if ((ids.count() > 0) && (_ids.count() > 0)) {
            _sorted = false;
        }
    }

    _ids.append(ids);

    if (_pendingRequestCount == 0) {
        if (!_filtered) {
            MessagingHelper::filterMessages(_ids, _filter);
        }
        if (!_sorted) {
            MessagingHelper::orderMessages(_ids, _sortOrder);
        }
        MessagingHelper::applyOffsetAndLimitToMessageIdList(_ids, _limit, _offset);

        emit q_ptr->messagesFound(_ids);

        setFinished(true);

        _ids.clear();
        _filter = QMessageFilter();
        _sortOrder = QMessageSortOrder();
    }
}

void QMessageServicePrivate::messagesCounted(int count)
{
    _pendingRequestCount--;

    _count += count;

    if (_pendingRequestCount == 0) {
        emit q_ptr->messagesCounted(_count);

        setFinished(true);

        _filter = QMessageFilter();
        _count = 0;
    }
}

void QMessageServicePrivate::progressChanged(uint value, uint total)
{
    emit q_ptr->progressChanged(value, total);
}

void QMessageServicePrivate::messagesFoundSlot()
{
    messagesFound(QMessageIdList(), true, false);
}

void QMessageServicePrivate::messagesCountedSlot()
{
    messagesCounted(0);
}

void QMessageServicePrivate::finishedSlot(bool successful)
{
    setFinished(successful);
}

QMessageService::QMessageService(QObject *parent)
 : QObject(parent),
   d_ptr(new QMessageServicePrivate(this))
{
    EventLoggerEngine::instance();
    TelepathyEngine::instance();
}

QMessageService::~QMessageService()
{
}

bool QMessageService::queryMessages(const QMessageFilter &filter, const QMessageSortOrder &sortOrder, uint limit, uint offset)
{
    return d_ptr->queryMessages(*this, filter, sortOrder, limit, offset);
}

bool QMessageService::queryMessages(const QMessageFilter &filter, const QString &body, QMessageDataComparator::MatchFlags matchFlags, const QMessageSortOrder &sortOrder, uint limit, uint offset)
{
    return d_ptr->queryMessages(*this, filter, body, matchFlags, sortOrder, limit, offset);
}

bool QMessageService::countMessages(const QMessageFilter &filter)
{
    return d_ptr->countMessages(*this, filter);
}

bool QMessageService::send(QMessage &message)
{
    if (d_ptr->_active) {
        return false;
    }

    d_ptr->_active = true;
    d_ptr->_error = QMessageManager::NoError;

    bool retVal = true;

    d_ptr->_state = QMessageService::ActiveState;
    emit stateChanged(d_ptr->_state);

    QMessageAccountId accountId = message.parentAccountId();
    QMessage::Type msgType = QMessage::NoType;

    // Check message type
    if (message.type() == QMessage::AnyType || message.type() == QMessage::NoType) {
        QMessage::TypeFlags types = QMessage::NoType;
        if (accountId.isValid()) {
            // ParentAccountId was defined => Message type can be read
            // from parent account
            QMessageAccount account = QMessageAccount(accountId);
            QMessage::TypeFlags types = account.messageTypes();
            if (types & QMessage::Sms) {
                msgType = QMessage::Sms;
            } else if (account.messageTypes() & QMessage::InstantMessage) {
                msgType = QMessage::InstantMessage;
            } else if (types & QMessage::Mms) {
                msgType = QMessage::Mms;
            } else if (types & QMessage::Email) {
                msgType = QMessage::Email;
            }
        }
        if (msgType == QMessage::NoType) {
            d_ptr->_error = QMessageManager::ConstraintFailure;
            retVal = false;
        }
    }

    if (retVal) {
        // Check account
        if (!accountId.isValid()) {
            accountId = QMessageAccount::defaultAccount(message.type());
            if (!accountId.isValid()) {
                d_ptr->_error = QMessageManager::InvalidId;
                retVal = false;
            }
        }
    }

    QMessageAccount account(accountId);
    if (retVal) {
        // Check account/message type compatibility
        if (!(account.messageTypes() & message.type()) && (msgType == QMessage::NoType)) {
            d_ptr->_error = QMessageManager::ConstraintFailure;
            retVal = false;
        }
    }

    if (retVal) {
        // Check recipients
        QMessageAddressList recipients = message.to() + message.bcc() + message.cc();
        if (recipients.isEmpty()) {
            d_ptr->_error = QMessageManager::ConstraintFailure;
            retVal = false;
        }
    }

    if (retVal) {
        QMessage outgoing(message);

        // Set default account if unset
        if (!outgoing.parentAccountId().isValid()) {
            outgoing.setParentAccountId(accountId);
        }

        if (account.messageTypes() & QMessage::Sms) {
            retVal = TelepathyEngine::instance()->sendMessage(outgoing);
        } else if (account.messageTypes() & QMessage::InstantMessage) {
            retVal = TelepathyEngine::instance()->sendMessage(outgoing);
        } else if (account.messageTypes() & QMessage::Mms) {
            d_ptr->_error = QMessageManager::NotYetImplemented;
            qWarning() << "QMessageService::send not yet implemented for MMS";
            retVal = false;
        } else if (account.messageTypes() & QMessage::Email) {
            retVal = ModestEngine::instance()->sendEmail(message);
        }
    }

    d_ptr->setFinished(retVal);
    return retVal;
}

bool QMessageService::compose(const QMessage &message)
{
    if (d_ptr->_active) {
        return false;
    }

    d_ptr->_active = true;
    d_ptr->_error = QMessageManager::NoError;

    bool retVal=false;
    d_ptr->_state = QMessageService::ActiveState;
    emit stateChanged(d_ptr->_state);

    if (message.type() == QMessage::Sms && !message.to().isEmpty() && !message.to().first().addressee().isEmpty()) {
      QUrl smsUrl((QString("sms:%1").arg(message.to().first().addressee())));
      smsUrl.addQueryItem("body",message.textContent());
      hildon_uri_open(smsUrl.toString().toStdString().c_str(),NULL,NULL);
      retVal = true;


    } else if (message.type() == QMessage::Mms) {
        d_ptr->_error = QMessageManager::NotYetImplemented; //TODO:
        qWarning() << "QMessageService::compose not yet implemented for MMS";
        retVal = false;
    } else if (message.type() == QMessage::Email) {
        retVal = ModestEngine::instance()->composeEmail(message);
    }

    d_ptr->setFinished(retVal); 
    return retVal;
}



bool QMessageService::retrieveHeader(const QMessageId& id)
{
    Q_UNUSED(id)
    return false; // stub
}

bool QMessageService::retrieveBody(const QMessageId& id)
{
    if (d_ptr->_active) {
        return false;
    }

    if (!id.isValid()) {
        d_ptr->_error = QMessageManager::InvalidId;
        return false;
    }

    d_ptr->_active = true;
    d_ptr->_error = QMessageManager::NoError;

    bool retVal = true;
    d_ptr->stateChanged(QMessageService::ActiveState);

    if (id.toString().startsWith("MO_")) {
        retVal = ModestEngine::instance()->retrieveBody(*this, id);
        if (retVal == true) {
            d_ptr->_pendingRequestCount = 1;
        }
    } else {
        retVal = false;
    }

    if (retVal == false) {
        d_ptr->setFinished(retVal);
    }

    return retVal;
}

bool QMessageService::retrieve(const QMessageId &messageId, const QMessageContentContainerId& id)
{
    if (d_ptr->_active) {
        return false;
    }

    if (!id.isValid()) {
        d_ptr->_error = QMessageManager::InvalidId;
        return false;
    }

    d_ptr->_active = true;
    d_ptr->_error = QMessageManager::NoError;

    bool retVal = true;
    d_ptr->stateChanged(QMessageService::ActiveState);

    if (messageId.toString().startsWith("MO_")) {
        retVal = ModestEngine::instance()->retrieve(*this, messageId, id, NULL);
        if (retVal == true) {
            d_ptr->_pendingRequestCount = 1;
        }
    } else {
        retVal = false;
    }

    if (retVal == false) {
        d_ptr->setFinished(retVal);
    }

    return retVal;
}

bool QMessageService::show(const QMessageId& id)
{
    if (d_ptr->_active) {
        return false;
    }

    if (!id.isValid()) {
        d_ptr->_error = QMessageManager::InvalidId;
        return false;
    }

    d_ptr->_active = true;
    d_ptr->_error = QMessageManager::NoError;

    bool retVal = true;
    d_ptr->_state = QMessageService::ActiveState;
    emit stateChanged(d_ptr->_state);

    if (id.toString().startsWith("MO_")) {
        retVal = ModestEngine::instance()->showMessage(id);
    } else {
        retVal = false;
    }

    d_ptr->setFinished(retVal);
    return retVal;
}

bool QMessageService::exportUpdates(const QMessageAccountId &id)
{
    if (d_ptr->_active) {
        return false;
    }

    if (!id.isValid()) {
        d_ptr->_error = QMessageManager::InvalidId;
        return false;
    }

    d_ptr->_active = true;
    d_ptr->_error = QMessageManager::NoError;

    bool retVal = true;
    d_ptr->_state = QMessageService::ActiveState;
    emit stateChanged(d_ptr->_state);

    if (id.toString().startsWith("MO_")) {
        retVal = ModestEngine::instance()->exportUpdates(id);
    } else {
        retVal = false;
    }

    d_ptr->setFinished(retVal);
    return retVal;
}

bool QMessageService::synchronize(const QMessageAccountId& id)
{
    Q_UNUSED(id);

    if(d_ptr->_active) {
        qWarning() << "Service is currently busy";
        return false;
    }

    d_ptr->_error = QMessageManager::NotYetImplemented;
    d_ptr->setFinished(false);

    return false;
}

QMessageService::State QMessageService::state() const
{
    return d_ptr->_state;
}

void QMessageService::cancel()
{
}

QMessageManager::Error QMessageService::error() const
{
    return d_ptr->_error;
}

#include "moc_qmessageservice_maemo_p.cpp"

QTM_END_NAMESPACE

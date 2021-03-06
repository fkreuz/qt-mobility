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

#include "ccentralrepositoryhandler_p.h"

#include "xqsettingskey_p.h"

CCentralRepositoryHandler* CCentralRepositoryHandler::NewL(TUid aUid)
{
    CCentralRepositoryHandler* self = new (ELeave) CCentralRepositoryHandler(aUid);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

void CCentralRepositoryHandler::ConstructL()
{
    iRepository = CRepository::NewL(m_uid);
}

CCentralRepositoryHandler::CCentralRepositoryHandler(TUid aUid) :
    m_uid(aUid)
{
}

CCentralRepositoryHandler::~CCentralRepositoryHandler()
{
    foreach (CCenRepMonitor* monitor, m_monitors)
        delete monitor;

    delete iRepository;
}

void CCentralRepositoryHandler::setObserver(MSettingsHandlerObserver* observer)
{
    m_observer = observer;
}

TInt CCentralRepositoryHandler::deleteKey(unsigned long key)
{
    return iRepository->Delete(key);
}

TInt CCentralRepositoryHandler::resetKey(unsigned long key)
{
    return iRepository->Reset(key);
}

TInt CCentralRepositoryHandler::resetRepository()
{
    return iRepository->Reset();
}

TInt CCentralRepositoryHandler::getValue(unsigned long key, TInt& value)
{
    return iRepository->Get(key, value);
}

TInt CCentralRepositoryHandler::getValue(unsigned long key, TReal& value)
{
    return iRepository->Get(key, value);
}

void CCentralRepositoryHandler::getValueL(unsigned long key, RBuf8& value)
{
    TInt actualLength;
    TInt err = iRepository->Get(key, value, actualLength);
    if (err == KErrOverflow)
    {
        value.ReAllocL(actualLength);
        err = iRepository->Get(key, value);
    }
    User::LeaveIfError(err);
}

void CCentralRepositoryHandler::getValueL(unsigned long key, RBuf16& value)
{
    TInt actualLength;
    TInt err = iRepository->Get(key, value, actualLength);
    if (err == KErrOverflow)
    {
        value.ReAllocL(actualLength);
        err = iRepository->Get(key, value);
    }
    User::LeaveIfError(err);
}

TInt CCentralRepositoryHandler::setValue(unsigned long key, const TInt& value)
{
    return iRepository->Set(key, value);
}

TInt CCentralRepositoryHandler::setValue(unsigned long key, const TReal& value)
{
    return iRepository->Set(key, value);
}

TInt CCentralRepositoryHandler::setValue(unsigned long key, const TDesC8& value)
{
    return iRepository->Set(key, value);
}

TInt CCentralRepositoryHandler::setValue(unsigned long key, const TDesC16& value)
{
    return iRepository->Set(key, value);
}

TInt CCentralRepositoryHandler::createKey(unsigned long key, const TInt& value)
{
    return iRepository->Create(key, value);
}

TInt CCentralRepositoryHandler::createKey(unsigned long key, const TReal& value)
{
    return iRepository->Create(key, value);
}

TInt CCentralRepositoryHandler::createKey(unsigned long key, const TDesC8& value)
{
    return iRepository->Create(key, value);
}

TInt CCentralRepositoryHandler::createKey(unsigned long key, const TDesC16& value)
{
    return iRepository->Create(key, value);
}

bool CCentralRepositoryHandler::handleStartMonitoring(const XQSettingsKey& key, XQSettingsManager::Type type, MSettingsHandlerObserver& observer, TInt& error)
{
    if (m_monitors.contains(key.key()))
    {
        error = KErrAlreadyExists;
        return false;
    }
    CCenRepMonitor* newMonitor = new CCenRepMonitor(*iRepository, key, type, observer);
    if (newMonitor)
    {
        m_monitors[key.key()] = newMonitor;
        error = newMonitor->StartMonitoring();
        return error == KErrNone;
    }
    error = KErrNoMemory;
    return false;
}

bool CCentralRepositoryHandler::handleStopMonitoring(const XQSettingsKey& key, TInt& error)
{
    if (!m_monitors.contains(key.key()))
    {
        error = KErrNotFound;
        return false;
    }
    const long int itemKey = key.key();
    CCenRepMonitor* monitor = m_monitors[itemKey];
    m_monitors.remove(itemKey);
    delete monitor;

    return error == KErrNone;
}

TInt CCentralRepositoryHandler::findKeyL(unsigned long partialKey, TUint32 mask, RArray<TUint32>& aFoundKeys)
{
    return iRepository->FindL(partialKey, mask, aFoundKeys);
}

TInt CCentralRepositoryHandler::findKeyL(unsigned long partialKey, TUint32 mask, const TInt& value, bool negative, RArray<TUint32>& aFoundKeys)
{
    return negative ? iRepository->FindNeqL(partialKey, mask, value, aFoundKeys) : iRepository->FindEqL(partialKey, mask, value, aFoundKeys);
}

TInt CCentralRepositoryHandler::findKeyL(unsigned long partialKey, TUint32 mask, const TReal& value, bool negative, RArray<TUint32>& aFoundKeys)
{
    return negative ? iRepository->FindNeqL(partialKey, mask, value, aFoundKeys) : iRepository->FindEqL(partialKey, mask, value, aFoundKeys);
}

TInt CCentralRepositoryHandler::findKeyL(unsigned long partialKey, TUint32 mask, const TDesC8& value, bool negative, RArray<TUint32>& aFoundKeys)
{
    return negative ? iRepository->FindNeqL(partialKey, mask, value, aFoundKeys) : iRepository->FindEqL(partialKey, mask, value, aFoundKeys);
}

TInt CCentralRepositoryHandler::findKeyL(unsigned long partialKey, TUint32 mask, const TDesC16& value, bool negative, RArray<TUint32>& aFoundKeys)
{
    return negative ? iRepository->FindNeqL(partialKey, mask, value, aFoundKeys) : iRepository->FindEqL(partialKey, mask, value, aFoundKeys);
}

#ifndef XQSETTINGSMANAGER_NO_TRANSACTIONS
TInt CCentralRepositoryHandler::startTransaction(CRepository::TTransactionMode transactionMode)
{
    return iRepository->StartTransaction(transactionMode);
}
#else
TInt CCentralRepositoryHandler::startTransaction(CRepository::TTransactionMode /*transactionMode*/)
{
    return KErrNotSupported;
}
#endif

TInt CCentralRepositoryHandler::commitTransaction()
{
    #ifndef XQSETTINGSMANAGER_NO_TRANSACTIONS
        TUint32 keyInfo;
        return iRepository->CommitTransaction(keyInfo);
    #else
        return KErrNotSupported;
    #endif
}

void CCentralRepositoryHandler::cancelTransaction()
{
    #ifndef XQSETTINGSMANAGER_NO_TRANSACTIONS
        iRepository->CancelTransaction();
    #endif
}

void CCentralRepositoryHandler::failTransaction()
{
    #ifndef XQSETTINGSMANAGER_NO_TRANSACTIONS
        iRepository->FailTransaction();
    #endif
}

TInt CCentralRepositoryHandler::transactionState() const
{
    #ifndef XQSETTINGSMANAGER_NO_TRANSACTIONS
        return iRepository->TransactionState();
    #else
        return KErrNotSupported;
    #endif
}

/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef ICONNECTIONPRESENCE_H
#define ICONNECTIONPRESENCE_H

#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

#include "maemo/interfaces/itypes.h"
#include "maemo/interfaces/iabstract.h"
#include "maemo/interfaces/iconnection.h"
#include "maemo/dbusproxy.h"

namespace DBus
{
    namespace Interfaces
    {
        class IConnectionPresence : public DBus::IAbstract
        {
            Q_OBJECT

        public:
            static inline const char *staticInterfaceName()
            {
                return "org.freedesktop.Telepathy.Connection.Interface.Presence";
            }

            IConnectionPresence(
                const QString& busName,
                const QString& objectPath,
                QObject* parent = 0
            );
            IConnectionPresence(
                const QDBusConnection& connection,
                const QString& busName,
                const QString& objectPath,
                QObject* parent = 0
            );
            IConnectionPresence(DBus::DBusProxy *proxy);
            explicit IConnectionPresence(const DBus::Interfaces::IConnection& mainInterface);
            IConnectionPresence(const DBus::Interfaces::IConnection& mainInterface, QObject* parent);

        public Q_SLOTS:
            inline QDBusPendingReply<> AddStatus(const QString& status, const QVariantMap& parameters)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(status) << QVariant::fromValue(parameters);
                return asyncCallWithArgumentList(QLatin1String("AddStatus"), argumentList);
            }
            inline QDBusPendingReply<> ClearStatus()
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                return asyncCall(QLatin1String("ClearStatus"));
            }
            inline QDBusPendingReply<DBus::Interfaces::ContactPresences> GetPresence(const DBus::Interfaces::UIntList& contacts)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<DBus::Interfaces::ContactPresences>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(contacts);
                return asyncCallWithArgumentList(QLatin1String("GetPresence"), argumentList);
            }
            inline QDBusPendingReply<DBus::Interfaces::StatusSpecMap> GetStatuses()
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<DBus::Interfaces::StatusSpecMap>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                return asyncCall(QLatin1String("GetStatuses"));
            }
            inline QDBusPendingReply<> RemoveStatus(const QString& status)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(status);
                return asyncCallWithArgumentList(QLatin1String("RemoveStatus"), argumentList);
            }
            inline QDBusPendingReply<> RequestPresence(const DBus::Interfaces::UIntList& contacts)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(contacts);
                return asyncCallWithArgumentList(QLatin1String("RequestPresence"), argumentList);
            }
            inline QDBusPendingReply<> SetLastActivityTime(uint time)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(time);
                return asyncCallWithArgumentList(QLatin1String("SetLastActivityTime"), argumentList);
            }
            inline QDBusPendingReply<> SetStatus(const DBus::Interfaces::MultipleStatusMap& statuses)
            {
                if (!invalidationReason().isEmpty()) {
                    return QDBusPendingReply<>(QDBusMessage::createError(
                        invalidationReason(),
                        invalidationMessage()
                    ));
                }
                QList<QVariant> argumentList;
                argumentList << QVariant::fromValue(statuses);
                return asyncCallWithArgumentList(QLatin1String("SetStatus"), argumentList);
            }

        Q_SIGNALS:
            void PresenceUpdate(const DBus::Interfaces::ContactPresences& presence);

        protected:
            virtual void invalidate(DBus::DBusProxy *, const QString &, const QString &);
        };
    }//Interfaces
}//DBus
Q_DECLARE_METATYPE(DBus::Interfaces::IConnectionPresence*)

#endif //ICONNECTIONPRESENCE_H
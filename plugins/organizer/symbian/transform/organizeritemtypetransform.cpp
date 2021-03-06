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

#include "organizeritemtypetransform.h"
#include <calrrule.h>

void OrganizerItemTypeTransform::modifyBaseSchemaDefinitions(QMap<QString, QMap<QString, QOrganizerItemDetailDefinition> > &schemaDefs) const
{
#ifndef AGENDA_EXT_SUPPORT
    // Note not supported on legacy platforms
    schemaDefs.remove(QOrganizerItemType::TypeNote);
#endif    
    // Journal is not supported on Symbian. Remove the type itself
    schemaDefs.remove(QOrganizerItemType::TypeJournal);
}

void OrganizerItemTypeTransform::transformToDetailL(const CCalEntry& entry, QOrganizerItem *item)
{
    CCalEntry::TType entryType = entry.EntryTypeL();
    QString itemType;

    if (entryType == CCalEntry::ETodo) {
        // Assume this is an occurrence if the recurrence id is set
        if (entry.RecurrenceIdL().TimeUtcL() != Time::NullTTime())
            itemType = QLatin1String(QOrganizerItemType::TypeTodoOccurrence);
        else
            itemType = QLatin1String(QOrganizerItemType::TypeTodo);
    }
    else if (entryType == CCalEntry::EEvent || entryType == CCalEntry::EAppt) {
        // Assume this is an occurrence if the recurrence id is set
        if (entry.RecurrenceIdL().TimeUtcL() != Time::NullTTime())
            itemType = QLatin1String(QOrganizerItemType::TypeEventOccurrence);
        else
            itemType = QLatin1String(QOrganizerItemType::TypeEvent);
    }
    else if (entryType == CCalEntry::EAnniv)
        itemType = QLatin1String(QOrganizerItemType::TypeEvent);
#ifdef AGENDA_EXT_SUPPORT
    else if (CCalEntry::ENote == entryType) {
        itemType = QLatin1String(QOrganizerItemType::TypeNote);
    }
#endif
    else
        User::Leave(KErrUnknown); // unknown type

    item->setType(itemType);
}

void OrganizerItemTypeTransform::transformToDetailL(const CCalInstance& instance, QOrganizerItem *itemOccurrence)
{
    QString itemType;
    CCalEntry &entry = instance.Entry();
    TCalRRule rrule;
    bool isRecurring = entry.GetRRuleL(rrule);
    bool isExceptional = entry.RecurrenceIdL().TimeUtcL() != Time::NullTTime();
    CCalEntry::TType entryType = entry.EntryTypeL();

    if (entryType == CCalEntry::ETodo) {
        if (isRecurring || isExceptional)
            itemType = QLatin1String(QOrganizerItemType::TypeTodoOccurrence);
        else
            itemType = QLatin1String(QOrganizerItemType::TypeTodo);
    } else if (entryType == CCalEntry::EEvent
        || entryType == CCalEntry::EAppt
        || entryType == CCalEntry::EAnniv) {
        if (isRecurring || isExceptional)
            itemType = QLatin1String(QOrganizerItemType::TypeEventOccurrence);
        else
            itemType = QLatin1String(QOrganizerItemType::TypeEvent);
    }
    else
        User::Leave(KErrUnknown); // unknown type

    itemOccurrence->setType(itemType);
}

void OrganizerItemTypeTransform::transformToEntryL(const QOrganizerItem& item, CCalEntry* entry)
{
    Q_UNUSED(item);
    Q_UNUSED(entry);
    // Not used. Entry type is already set when CCalEntry was created.
}

QString OrganizerItemTypeTransform::detailDefinitionName()
{
    return QOrganizerItemType::DefinitionName;    
}

CCalEntry::TType OrganizerItemTypeTransform::entryTypeL(const QOrganizerItem &item)
{
    QString itemType = item.type();
    CCalEntry::TType entryType(CCalEntry::EAppt);

    if (itemType == QOrganizerItemType::TypeTodo || itemType == QOrganizerItemType::TypeTodoOccurrence)
        entryType = CCalEntry::ETodo;
    else if (itemType == QOrganizerItemType::TypeEvent || itemType == QOrganizerItemType::TypeEventOccurrence)
        entryType = CCalEntry::EAppt;
#ifdef AGENDA_EXT_SUPPORT
    else if (QOrganizerItemType::TypeNote == itemType) {
        entryType = CCalEntry::ENote;
    }
#endif
    else
        User::Leave(KErrUnknown); // unknown type
        
        

    // TODO: CCalEntry::EEvent???
    // TODO: CCalEntry::EReminder
    // TODO: CCalEntry::EAnniv if itemType=event & category=anniversary

    return entryType;
}

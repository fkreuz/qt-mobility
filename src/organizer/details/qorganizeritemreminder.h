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


#ifndef QORGANIZERITEMREMINDER_H
#define QORGANIZERITEMREMINDER_H

#include <QString>
#include <QUrl>
#include <QList>
#include <QVariantList>

#include "qtorganizerglobal.h"
#include "qorganizeritemdetail.h"
#include "qorganizeritem.h"

QTM_BEGIN_NAMESPACE

/* Leaf class */
class Q_ORGANIZER_EXPORT QOrganizerItemReminder : public QOrganizerItemDetail
{
public:
#ifdef Q_QDOC
    static const QLatin1Constant DefinitionName;
    static const QLatin1Constant FieldSecondsBeforeStart;
    static const QLatin1Constant FieldRepetitionCount;
    static const QLatin1Constant FieldRepetitionDelay;
#else
    Q_DECLARE_CUSTOM_ORGANIZER_DETAIL(QOrganizerItemReminder, "Reminder")
    Q_DECLARE_LATIN1_CONSTANT(FieldSecondsBeforeStart, "SecondsBeforeStart");
    Q_DECLARE_LATIN1_CONSTANT(FieldRepetitionCount, "RepetitionCount");
    Q_DECLARE_LATIN1_CONSTANT(FieldRepetitionDelay, "RepetitionDelay");
#endif

    enum ReminderType {
        NoReminder = 0,
        VisualReminder,
        AudibleReminder,
        EmailReminder

        // other types of reminders?
        //ProcedureReminder,
        //TactileReminder,
    };

    ReminderType reminderType() const;

    void setSecondsBeforeStart(int seconds) {setValue(FieldSecondsBeforeStart, seconds);}
    int secondsBeforeStart() const {return value<int>(FieldSecondsBeforeStart);}

    // duration/repetition
    void setRepetition(int count, int delaySeconds) {setValue(FieldRepetitionCount, count); setValue(FieldRepetitionDelay, delaySeconds);}
    int repetitionDelay() const {return value<int>(FieldRepetitionDelay);}
    int repetitionCount() const {return value<int>(FieldRepetitionCount);}

protected:
    QOrganizerItemReminder(const char* definitionName) : QOrganizerItemDetail(definitionName) {}
    QOrganizerItemReminder(const QOrganizerItemDetail& detail, const char* definitionName) : QOrganizerItemDetail(detail, definitionName) {}
};

#define Q_DECLARE_CUSTOM_ORGANIZER_REMINDER_DETAIL(className, definitionNameString) \
    className() : QOrganizerItemReminder(DefinitionName.latin1()) {} \
    className(const QOrganizerItemDetail& field) : QOrganizerItemReminder(field, DefinitionName.latin1()) {} \
    className& operator=(const QOrganizerItemDetail& other) {assign(other, DefinitionName.latin1()); return *this;} \
    \
    Q_DECLARE_LATIN1_CONSTANT(DefinitionName, definitionNameString);

#define Q_IMPLEMENT_CUSTOM_ORGANIZER_REMINDER_DETAIL(className, definitionNameString) \
    Q_DEFINE_LATIN1_CONSTANT(className::DefinitionName, definitionNameString)

QTM_END_NAMESPACE

#endif


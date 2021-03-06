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
#include "cnttransformemail.h"

QList<CContactItemField *> CntTransformEmail::transformDetailL(const QContactDetail &detail)
{
    if(detail.definitionName() != QContactEmailAddress::DefinitionName)
        User::Leave(KErrArgument);

    QList<CContactItemField *> fieldList;

	//cast to email
	const QContactEmailAddress &email(static_cast<const QContactEmailAddress&>(detail));

	//create new field with contexts
    transformToTextFieldL(email, fieldList, email.emailAddress(), KUidContactFieldEMail, KUidContactFieldVCardMapEMAILINTERNET, true);

	return fieldList;
}

QContactDetail *CntTransformEmail::transformItemField(const CContactItemField& field, const QContact &contact)
{
	Q_UNUSED(contact);

	QContactEmailAddress *email = new QContactEmailAddress();

	CContactTextField* storage = field.TextStorage();
	QString emailAddress = QString::fromUtf16(storage->Text().Ptr(), storage->Text().Length());

	email->setEmailAddress(emailAddress);

	for (int i = 0; i < field.ContentType().FieldTypeCount(); i++)
	{
		setContexts(field.ContentType().FieldType(i), *email);
	}

	return email;
}

bool CntTransformEmail::supportsDetail(QString detailName) const
{
    bool ret = false;
    if (detailName == QContactEmailAddress::DefinitionName) {
        ret = true;
    }
    return ret;
}

QList<TUid> CntTransformEmail::supportedFields() const
{
    return QList<TUid>()
        << KUidContactFieldEMail;
}

QList<TUid> CntTransformEmail::supportedSortingFieldTypes(QString detailFieldName) const
{
    QList<TUid> uids;
    if( detailFieldName == QContactEmailAddress::FieldEmailAddress )
        uids << KUidContactFieldEMail;
    return uids;
}

/*!
 * Checks whether the subtype is supported
 *
 * \a subType The subtype to be checked
 * \return True if this subtype is supported
 */
bool CntTransformEmail::supportsSubType(const QString& subType) const
{
    Q_UNUSED(subType);
    return false;
}

/*!
 * Returns the filed id corresponding to a field
 *
 * \a fieldName The name of the supported field
 * \return fieldId for the fieldName, 0  if not supported
 */
quint32 CntTransformEmail::getIdForField(const QString& fieldName) const
{
    if (QContactEmailAddress::FieldEmailAddress == fieldName)
        return KUidContactFieldEMail.iUid;
    else
        return 0;
}

/*!
 * Modifies the detail definitions. The default detail definitions are
 * queried from QContactManagerEngine::schemaDefinitions and then modified
 * with this function in the transform leaf classes.
 *
 * \a definitions The detail definitions to modify.
 * \a contactType The contact type the definitions apply for.
 */
void CntTransformEmail::detailDefinitions(QMap<QString, QContactDetailDefinition> &definitions, const QString& contactType) const
{
    Q_UNUSED(contactType);
    
    if(definitions.contains(QContactEmailAddress::DefinitionName)) {
        QContactDetailDefinition d = definitions.value(QContactEmailAddress::DefinitionName);
        QMap<QString, QContactDetailFieldDefinition> fields = d.fields();
        
        // Don't support "ContextOther"
        QContactDetailFieldDefinition f;
        f.setDataType(QVariant::StringList);
        f.setAllowableValues(QVariantList() 
            << QLatin1String(QContactDetail::ContextHome) 
            << QLatin1String(QContactDetail::ContextWork));
        fields[QContactDetail::FieldContext] = f;
        d.setFields(fields);

        // Replace original definitions
        definitions.insert(d.name(), d);
    }
}

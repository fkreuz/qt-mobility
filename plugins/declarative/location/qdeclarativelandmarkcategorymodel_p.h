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
***************************************************************************/

#ifndef QDECLARATIVELANDMARKCATEGORYMODEL_P_H
#define QDECLARATIVELANDMARKCATEGORYMODEL_P_H

#include <qdeclarativelandmark_p.h>
#include <qdeclarativelandmarkmodel_p.h>
#include <qdeclarativelandmarkcategory_p.h>
#include <qdeclarativelandmarkfilters_p.h>

#include <qlandmarkcategory.h>
#include <qlandmarkmanager.h>
#include <qlandmarkabstractrequest.h>

#include <QtDeclarative/qdeclarative.h>
#include <QAbstractListModel>

QTM_BEGIN_NAMESPACE

class QDeclarativeLandmarkCategoryModel: public QDeclarativeLandmarkAbstractModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QDeclarativeLandmark* landmark READ landmark WRITE setLandmark NOTIFY landmarkChanged)
    Q_PROPERTY(QDeclarativeListProperty<QDeclarativeLandmarkCategory> categories READ categories NOTIFY categoriesChanged)

public:
    explicit QDeclarativeLandmarkCategoryModel(QObject* parent = 0);
    virtual ~QDeclarativeLandmarkCategoryModel();

    // From QAbstractListModel
    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    // Roles for exposing data via model
    enum {
        CategoryRole = Qt::UserRole + 500
    };
    QDeclarativeLandmark* landmark() const;
    void setLandmark(QDeclarativeLandmark* landmark);
    int count() const;

    // For testing purpose to access ordered data
    QList<QLandmarkCategory> categoryList();
    QDeclarativeListProperty<QDeclarativeLandmarkCategory> categories();
    static void categories_append(QDeclarativeListProperty<QDeclarativeLandmarkCategory>* prop, QDeclarativeLandmarkCategory* category);
    static int categories_count(QDeclarativeListProperty<QDeclarativeLandmarkCategory>* prop);
    static QDeclarativeLandmarkCategory* categories_at(QDeclarativeListProperty<QDeclarativeLandmarkCategory>* prop, int index);
    static void categories_clear(QDeclarativeListProperty<QDeclarativeLandmarkCategory>* prop);

signals:
    void countChanged();
    void landmarkChanged();
    void categoriesChanged();

private slots:
    void cancelUpdate();
    void fetchRequestStateChanged(QLandmarkAbstractRequest::State state);

private:
    Q_INVOKABLE void startUpdate();
    void convertCategoriesToDeclarative();
    void setFetchRange();
    void setFetchFilters();
    void setFetchOrder();

private:
    QLandmarkAbstractRequest* m_fetchRequest;
    // Landmark list received from platform
    QList<QLandmarkCategory> m_categories;
    // Same category list, but as declarative classes
    QMap<QString, QDeclarativeLandmarkCategory*> m_categoryMap;
    QDeclarativeLandmark* m_landmark;
};

QTM_END_NAMESPACE
QML_DECLARE_TYPE(QTM_PREPEND_NAMESPACE(QDeclarativeLandmarkCategoryModel));

#endif // QDECLARATIVELANDMARKCATEGORYMODEL_P_H

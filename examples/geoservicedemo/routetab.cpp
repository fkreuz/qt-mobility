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

#include "routetab.h"
#include "routepresenter.h"

#include <QTreeWidget>
#include <QLineEdit>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <qgeorouterequest.h>

RouteTab::RouteTab(QWidget *parent) :
        QWidget(parent),
        m_routingManager(0)
{
    QLabel *source = new QLabel(tr("Source (lat,long):"));
    m_srcLong = new QLineEdit("9.986572");
    m_srcLong->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_srcLat = new QLineEdit("53.546836");
    m_srcLat->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QLabel *destination = new QLabel(tr("Destination (lat,long):"));
    m_destLong = new QLineEdit("9.733887");
    m_destLong->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_destLat = new QLineEdit("52.382306");
    m_destLat->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    requestBtn = new QPushButton(tr("Request Route"));
    requestBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    requestBtn->setDisabled(true);
    QObject::connect(requestBtn, SIGNAL(clicked(bool)),
                     this, SLOT(on_btnRequest_clicked()));
    updateBtn = new QPushButton(tr("Update Route"));
    updateBtn->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
    updateBtn->setDisabled(true);
    QObject::connect(updateBtn, SIGNAL(clicked(bool)),
                     this, SLOT(on_btnUpdate_clicked()));
    m_resultTree = new QTreeWidget();
    QStringList labels;
    labels << tr("Elements") << tr("Value");
    m_resultTree->setHeaderLabels(labels);

    QHBoxLayout *firstrow = new QHBoxLayout;
    firstrow->setSpacing(0);
    firstrow->setContentsMargins(0, 0, 0, 0);
    firstrow->addWidget(source);
    firstrow->addWidget(m_srcLat);
    firstrow->addWidget(m_srcLong);
    firstrow->addWidget(requestBtn);

    QHBoxLayout *secondrow = new QHBoxLayout;
    secondrow->setSpacing(0);
    secondrow->setContentsMargins(0, 0, 0, 0);
    secondrow->addWidget(destination);
    secondrow->addWidget(m_destLat);
    secondrow->addWidget(m_destLong);
    secondrow->addWidget(updateBtn);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(2, 1, 2, 1);
    mainLayout->addLayout(firstrow);
    mainLayout->addLayout(secondrow);
    mainLayout->addWidget(m_resultTree);
    setLayout(mainLayout);
}

RouteTab::~RouteTab()
{
}

void RouteTab::initialize(QGeoRoutingManager *routingManager)
{
    m_resultTree->clear();
    m_routingManager = routingManager;
    if (m_routingManager) {
        QObject::connect(m_routingManager, SIGNAL(finished(QGeoRouteReply*)), this,
                         SLOT(replyFinished(QGeoRouteReply*)));
        QObject::connect(m_routingManager,
                         SIGNAL(error(QGeoRouteReply*, QGeoRouteReply::Error, QString)), this,
                         SLOT(resultsError(QGeoRouteReply*, QGeoRouteReply::Error, QString)));
        requestBtn->setDisabled(false);
    }
    else
        requestBtn->setDisabled(true);
}

void RouteTab::on_btnRequest_clicked()
{
    if (m_routingManager) {
        m_resultTree->clear();
        QTreeWidgetItem* top = new QTreeWidgetItem(m_resultTree);
        top->setText(0, tr("Route"));
        top->setText(1, tr("Calculating"));
        updateBtn->setDisabled(true);

        QGeoCoordinate src(m_srcLat->text().toDouble(), m_srcLong->text().toDouble());
        QGeoCoordinate dst(m_destLat->text().toDouble(), m_destLong->text().toDouble());

        QGeoRouteRequest request(src, dst);
        if((m_routingManager->supportedSegmentDetails() & QGeoRouteRequest::BasicSegmentData) != 0)
            request.setSegmentDetail(QGeoRouteRequest::BasicSegmentData);
        if((m_routingManager->supportedInstructionDetails() & QGeoRouteRequest::BasicInstructions) != 0)
            request.setInstructionDetail(QGeoRouteRequest::BasicInstructions);

        m_routingManager->calculateRoute(request);
    } else {
        QMessageBox::warning(this, tr("Routing"), tr("No routing manager available."));
    }
}

void RouteTab::on_btnUpdate_clicked()
{
    if (m_routingManager && routes.count()>0) {
        m_resultTree->clear();
        QTreeWidgetItem* top = new QTreeWidgetItem(m_resultTree);
        top->setText(0, tr("Route"));
        top->setText(1, tr("Updating"));
        updateBtn->setDisabled(true);
        QGeoCoordinate src(m_srcLat->text().toDouble(), m_srcLong->text().toDouble());
        m_routingManager->updateRoute(routes[0],src);
    } else {
        QMessageBox::warning(this, tr("Routing"), tr("Route update not available."));
    }
}

void RouteTab::replyFinished(QGeoRouteReply* reply)
{
    if(reply->error() == QGeoRouteReply::NoError) {
        routes = reply->routes();
        RoutePresenter presenter(m_resultTree, reply);
        presenter.show();
        reply->deleteLater();
        if(routes.count()>0 && m_routingManager->supportsRouteUpdates())
            updateBtn->setDisabled(false);
        }
}

void RouteTab::resultsError(QGeoRouteReply* reply, QGeoRouteReply::Error error, QString errorString)
{
    Q_UNUSED(error)

    QTreeWidgetItem* top = new QTreeWidgetItem(m_resultTree);
    top->setText(0, tr("Error"));
    top->setText(1, errorString);
    reply->deleteLater();
}
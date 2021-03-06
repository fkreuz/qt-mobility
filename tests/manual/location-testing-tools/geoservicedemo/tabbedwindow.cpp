/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "tabbedwindow.h"
#include "servicestab.h"

#include <QApplication>
#include <QTabWidget>
#include <QAction>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QTimer>
#include <QMessageBox>
#include <QNetworkProxyFactory>
#include <qnetworkconfigmanager.h>

TabbedWindow::TabbedWindow(QWidget *parent)
        : QMainWindow(parent), m_serviceProvider(NULL)
{
    setWindowTitle(tr("Geo Service Demo"));

    m_servicesTab = new ServicesTab();

    QObject::connect(m_servicesTab, SIGNAL(serviceProviderChanged(QString)), this,
                     SLOT(setProvider(QString)), Qt::QueuedConnection);

    m_geocodingTab = new GeocodingTab();
    m_reverseTab = new ReverseGeocodingTab();
    m_routingTab = new RouteTab();
    m_directionsTab = new DirectionsTab();

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(m_servicesTab, tr("Service Providers"));
    m_tabWidget->addTab(m_routingTab, tr("Route"));
    m_tabWidget->addTab(m_geocodingTab, tr("Geocoding"));
    m_tabWidget->addTab(m_reverseTab, tr("Reverse Geocoding"));
    m_tabWidget->addTab(m_directionsTab, tr("Directions"));

    setCentralWidget(m_tabWidget);

    // setup exit menu for devices

#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM) || defined(Q_WS_MAEMO_5) || defined(Q_WS_MAEMO_6)
    QAction* exitAction = new QAction(tr("Exit"), this);
    menuBar()->addAction(exitAction);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
#endif

    // Set Internet Access Point
    QNetworkConfigurationManager manager;
    const bool canStartIAP = (manager.capabilities()
                              & QNetworkConfigurationManager::CanStartAndStopInterfaces);
    // Is there default access point, use it
    QNetworkConfiguration cfg = manager.defaultConfiguration();
    if (!cfg.isValid() || (!canStartIAP && cfg.state() != QNetworkConfiguration::Active)) {
        QMessageBox::information(this, tr("Geo Service Demo"), tr(
                                     "Available Access Points not found."));
        return;
    }

    m_session = new QNetworkSession(cfg, this);
    connect(m_session, SIGNAL(opened()), this, SLOT(networkSessionOpened()));
    connect(m_session,
            SIGNAL(error(QNetworkSession::SessionError)),
            this,
            SLOT(error(QNetworkSession::SessionError)));

    m_session->open();
    resize(640, 480);
}

TabbedWindow::~TabbedWindow()
{
    delete m_serviceProvider;
}

void TabbedWindow::networkSessionOpened()
{
    QNetworkProxyFactory::setUseSystemConfiguration(true);
    QTimer::singleShot(0, m_servicesTab, SLOT(initialize()));
}

void TabbedWindow::error(QNetworkSession::SessionError error)
{
    if (error == QNetworkSession::UnknownSessionError) {
        if (m_session->state() == QNetworkSession::Connecting) {
            QMessageBox msgBox(qobject_cast<QWidget *>(parent()));
            msgBox.setText("This application requires network access to function.");
            msgBox.setInformativeText("Press Cancel to quit the application.");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Retry);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Retry) {
                QTimer::singleShot(0, m_session, SLOT(open()));
            } else if (ret == QMessageBox::Cancel) {
                close();
            }
        }
    } else if (error == QNetworkSession::SessionAbortedError) {
        QMessageBox msgBox(qobject_cast<QWidget *>(parent()));
        msgBox.setText("Out of range of network.");
        msgBox.setInformativeText("Move back into range and press Retry, or press Cancel to quit the application.");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStandardButtons(QMessageBox::Retry | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Retry);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Retry) {
            QTimer::singleShot(0, m_session, SLOT(open()));
        } else if (ret == QMessageBox::Cancel) {
            close();
        }
    }
}

void TabbedWindow::setProvider(QString providerId)
{
    if (m_serviceProvider)
        delete m_serviceProvider ;
    m_serviceProvider = new QGeoServiceProvider(providerId);
    if (m_serviceProvider->error() != QGeoServiceProvider::NoError) {
        QMessageBox::information(this, tr("Geo Service Demo"), tr(
                                     "Unable to find the %1 geoservices plugin.").arg(providerId));
        QTimer::singleShot(0, qApp, SLOT(quit()));
        return;
    }

    m_geocodingTab->initialize(m_serviceProvider->searchManager());
    m_reverseTab->initialize(m_serviceProvider->searchManager());
    m_routingTab->initialize(m_serviceProvider->routingManager());
    m_directionsTab->initialize(m_serviceProvider->routingManager(), m_serviceProvider->searchManager());
}


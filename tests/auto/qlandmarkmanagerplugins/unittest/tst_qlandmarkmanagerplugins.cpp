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

//TESTED_COMPONENT=src/location

#include <QtTest/QtTest>
#include <QApplication>

#include <qlandmarkmanager.h>

QTM_USE_NAMESPACE

class tst_QLandmarkManagerPlugins : public QObject
{
Q_OBJECT
public:
    tst_QLandmarkManagerPlugins();
    virtual ~tst_QLandmarkManagerPlugins();

public slots:
    void init();
    void cleanup();
private slots:
    void testDummy();
};

tst_QLandmarkManagerPlugins::tst_QLandmarkManagerPlugins()
{
}

tst_QLandmarkManagerPlugins::~tst_QLandmarkManagerPlugins()
{
}

void tst_QLandmarkManagerPlugins::init()
{
}

void tst_QLandmarkManagerPlugins::cleanup()
{
}

#ifndef Q_OS_SYMBIAN
Q_IMPORT_PLUGIN(landmarks_testdummystatic)
#endif

void tst_QLandmarkManagerPlugins::testDummy()
{
#ifndef Q_OS_SYMBIAN
    QVERIFY(QLandmarkManager::availableManagers().contains("LandmarkManagerFactoryDummyStatic"));
    QLandmarkManager lm1("LandmarkManagerFactoryDummyStatic");
    //Note the tests below is intended to "fail"
    //the dummy plugin is mocked to return a LockedError. we are just testing if the plugin loads
    QVERIFY(lm1.error() == QLandmarkManager::LockedError);
    QVERIFY(lm1.managerName() == "");
#endif
    QVERIFY(QLandmarkManager::availableManagers().contains("LandmarkManagerFactoryDummyDynamic"));
#if !defined (Q_OS_SYMBIAN) && !defined(Q_OS_WINCE) && !defined(Q_WS_MAEMO_5) && !defined(Q_WS_MAEMO_6)
    QVERIFY(QLandmarkManager::availableManagers().contains("com.nokia.qt.landmarks.engines.sqlite"));
    QLandmarkManager lm("com.nokia.qt.landmarks.engines.sqlite");
    QVERIFY(lm.managerName() == "com.nokia.qt.landmarks.engines.sqlite");
#endif
}

QTEST_MAIN(tst_QLandmarkManagerPlugins)
#include "tst_qlandmarkmanagerplugins.moc"

/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include "qnmeapositioninfosourceproxyfactory.h"
#include "../testqgeopositioninfosource_p.h"
#include "../qlocationtestutils_p.h"

#include <qnmeapositioninfosource.h>

#include <QTest>
#include <QDir>
#include <QDebug>
#include <QBuffer>
#include <QSignalSpy>
#include <QMetaType>
#include <QFile>
#include <QTemporaryFile>
#include <QHash>
#include <QTimer>

QTM_USE_NAMESPACE
Q_DECLARE_METATYPE(QNmeaPositionInfoSource::UpdateMode)
Q_DECLARE_METATYPE(QGeoPositionInfo)
Q_DECLARE_METATYPE(QList<QDateTime>)

class tst_QNmeaPositionInfoSource : public QObject
{
    Q_OBJECT

public:
    enum UpdateTriggerMethod
    {
        StartUpdatesMethod,
        RequestUpdatesMethod
    };

    tst_QNmeaPositionInfoSource(QNmeaPositionInfoSource::UpdateMode mode, QObject *parent = 0);

private:
    QList<QDateTime> createDateTimes(int count) const
    {
        QList<QDateTime> dateTimes;
        QDateTime dt = QDateTime::currentDateTime().toUTC();
        int interval = 100;
        for (int i=0; i<count; i++) {
            dateTimes << dt.addMSecs(interval);
            interval += 100;
        }
        return dateTimes;
    }

private slots:
    void initTestCase();

    void constructor();

    void supportedPositioningMethods();

    void minimumUpdateInterval();

    void setUpdateInterval_delayedUpdate();

    void lastKnownPosition();

    void beginWithBufferedData();
    void beginWithBufferedData_data();

    void startUpdates();
    void startUpdates_data();

    void startUpdates_expectLatestUpdateOnly();

    void startUpdates_waitForValidDateTime();
    void startUpdates_waitForValidDateTime_data();

    void requestUpdate_waitForValidDateTime();
    void requestUpdate_waitForValidDateTime_data();

    void requestUpdate();
    void requestUpdate_after_start();

    void testWithBadNmea();
    void testWithBadNmea_data();

private:
    QNmeaPositionInfoSource::UpdateMode m_mode;
};

Q_DECLARE_METATYPE(tst_QNmeaPositionInfoSource::UpdateTriggerMethod)


tst_QNmeaPositionInfoSource::tst_QNmeaPositionInfoSource(QNmeaPositionInfoSource::UpdateMode mode, QObject *parent)
    : QObject(parent),
      m_mode(mode)
{
}

void tst_QNmeaPositionInfoSource::initTestCase()
{
    qRegisterMetaType<QGeoPositionInfo>();
    qRegisterMetaType<QNmeaPositionInfoSource::UpdateMode>();
}

void tst_QNmeaPositionInfoSource::constructor()
{
    QObject o;
    QNmeaPositionInfoSource source(m_mode, &o);
    QCOMPARE(source.updateMode(), m_mode);
    QCOMPARE(source.parent(), &o);
}

void tst_QNmeaPositionInfoSource::supportedPositioningMethods()
{
    QNmeaPositionInfoSource source(m_mode);
    QCOMPARE(source.supportedPositioningMethods(), QNmeaPositionInfoSource::SatellitePositioningMethods);
}

void tst_QNmeaPositionInfoSource::minimumUpdateInterval()
{
    QNmeaPositionInfoSource source(m_mode);
    QCOMPARE(source.minimumUpdateInterval(), 100);
}

void tst_QNmeaPositionInfoSource::setUpdateInterval_delayedUpdate()
{
    // If an update interval is set, and an update is not available at a
    // particular interval, the source should emit the next update as soon
    // as it becomes available

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->setUpdateInterval(500);
    proxy->source()->startUpdates();

    QTest::qWait(600);
    QDateTime now = QDateTime::currentDateTime();
    proxy->feedUpdate(now);
    QTRY_COMPARE(spyUpdate.count(), 1);

    // should have gotten the update immediately, and not have needed to
    // wait until the next interval
    QVERIFY(now.time().msecsTo(QDateTime::currentDateTime().time()) < 200);
}

void tst_QNmeaPositionInfoSource::lastKnownPosition()
{
    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QCOMPARE(proxy->source()->lastKnownPosition(), QGeoPositionInfo());

    // source may need requestUpdate() or startUpdates() to be called to
    // trigger reading of data channel
    QSignalSpy spyTimeout(proxy->source(), SIGNAL(requestTimeout()));
    proxy->source()->requestUpdate(proxy->source()->minimumUpdateInterval());
    QTRY_COMPARE(spyTimeout.count(), 1);

    // If an update is received and startUpdates() or requestUpdate() hasn't
    // been called, it should still be available through lastKnownPosition()
    QDateTime dt = QDateTime::currentDateTime().toUTC();
    proxy->feedUpdate(dt);
    QTRY_COMPARE(proxy->source()->lastKnownPosition().dateTime(), dt);

    QList<QDateTime> dateTimes = createDateTimes(5);
    for (int i=0; i<dateTimes.count(); i++) {
        proxy->source()->requestUpdate();
        proxy->feedUpdate(dateTimes[i]);
        QTRY_COMPARE(proxy->source()->lastKnownPosition().dateTime(), dateTimes[i]);
    }

    proxy->source()->startUpdates();
    dateTimes = createDateTimes(5);
    for (int i=0; i<dateTimes.count(); i++) {
        proxy->feedUpdate(dateTimes[i]);
        QTRY_COMPARE(proxy->source()->lastKnownPosition().dateTime(), dateTimes[i]);
    }
}

void tst_QNmeaPositionInfoSource::beginWithBufferedData()
{
    // In SimulationMode, data stored in the QIODevice is read when
    // startUpdates() or requestUpdate() is called.
    // In RealTimeMode, all existing data in the QIODevice is ignored -
    // only new data will be read.

    QFETCH(QList<QDateTime>, dateTimes);
    QFETCH(UpdateTriggerMethod, trigger);

    QByteArray bytes;
    for (int i=0; i<dateTimes.count(); i++)
        bytes += QLocationTestUtils::createRmcSentence(dateTimes[i]).toLatin1();
    QBuffer buffer;
    buffer.setData(bytes);

    QNmeaPositionInfoSource source(m_mode);
    QSignalSpy spy(&source, SIGNAL(positionUpdated(QGeoPositionInfo)));
    source.setDevice(&buffer);

    if (trigger == StartUpdatesMethod)
        source.startUpdates();
    else if (trigger == RequestUpdatesMethod)
        source.requestUpdate();

    if (m_mode == QNmeaPositionInfoSource::RealTimeMode) {
        QTest::qWait(300);
        QCOMPARE(spy.count(), 0);
    } else {
        if (trigger == StartUpdatesMethod) {
            QTRY_COMPARE(spy.count(), dateTimes.count());
            for (int i=0; i<dateTimes.count(); i++)
                QCOMPARE(spy.at(i).at(0).value<QGeoPositionInfo>().dateTime(), dateTimes[i]);
        } else if (trigger == RequestUpdatesMethod) {
            QTRY_COMPARE(spy.count(), 1);
            QCOMPARE(spy.at(0).at(0).value<QGeoPositionInfo>().dateTime(), dateTimes.first());
        }
    }
}

void tst_QNmeaPositionInfoSource::beginWithBufferedData_data()
{
    QTest::addColumn<QList<QDateTime> >("dateTimes");
    QTest::addColumn<UpdateTriggerMethod>("trigger");

    QList<QDateTime> dateTimes;
    dateTimes << QDateTime::currentDateTime().toUTC();

    QTest::newRow("startUpdates(), 1 update in buffer") << dateTimes << StartUpdatesMethod;
    QTest::newRow("requestUpdate(), 1 update in buffer") << dateTimes << RequestUpdatesMethod;

    for (int i=1; i<3; i++)
        dateTimes << dateTimes[0].addDays(i);
    QTest::newRow("startUpdates(), multiple updates in buffer") << dateTimes << StartUpdatesMethod;
    QTest::newRow("requestUpdate(), multiple updates in buffer") << dateTimes << RequestUpdatesMethod;
}

void tst_QNmeaPositionInfoSource::startUpdates()
{
    QFETCH(QList<QDateTime>, dateTimes);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->startUpdates();

    for (int i=0; i<dateTimes.count(); i++)
        proxy->feedUpdate(dateTimes[i]);
    QTRY_COMPARE(spyUpdate.count(), dateTimes.count());
}

void tst_QNmeaPositionInfoSource::startUpdates_data()
{
    QTest::addColumn<QList<QDateTime> >("dateTimes");

    QTest::newRow("1 update") << createDateTimes(1);
    QTest::newRow("2 updates") << createDateTimes(2);
    QTest::newRow("10 updates") << createDateTimes(10);
}

void tst_QNmeaPositionInfoSource::startUpdates_expectLatestUpdateOnly()
{
    // If startUpdates() is called and an interval has been set, if multiple'
    // updates are in the buffer, only the latest update should be emitted

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->setUpdateInterval(500);
    proxy->source()->startUpdates();

    QList<QDateTime> dateTimes = createDateTimes(3);
    for (int i=0; i<dateTimes.count(); i++)
        proxy->feedUpdate(dateTimes[i]);

    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().dateTime(), dateTimes.last());
}

void tst_QNmeaPositionInfoSource::startUpdates_waitForValidDateTime()
{
    // Tests that the class does not emit an update until it receives a
    // sentences with a valid date *and* time. All sentences before this
    // should be ignored, and any sentences received after this that do
    // not have a date should use the known date.

    QFETCH(QByteArray, bytes);
    QFETCH(QList<QDateTime>, dateTimes);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spy(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->startUpdates();

    proxy->feedBytes(bytes);
    QTRY_COMPARE(spy.count(), dateTimes.count());

    for (int i=0; i<spy.count(); i++)
        QCOMPARE(spy[i][0].value<QGeoPositionInfo>().dateTime(), dateTimes[i]);
}

void tst_QNmeaPositionInfoSource::startUpdates_waitForValidDateTime_data()
{
    QTest::addColumn<QByteArray>("bytes");
    QTest::addColumn<QList<QDateTime> >("dateTimes");

    QDateTime dt = QDateTime::currentDateTime().toUTC();
    QByteArray bytes;

    // should only receive RMC sentence and the GGA sentence *after* it
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(1).time()).toLatin1();
    bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(2)).toLatin1();
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(3).time()).toLatin1();
    QTest::newRow("Feed GGA,RMC,GGA; expect RMC, second GGA only")
            << bytes << (QList<QDateTime>() << dt.addSecs(2) << dt.addSecs(3));

    // should not receive ZDA (has no coordinates) but should get the GGA
    // sentence after it since it got the date/time from ZDA
    bytes.clear();
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(1).time()).toLatin1();
    bytes += QLocationTestUtils::createZdaSentence(dt.addSecs(2)).toLatin1();
    bytes += QLocationTestUtils::createGgaSentence(dt.addSecs(3).time()).toLatin1();
    QTest::newRow("Feed GGA,ZDA,GGA; expect second GGA only")
            << bytes << (QList<QDateTime>() << dt.addSecs(3));

    if (m_mode == QNmeaPositionInfoSource::SimulationMode) {
        // In sim m_mode, should ignore sentence with a date/time before the known date/time
        // (in real time m_mode, everything is passed on regardless)
        bytes.clear();
        bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(1)).toLatin1();
        bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(-2)).toLatin1();
        bytes += QLocationTestUtils::createRmcSentence(dt.addSecs(2)).toLatin1();
        QTest::newRow("Feed good RMC, RMC with bad date/time, good RMC; expect first and third RMC only")
                << bytes << (QList<QDateTime>() << dt.addSecs(1) << dt.addSecs(2));
    }
}

void tst_QNmeaPositionInfoSource::requestUpdate_waitForValidDateTime()
{
    QFETCH(QByteArray, bytes);
    QFETCH(QList<QDateTime>, dateTimes);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spy(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    proxy->source()->requestUpdate();

    proxy->feedBytes(bytes);
    QTRY_COMPARE(spy.count(), 1);
    QCOMPARE(spy[0][0].value<QGeoPositionInfo>().dateTime(), dateTimes[0]);
}

void tst_QNmeaPositionInfoSource::requestUpdate_waitForValidDateTime_data()
{
    startUpdates_waitForValidDateTime_data();
}

void tst_QNmeaPositionInfoSource::requestUpdate()
{
    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    QSignalSpy spyTimeout(proxy->source(), SIGNAL(requestTimeout()));
    QDateTime dt;

    proxy->source()->requestUpdate(100);
    QTRY_COMPARE(spyTimeout.count(), 1);
    spyTimeout.clear();

    dt = QDateTime::currentDateTime().toUTC();
    proxy->feedUpdate(dt);
    proxy->source()->requestUpdate();
    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().dateTime(), dt);
    QCOMPARE(spyTimeout.count(), 0);
    spyUpdate.clear();

    // delay the update and expect it to be emitted after 300ms
    dt = QDateTime::currentDateTime().toUTC();
    proxy->source()->requestUpdate(1000);
    QTest::qWait(300);
    proxy->feedUpdate(dt);
    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().dateTime(), dt);
    QCOMPARE(spyTimeout.count(), 0);
    spyUpdate.clear();

    // delay the update and expect requestTimeout() to be emitted
    dt = QDateTime::currentDateTime().toUTC();
    proxy->source()->requestUpdate(500);
    QTest::qWait(1000);
    proxy->feedUpdate(dt);
    QCOMPARE(spyTimeout.count(), 1);
    QCOMPARE(spyUpdate.count(), 0);
    spyUpdate.clear();
}

void tst_QNmeaPositionInfoSource::requestUpdate_after_start()
{
    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spyUpdate(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    QSignalSpy spyTimeout(proxy->source(), SIGNAL(requestTimeout()));

    // Start updates with 500ms interval and requestUpdate() with 100ms
    // timeout. Feed an update, and it should be emitted immediately due to
    // the requestUpdate(). The update should not be emitted again after that
    // (i.e. the startUpdates() interval should not cause it to be re-emitted).

    QDateTime dt = QDateTime::currentDateTime().toUTC();
    proxy->source()->setUpdateInterval(500);
    proxy->source()->startUpdates();
    proxy->source()->requestUpdate(100);
    proxy->feedUpdate(dt);
    QTRY_COMPARE(spyUpdate.count(), 1);
    QCOMPARE(spyUpdate[0][0].value<QGeoPositionInfo>().dateTime(), dt);
    QCOMPARE(spyTimeout.count(), 0);
    spyUpdate.clear();

    // Update has been emitted for requestUpdate(), shouldn't be emitted for startUpdates()
    QTest::qWait(1000);
    QCOMPARE(spyUpdate.count(), 0);
}

void tst_QNmeaPositionInfoSource::testWithBadNmea()
{
    QFETCH(QByteArray, bytes);
    QFETCH(QList<QDateTime>, dateTimes);
    QFETCH(UpdateTriggerMethod, trigger);

    QNmeaPositionInfoSource source(m_mode);
    QNmeaPositionInfoSourceProxyFactory factory;
    QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(factory.createProxy(&source));

    QSignalSpy spy(proxy->source(), SIGNAL(positionUpdated(QGeoPositionInfo)));
    if (trigger == StartUpdatesMethod)
        proxy->source()->startUpdates();
    else
        proxy->source()->requestUpdate();

    proxy->feedBytes(bytes);
    QTRY_COMPARE(spy.count(), dateTimes.count());
    for (int i=0; i<dateTimes.count(); i++)
        QCOMPARE(spy[i][0].value<QGeoPositionInfo>().dateTime(), dateTimes[i]);
}

void tst_QNmeaPositionInfoSource::testWithBadNmea_data()
{
    QTest::addColumn<QByteArray>("bytes");
    QTest::addColumn<QList<QDateTime> >("dateTimes");
    QTest::addColumn<UpdateTriggerMethod>("trigger");

    QDateTime firstDateTime = QDateTime::currentDateTime().toUTC();
    QByteArray bad = QLocationTestUtils::createRmcSentence(firstDateTime.addSecs(1)).toLatin1();
    bad = bad.mid(bad.length()/2);
    QDateTime lastDateTime = firstDateTime.addSecs(2);

    QByteArray bytes;
    bytes += QLocationTestUtils::createRmcSentence(firstDateTime).toLatin1();
    bytes += bad;
    bytes += QLocationTestUtils::createRmcSentence(lastDateTime).toLatin1();
    QTest::newRow("requestUpdate(), bad second sentence") << bytes
            << (QList<QDateTime>() << firstDateTime) << RequestUpdatesMethod;
    QTest::newRow("startUpdates(), bad second sentence") << bytes
            << (QList<QDateTime>() << firstDateTime << lastDateTime) << StartUpdatesMethod;
}


class tst_QNmeaPositionInfoSource_RealTime : public tst_QNmeaPositionInfoSource
{
    Q_OBJECT
public:
    tst_QNmeaPositionInfoSource_RealTime()
        : tst_QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode) {}
};

class tst_QNmeaPositionInfoSource_Simulation : public tst_QNmeaPositionInfoSource
{
    Q_OBJECT
public:
    tst_QNmeaPositionInfoSource_Simulation()
        : tst_QNmeaPositionInfoSource(QNmeaPositionInfoSource::SimulationMode) {}
};

//---------------------------------------------------

class Feeder : public QObject
{
    Q_OBJECT
public:
    Feeder(QObject *parent)
        : QObject(parent)
    {
    }

    void start(QNmeaPositionInfoSourceProxy *proxy)
    {
        m_proxy = proxy;
        QTimer *timer = new QTimer;
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
        timer->setInterval(proxy->source()->minimumUpdateInterval()*2);
        timer->start();
    }

public slots:
    void timeout()
    {
        m_proxy->feedBytes(QLocationTestUtils::createRmcSentence(QDateTime::currentDateTime()).toAscii());
    }

private:
    QNmeaPositionInfoSourceProxy *m_proxy;
};

class tst_QNmeaPositionInfoSource_RealTime_Generic : public TestQGeoPositionInfoSource
{
    Q_OBJECT

public:
    tst_QNmeaPositionInfoSource_RealTime_Generic()
    {
        m_factory = new QNmeaPositionInfoSourceProxyFactory;
    }

    ~tst_QNmeaPositionInfoSource_RealTime_Generic()
    {
        delete m_factory;
    }

protected:
    QGeoPositionInfoSource *createTestSource()
    {
        QNmeaPositionInfoSource *source = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::RealTimeMode);
        QNmeaPositionInfoSourceProxy *proxy = static_cast<QNmeaPositionInfoSourceProxy*>(m_factory->createProxy(source));
        Feeder *feeder = new Feeder(source);
        feeder->start(proxy);
        return source;
    }

private:
    QNmeaPositionInfoSourceProxyFactory *m_factory;
};

//---------------------------------------------------


class UnlimitedNmeaStream : public QIODevice
{
    Q_OBJECT

public:
    UnlimitedNmeaStream(QObject *parent) : QIODevice(parent) {}

protected:
    qint64 readData(char *data, qint64 maxSize)
    {
        QByteArray bytes = QLocationTestUtils::createRmcSentence(QDateTime::currentDateTime()).toLatin1();
        qint64 sz = qMin(qint64(bytes.size()), maxSize);
        memcpy(data, bytes.constData(), sz);
        return sz;
    }

    qint64 writeData(const char *, qint64)
    {
        return -1;
    }

    qint64 bytesAvailable() const
    {
        return 1024 + QIODevice::bytesAvailable();
    }
};

class tst_QNmeaPositionInfoSource_Simulation_Generic : public TestQGeoPositionInfoSource
{
    Q_OBJECT

protected:
    QGeoPositionInfoSource *createTestSource()
    {
        QNmeaPositionInfoSource *source = new QNmeaPositionInfoSource(QNmeaPositionInfoSource::SimulationMode);
        source->setDevice(new UnlimitedNmeaStream(source));
        return source;
    }

};


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    int r;
    bool fail = false;

    tst_QNmeaPositionInfoSource_RealTime common_realTime;
    r = QTest::qExec(&common_realTime, argc, argv);
    if (r < 0) fail = true;

    tst_QNmeaPositionInfoSource_Simulation common_sim;
    r = QTest::qExec(&common_sim, argc, argv);
    if (r < 0) fail = true;

    tst_QNmeaPositionInfoSource_RealTime_Generic generic_realTime;
    r = QTest::qExec(&generic_realTime, argc, argv);
    if (r < 0) fail = true;

    tst_QNmeaPositionInfoSource_Simulation_Generic generic_sim;
    r = QTest::qExec(&generic_sim, argc, argv);
    if (r < 0) fail = true;

    return fail ? -1 : 0;
}

#include "tst_qnmeapositioninfosource.moc"
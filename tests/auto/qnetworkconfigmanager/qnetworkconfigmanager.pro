SOURCES  += tst_qnetworkconfigmanager.cpp
HEADERS  += ../qbearertestcommon.h
TARGET = tst_qnetworkconfigurationmanager
CONFIG += testcase

QT = core network

INCLUDEPATH += ../../../src/bearer

include(../../../common.pri)
qtAddLibrary(QtBearer)

symbian {
    TARGET.CAPABILITY = NetworkServices NetworkControl ReadUserData
}
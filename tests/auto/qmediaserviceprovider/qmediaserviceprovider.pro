TARGET = tst_qmediaserviceprovider
INCLUDEPATH += ../../../src/multimedia ../../../src
CONFIG += testcase

SOURCES += tst_qmediaserviceprovider.cpp

include (../../../common.pri)

qtAddLibrary(QtMedia)

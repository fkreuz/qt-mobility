# config.pri specifies the configure options

!include($$QT_MOBILITY_BUILD_TREE/config.pri) {
    error("Please run configure script");
}

lessThan(QT_MAJOR_VERSION, 4) {
    error(Qt Mobility requires Qt 4.6 or higher. Qt $${QT_VERSION} was found.);
}

contains(QT_MAJOR_VERSION, 4):lessThan(QT_MINOR_VERSION, 6) {
    error(Qt Mobility requires Qt 4.6 or higher. Qt $${QT_VERSION} was found.);
}


#generate prf file for Qt integration
PRF_OUTPUT=$${QT_MOBILITY_BUILD_TREE}/features/mobility.prf

system(echo MOBILITY_PREFIX=$${QT_MOBILITY_PREFIX} > $$PRF_OUTPUT)
system(echo MOBILITY_INCLUDE=$${QT_MOBILITY_INCLUDE} >> $$PRF_OUTPUT)
system(echo MOBILITY_LIB=$${QT_MOBILITY_LIB} >> $$PRF_OUTPUT)

unix:!symbian:system(cat $${QT_MOBILITY_SOURCE_TREE}/features/mobility.prf.template >> $$PRF_OUTPUT)
win32:system(type $${QT_MOBILITY_SOURCE_TREE}\features\mobility.prf.template >> $$PRF_OUTPUT)
symbian:system(type $${QT_MOBILITY_SOURCE_TREE}\features\mobility.prf.template >> $$PRF_OUTPUT)


# install feature file
feature.path = $$[QT_INSTALL_DATA]/mkspecs/features
feature.files = $$QT_MOBILITY_BUILD_TREE/features/mobility.prf
INSTALLS += feature

TEMPLATE = subdirs
CONFIG+=ordered

SUBDIRS += src tools plugins
#built documentation snippets
SUBDIRS += doc   

contains(build_unit_tests, yes):SUBDIRS+=tests
contains(build_examples, yes):SUBDIRS+=examples

include(doc/doc.pri)

# install Qt style headers
qtmheaders.path = $${QT_MOBILITY_INCLUDE}
qtmheaders.files = $${QT_MOBILITY_BUILD_TREE}/include/*

INSTALLS += qtmheaders

OTHER_FILES += doc/src/*.qdoc doc/src/examples/*.qdoc
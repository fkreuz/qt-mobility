include(../../staticconfig.pri)

requires(contains(mobility_modules,location))

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += qgeocoordinate \
      qgeoboundingbox \
      qgeoboundingcircle \
      qgeotiledmappingmanagerengine \
      qgeotiledmapdata \
      qgeopositioninfo \
      qgeosatelliteinfo \
      qgeoareamonitor \
      qlocationutils \
      qnmeapositioninfosource \
      qlandmarkmanagerengine \
      qlandmark \
      qlandmarkcategory \
      qlandmarkmanager \
      qgeoaddress \
      qgeomaneuver \
      qgeomapobjectplugin \
      qgeomapobject \
      qgeomapcircleobject \
      qgeomapgroupobject \
      qgeomappixmapobject \
      qgeomappolygonobject \
      qgeomappolylineobject \
      qgeomaprectangleobject \
      qgeomaprouteobject \
      qgeomaptextobject \
      qgeoplace \
      qgeoroute \
      qgeoroutingmanagerplugins \
      qgeoroutingmanager \
      qgeoroutereply \
      qgeorouterequest \
      qgeoroutesegment \
      qgeosearchreply \
      qgeosearchmanagerplugins \
      qgeosearchmanager \
      geoservicesgeomapplugin \
      geoservicesgeomap \
      geoservicesgeotiledmapplugin \
      geoservicesgeotiledmap \
      pixelindex \
      positionplugin \
      positionplugintest

# Uses experimental qtest-qml library, cannot be enabled yet.
# qmlmapsandnav

# Not implemented yet
# qgeomapobjectinfo

# With MeeGo, by default use mock backend for autotesting.
meego: {
    SUBDIRS += qgeopositioninfosource_mock \
               qgeosatelliteinfosource_mock
} else {
    SUBDIRS += qgeopositioninfosource \
               qgeosatelliteinfosource
}

!symbian:!maemo*{
SUBDIRS += qgeoserviceproviderplugins \
            qgeoserviceprovider 
}

!symbian:!maemo*:SUBDIRS +=qlandmarkmanagerplugins

contains(QT_CONFIG, declarative) {
    SUBDIRS += qdeclarativeposition
    SUBDIRS += qdeclarativelandmark
    SUBDIRS += qdeclarativemap
}
wince* {
    SUBDIRS += qgeoinfosources_wince
}

!symbian{
SUBDIRS +=  qlandmarkfilehandler_gpx \
            qlandmarkfilehandler_lmx
}

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

#include "qgeomapobject.h"
#include "qgeomapobject_p.h"
#include "qgeomapobjectengine_p.h"
#include "qgeomapobjectinfo.h"
#include "qgeomapdata.h"
#include "qgeomapdata_p.h"
#include "qgeoboundingbox.h"
#include "qgeocoordinate.h"
#include "projwrapper_p.h"

#include <QtAlgorithms>

QTM_BEGIN_NAMESPACE

/*!
    \class QGeoMapObject
    \brief The QGeoMapObject class is a graphical item to be displayed on a map.
    \inmodule QtLocation
    \since 1.1

    \ingroup maps-mapping-objects

    QGeoMapObject is the base class used to display graphical items on a map.

    Subclasses of QGeoMapObject exist in order to simplify the task of
    creating and managing map objects of various kinds.

    QGeoMapCustomObject is the most generic of these objects in that it
    allows QGraphicsItems to be added to a map, however as not all mapping
    plugins use the Qt Graphics View framework so clients should use
    QGraphicsGeoMap::supportsCustomMapObjects() before using
    QGeoMapCustomObject.

    QGeoMapObject instances can also be grouped into heirarchies in order to
    simplify the process of creating compound objects and managing groups of
    objects (see QGeoMapGroupObject)
*/

/*!
    \enum QGeoMapObject::Type

    Describes the type of a map object.

    \value NullType
        An empty QGeoMapObject.
    \value GroupType
        A QGeoMapObject used to organize other map objects into groups.
    \value RectangleType
        A QGeoMapObject used to display a rectangular region.
    \value CircleType
        A QGeoMapObject used to display a circular region.
    \value PolylineType
        A QGeoMapObject used to display a multi-segment line.
    \value PolygonType
        A QGeoMapObject used to display a polygonal region.
    \value PixmapType
        A QGeoMapObject used to display a pixmap on a map.
    \value TextType
        A QGeoMapObject used to display text on a map
    \value RouteType
        A QGeoMapObject used to display a route.
    \value CustomType
        A QGeoMapObject displaying a custom GraphicsItem.
*/

/*!
    \enum QGeoMapObject::CoordinateUnit

    \since 1.2

    Describes the units of measurement used for a map object's
    graphics item.

    \value PixelUnit
        Units are in pixels on the screen. Pixel coordinate (0,0) is
        translated to the origin coordinate.
    \value MeterUnit
        Units are in meters on the ground -- a local Transverse Mercator
        coordinate system (on the WGS84 ellipsoid) is used for translation,
        centered on the origin coordinate.
    \value RelativeArcSecondUnit
        Units are in arc seconds relative to the origin coordinate (along the
        WGS84 ellipsoid).
    \value AbsoluteArcSecondUnit
        Units are in arc seconds on WGS84, origin ignored.
*/

/*!
    \enum QGeoMapObject::TransformType

    \since 1.2

    Describes the type of transformation applied to change this object's
    coordinate system into screen coordinates.

    \value BilinearTransform
        This object's bounding box is taken, and transformed at each of its
        corners into screen coordinates. A bilinear interpolation is then used
        to draw the rest of the object's GraphicsItem.
    \value ExactTransform
        Individual key points on the object are transformed and the GraphicsItem
        is constructed in direct pixel coordinates. This is only available for
        certain subclasses, depending on the implementation of QGeoMapData used.
*/

/*!
    Constructs a new map object associated with \a mapData.

    The object will be in pixel coordinates, with exact transform.
*/
QGeoMapObject::QGeoMapObject(QGeoMapData *mapData)
    : d_ptr(new QGeoMapObjectPrivate())
{
    setMapData(mapData);
}

/*!
    Destroys this map object.
*/
QGeoMapObject::~QGeoMapObject()
{
    setMapData(0);
    delete d_ptr;
}

/*!
    Returns the type of this map object.
    \since 1.1
*/
QGeoMapObject::Type QGeoMapObject::type() const
{
//    if (d_ptr->graphicsItem)
//        return QGeoMapObject::CustomType;
//    else
        return QGeoMapObject::NullType;
}

/*!
    \property QGeoMapObject::zValue
    \brief the z-value of the map object.

    The z-value determines the order in which the objects are drawn on the
    screen.  Objects with the same value will be drawn in the order that
    they were added to the map or map object.

    This is the same behaviour as QGraphicsItem.
    \since 1.1
*/
void QGeoMapObject::setZValue(int zValue)
{
    if (d_ptr->zValue != zValue) {
        d_ptr->zValue = zValue;
        emit zValueChanged(d_ptr->zValue);
    }
}

int QGeoMapObject::zValue() const
{
    return d_ptr->zValue;
}

/*!
    \property QGeoMapObject::visible
    \brief whether the map object is visible.
    \since 1.1
*/
void QGeoMapObject::setVisible(bool visible)
{
    if (d_ptr->isVisible != visible) {
        d_ptr->isVisible = visible;
        emit visibleChanged(d_ptr->isVisible);
    }
}

bool QGeoMapObject::isVisible() const
{
    return d_ptr->isVisible;
}

/*!
    \property QGeoMapObject::selected
    \brief whether the map object is selected.
    \since 1.1
*/
void QGeoMapObject::setSelected(bool selected)
{
    if (d_ptr->isSelected != selected) {
        d_ptr->isSelected = selected;
        emit selectedChanged(d_ptr->isSelected);
    }
}

bool QGeoMapObject::isSelected() const
{
    return d_ptr->isSelected;
}

/*!
    Returns a bounding box which contains this map object.

    The default implementation requires the object to be added to a map
    before this function returns a valid bounding box.
    \since 1.1
*/
QGeoBoundingBox QGeoMapObject::boundingBox() const
{
    if (!d_ptr->info)
        return QGeoBoundingBox();

    return d_ptr->info->boundingBox();
}

/*!
    Returns whether \a coordinate is contained with the boundary of this
    map object.

    The default implementation requires the object to be added to a map
    before this function is able to return true.
    \since 1.1
*/
bool QGeoMapObject::contains(const QGeoCoordinate &coordinate) const
{
    if (!d_ptr->info)
        return false;

    return d_ptr->info->contains(coordinate);
}

/*!
    \internal
    \since 1.1
*/
bool QGeoMapObject::operator<(const QGeoMapObject &other) const
{
    return d_ptr->zValue < other.d_ptr->zValue ||
            (d_ptr->zValue == other.d_ptr->zValue && d_ptr->serial < other.d_ptr->serial);
}

/*!
    \internal
    \since 1.1
*/
bool QGeoMapObject::operator>(const QGeoMapObject &other) const
{
    return d_ptr->zValue > other.d_ptr->zValue ||
            (d_ptr->zValue == other.d_ptr->zValue && d_ptr->serial > other.d_ptr->serial);
}

/*!
    Associates the QGeoMapData instance \a mapData with this map object.

    This will create an appropriate QGeoMapObjectInfo instance for
    this QGeoMapObject and will connect the appropriate signals to it
    so that it can be kept up to date.
    \since 1.1
*/
void QGeoMapObject::setMapData(QGeoMapData *mapData)
{
    if (d_ptr->mapData == mapData)
        return;

    if (d_ptr->info) {
        delete d_ptr->info;
        d_ptr->info = 0;
    }

    d_ptr->mapData = mapData;
    if (!d_ptr->mapData)
        return;

    d_ptr->info = mapData->createMapObjectInfo(this);

    if (!d_ptr->info)
        return;

    connect(d_ptr->mapData,
            SIGNAL(windowSizeChanged(QSizeF)),
            d_ptr->info,
            SLOT(windowSizeChanged(QSizeF)));
    connect(d_ptr->mapData,
            SIGNAL(zoomLevelChanged(qreal)),
            d_ptr->info,
            SLOT(zoomLevelChanged(qreal)));
    connect(d_ptr->mapData,
            SIGNAL(centerChanged(QGeoCoordinate)),
            d_ptr->info,
            SLOT(centerChanged(QGeoCoordinate)));

    connect(this,
            SIGNAL(zValueChanged(int)),
            d_ptr->info,
            SLOT(zValueChanged(int)));
    connect(this,
            SIGNAL(visibleChanged(bool)),
            d_ptr->info,
            SLOT(visibleChanged(bool)));
    connect(this,
            SIGNAL(selectedChanged(bool)),
            d_ptr->info,
            SLOT(selectedChanged(bool)));
    connect(this,
            SIGNAL(originChanged(QGeoCoordinate)),
            d_ptr->info,
            SLOT(originChanged(QGeoCoordinate)));
    connect(this,
            SIGNAL(transformTypeChanged(QGeoMapObject::TransformType)),
            d_ptr->info,
            SLOT(transformTypeChanged(QGeoMapObject::TransformType)));
    connect(this,
            SIGNAL(unitsChanged(QGeoMapObject::CoordinateUnit)),
            d_ptr->info,
            SLOT(unitsChanged(QGeoMapObject::CoordinateUnit)));

    d_ptr->info->init();
}

/*!
    Returns the QGeoMapData instance associated with this object.

    Will return 0 if not QGeoMapData instance has been set.
    \since 1.1
*/
QGeoMapData* QGeoMapObject::mapData() const
{
    return d_ptr->mapData;
}

/*!
    Returns the QGeoMapObjectInfo instance which implements the
    QGeoMapData specific behaviours of this map object.

    This will mostly be useful when implementing custom QGeoMapData
    subclasses.
    \since 1.1
*/
QGeoMapObjectInfo *QGeoMapObject::info() const
{
    return d_ptr->info;
}

/*!
    \property QGeoMapObject::transformType
    \brief the transformation type used to draw the object.

    \since 1.2

    \sa QGeoMapObject::TransformType
*/
QGeoMapObject::TransformType QGeoMapObject::transformType() const
{
    return d_ptr->transType;
}

/*!
    Sets the transform type of the object to \a type.
    \since 1.2
*/
void QGeoMapObject::setTransformType(const TransformType &type)
{
    if (type == d_ptr->transType)
        return;

    d_ptr->transType = type;

    emit transformTypeChanged(type);
}

/*!
    \property QGeoMapObject::origin
    \brief the origin of the object's coordinate system.

    \since 1.2

    How the origin coordinate is used depends on the selected coordinate
    system, see QGeoMapObject::TransformType for more details.
*/
QGeoCoordinate QGeoMapObject::origin() const
{
    return d_ptr->origin;
}

/*!
    Sets the origin of the object to \a origin.
    \since 1.2
*/
void QGeoMapObject::setOrigin(const QGeoCoordinate &origin)
{
    if (origin == d_ptr->origin)
        return;

    d_ptr->origin = origin;

    emit originChanged(origin);
}

/*!
    \property QGeoMapObject::units
    \brief the units of measurement for the object.

    \since 1.2

    \sa QGeoMapObject::CoordinateUnit
*/
QGeoMapObject::CoordinateUnit QGeoMapObject::units() const
{
    return d_ptr->units;
}

/*!
    Sets the coordinate units of the object to \a unit.

    Note that setting this property will reset the transformType property to
    the default for the units given. For PixelUnit, this is ExactTransform,
    and for all others, BilinearTransform.
    \since 1.2
*/
void QGeoMapObject::setUnits(const CoordinateUnit &unit)
{
    if (unit == d_ptr->units)
        return;

    d_ptr->units = unit;

    if (unit == QGeoMapObject::PixelUnit)
        setTransformType(QGeoMapObject::ExactTransform);
    else
        setTransformType(QGeoMapObject::BilinearTransform);

    emit unitsChanged(unit);
}

/*!
\fn void QGeoMapObject::zValueChanged(int zValue)

    This signal is emitted when the z value of the map object
    has changed.

    The new value is \a zValue.
    \since 1.1
*/

/*!
\fn void QGeoMapObject::visibleChanged(bool visible)

    This signal is emitted when the visible state of the map object
    has changed.

    The new value is \a visible.
    \since 1.1
*/

/*!
\fn void QGeoMapObject::selectedChanged(bool selected)

    This signal is emitted when the selected state of the map object
    has changed.

    The new vlaue is \a selected.
    \since 1.1
*/

/*!
\fn void QGeoMapObject::originChanged(QGeoCoordinate origin)

    This signal is emitted when the origin of the map object has changed.

    The new value is \a origin.
    \since 1.2
*/

/*!
\fn void QGeoMapObject::unitsChanged(QGeoMapObject::CoordinateUnit units)

    This signal is emitted when the coordinate units of the map object have changed.

    The new value is \a units.
    \since 1.2
*/

/*!
\fn void QGeoMapObject::transformTypeChanged(QGeoMapObject::TransformType transformType)

    This signal is emitted when the transform type of the map object has changed.

    The new value is \a transformType.
    \since 1.2
*/

/*******************************************************************************
*******************************************************************************/

QGeoMapObjectPrivate::QGeoMapObjectPrivate()
    : zValue(0),
      serial(0),
      isVisible(true),
      isSelected(false),
      mapData(0),
      info(0),
      units(QGeoMapObject::PixelUnit),
      transType(QGeoMapObject::ExactTransform){}

QGeoMapObjectPrivate::~QGeoMapObjectPrivate()
{
    if (info)
        delete info;
}

/*******************************************************************************
*******************************************************************************/

#include "moc_qgeomapobject.cpp"

QTM_END_NAMESPACE

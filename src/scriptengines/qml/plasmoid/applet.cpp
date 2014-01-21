/*
 *   Copyright 2014 Marco Martin <mart@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "applet.h"

#include <QQmlComponent>
#include <QQmlExpression>
#include <QQmlEngine>
#include <QQmlProperty>
#include <QQmlContext>

#include <QDebug>

#include <Plasma/Applet>
#include <Plasma/Containment>
#include <Plasma/Corona>
#include <Plasma/Package>
#include <kdeclarative/qmlobject.h>

SizeHintAttachedType::SizeHintAttachedType(QObject *parent)
    : QObject(parent),
      m_minimumWidth(-1),
      m_minimumHeight(-1),
      m_implicitWidth(-1),
      m_implicitHeight(-1),
      m_maximumWidth(-1),
      m_maximumHeight(-1)
{
}

SizeHintAttachedType::~SizeHintAttachedType()
{
}

int SizeHintAttachedType::minimumWidth() const
{
    return m_minimumWidth;
}

void SizeHintAttachedType::setMinimumWidth(int width)
{
    if (m_minimumWidth == width) {
        return;
    }

    m_minimumWidth = width;
    emit minimumWidthChanged(width);
}

int SizeHintAttachedType::minimumHeight() const
{
    return m_minimumHeight;
}

void SizeHintAttachedType::setMinimumHeight(int height)
{
    if (m_minimumHeight == height) {
        return;
    }

    m_minimumHeight = height;
    emit minimumHeightChanged(height);
}

int SizeHintAttachedType::implicitWidth() const
{
    return m_implicitWidth;
}

void SizeHintAttachedType::setImplicitWidth(int width)
{
    if (m_implicitWidth == width) {
        return;
    }

    m_implicitWidth = width;
    emit implicitWidthChanged(width);
}

int SizeHintAttachedType::implicitHeight() const
{
    return m_implicitHeight;
}

void SizeHintAttachedType::setImplicitHeight(int height)
{
    if (m_implicitHeight == height) {
        return;
    }

    m_implicitHeight = height;
    emit implicitHeightChanged(height);
}

int SizeHintAttachedType::maximumWidth() const
{
    return m_maximumWidth;
}

void SizeHintAttachedType::setMaximumWidth(int width)
{
    if (m_maximumWidth == width) {
        return;
    }

    m_maximumWidth = width;
    emit maximumWidthChanged(width);
}

int SizeHintAttachedType::maximumHeight() const
{
    return m_maximumHeight;
}

void SizeHintAttachedType::setMaximumHeight(int height)
{
    if (m_maximumHeight == height) {
        return;
    }

    m_maximumHeight = height;
    emit maximumHeightChanged(height);
}



Applet::Applet(QQuickItem *parent)
    : QQuickItem(parent),
      m_switchWidth(-1),
      m_switchHeight(-1),
      m_engine(0),
      m_expanded(false)
{
    m_compactRepresentationCheckTimer.setSingleShot(true);
    m_compactRepresentationCheckTimer.setInterval(250);
    connect(&m_compactRepresentationCheckTimer, SIGNAL(timeout()),
            this, SLOT(compactRepresentationCheck()));
    m_compactRepresentationCheckTimer.start();
}

Applet::~Applet()
{
}

int Applet::switchWidth() const
{
    return m_switchWidth;
}

void Applet::setSwitchWidth(int width)
{
    if (m_switchWidth == width) {
        return;
    }

    m_switchWidth = width;
    emit switchWidthChanged(width);
}

int Applet::switchHeight() const
{
    return m_switchHeight;
}

void Applet::setSwitchHeight(int width)
{
    if (m_switchHeight == width) {
        return;
    }

    m_switchHeight = width;
    emit switchHeightChanged(width);
}

QQmlComponent *Applet::compactRepresentation()
{
    return m_compactRepresentation.data();
}

void Applet::setCompactRepresentation(QQmlComponent *component)
{
    if (m_compactRepresentation.data() == component) {
        return;
    }

    m_compactRepresentation = component;
    emit compactRepresentationChanged(component);
}


QQmlComponent *Applet::fullRepresentation()
{
    return m_fullRepresentation.data();
}

void Applet::setFullRepresentation(QQmlComponent *component)
{
    if (m_fullRepresentation.data() == component) {
        return;
    }

    m_fullRepresentation = component;
    emit fullRepresentationChanged(component);
}

QQmlComponent *Applet::preferredRepresentation()
{
    return m_preferredRepresentation.data();
}

void Applet::setPreferredRepresentation(QQmlComponent *component)
{
    if (m_preferredRepresentation.data() == component) {
        return;
    }

    m_preferredRepresentation = component;
    emit preferredRepresentationChanged(component);
}

bool Applet::isExpanded() const
{
    return m_expanded;
}

void Applet::setExpanded(bool expanded)
{
    //if there is no compact representation it means it's always expanded
    //Containnments are always expanded
    if (!m_compactRepresentationItem || /*qobject_cast<ContainmentInterface *>(this) ||*/ m_expanded == expanded) {
        return;
    }

    m_expanded = expanded;
    emit expandedChanged(expanded);
}



////////////Internals

void Applet::classBegin()
{
    m_engine = QtQml::qmlEngine(this);
    Q_ASSERT(m_engine);
    m_engine->rootContext()->setContextProperty("plasmoid", this);
}

void Applet::componentComplete()
{
    m_applet = property("_plasma_applet").value<Plasma::Applet *>();
    Q_ASSERT(m_applet);

    m_qmlObject = new KDeclarative::QmlObject(m_engine, this);

    //default m_compactRepresentation is a simple icon provided by the shell package
    if (!m_compactRepresentation) {
        m_compactRepresentation = new QQmlComponent(m_engine, this);
        m_compactRepresentation.data()->loadUrl(QUrl::fromLocalFile(m_applet->containment()->corona()->package().filePath("defaultcompactrepresentation")));
    }

    //we really want a full representation, default m_fullRepresentation is an error message
    if (!m_fullRepresentation) {
        m_fullRepresentation = new QQmlComponent(m_engine, this);
        m_fullRepresentation.data()->loadUrl(QUrl::fromLocalFile(m_applet->containment()->corona()->package().filePath("appleterror")));
    }

    //default m_compactRepresentationExpander is the popup in which fullRepresentation goes
    if (!m_compactRepresentationExpander) {
        m_compactRepresentationExpander = new QQmlComponent(m_engine, this);
        m_compactRepresentationExpander.data()->loadUrl(QUrl::fromLocalFile(m_applet->containment()->corona()->package().filePath("compactapplet")));
    }
}

QObject *Applet::compactRepresentationItem()
{
    return m_compactRepresentationItem.data();
}

QObject *Applet::fullRepresentationItem()
{
    return m_fullRepresentationItem.data();
}

QObject *Applet::compactRepresentationExpanderItem()
{
    return m_compactRepresentationExpanderItem.data();
}



QObject *Applet::createCompactRepresentationItem()
{
    if (!m_compactRepresentation) {
        return 0;
    }

    if (m_compactRepresentationItem) {
        return m_compactRepresentationItem.data();
    }

    m_compactRepresentationItem = m_qmlObject->createObjectFromComponent(m_compactRepresentation.data());

    emit compactRepresentationItemChanged(m_compactRepresentationItem.data());

    return m_compactRepresentationItem.data();
}

QObject *Applet::createFullRepresentationItem()
{
    if (!m_fullRepresentation) {
        return 0;
    }

    if (m_fullRepresentationItem) {
        return m_fullRepresentationItem.data();
    }

    m_fullRepresentationItem = m_qmlObject->createObjectFromComponent(m_fullRepresentation.data());

    emit fullRepresentationItemChanged(m_fullRepresentationItem.data());

    return m_fullRepresentationItem.data();
}

QObject *Applet::createCompactRepresentationExpanderItem()
{
    if (!m_compactRepresentationExpander) {
        return 0;
    }

    if (m_compactRepresentationExpanderItem) {
        return m_compactRepresentationExpanderItem.data();
    }

    m_compactRepresentationExpanderItem = m_qmlObject->createObjectFromComponent(m_compactRepresentationExpander.data());

    emit compactRepresentationExpanderItemChanged(m_compactRepresentationExpanderItem.data());

    return m_compactRepresentationExpanderItem.data();
}

void Applet::connectLayoutAttached(QObject *item)
{
    QObject *layout = 0;

    //Extract the representation's Layout, if any
    //No Item?
    if (!item) {
        return;
    }

    //Search a child that has the needed Layout properties
    //HACK: here we are not type safe, but is the only way to access to a pointer of Layout
    foreach (QObject *child, item->children()) {
        //find for the needed property of Layout: minimum/maximum/preferred sizes and fillWidth/fillHeight
        if (child->property("minimumWidth").isValid() && child->property("minimumHeight").isValid() &&
            child->property("preferredWidth").isValid() && child->property("preferredHeight").isValid() &&
            child->property("maximumWidth").isValid() && child->property("maximumHeight").isValid() &&
            child->property("fillWidth").isValid() && child->property("fillHeight").isValid()
        ) {
            layout = child;
        }
    }

    if (!layout) {
        return;
    }

    //propagate all the size hints
    propagateSizeHint("minimumWidth");
    propagateSizeHint("minimumHeight");
    propagateSizeHint("preferredWidth");
    propagateSizeHint("preferredHeight");
    propagateSizeHint("maximumWidth");
    propagateSizeHint("maximumHeight");
    propagateSizeHint("fillWidth");
    propagateSizeHint("fillHeight");

    //HACK: check the Layout properties we wrote
    QQmlProperty p(this, "Layout.minimumWidth", QtQml::qmlContext(this));
    QObject *ownLayout = p.object();

    //this should never happen, since we ask to create it if doesn't exists
    if (!ownLayout) {
        return;
    }

    //if the representation didn't change, don't do anything
    if (m_representationLayout.data() == layout ||
        m_ownLayout.data() == ownLayout) {
        return;
    }

    disconnect(layout, 0, this, 0);

    //Here we can't use the new connect syntax because we can't link against QtQuick layouts
    connect(layout, SIGNAL(minimumWidthChanged()),
            this, SLOT(minimumWidthChanged()));
    connect(layout, SIGNAL(minimumHeightChanged()),
            this, SLOT(minimumHeightChanged()));

    connect(layout, SIGNAL(preferredWidthChanged()),
            this, SLOT(preferredWidthChanged()));
    connect(layout, SIGNAL(preferredHeightChanged()),
            this, SLOT(preferredHeightChanged()));

    connect(layout, SIGNAL(maximumWidthChanged()),
            this, SLOT(maximumWidthChanged()));
    connect(layout, SIGNAL(maximumHeightChanged()),
            this, SLOT(maximumHeightChanged()));

    connect(layout, SIGNAL(fillWidthChanged()),
            this, SLOT(fillWidthChanged()));
    connect(layout, SIGNAL(fillHeightChanged()),
            this, SLOT(fillHeightChanged()));

    m_representationLayout = layout;
    m_ownLayout = ownLayout;
}

void Applet::propagateSizeHint(const QByteArray &layoutProperty)
{
    if (!m_currentRepresentationItem) {
        return;
    }

    QQmlExpression *expr = new QQmlExpression(QtQml::qmlContext(m_currentRepresentationItem.data()), m_currentRepresentationItem.data(), "Layout."+layoutProperty);
    QQmlProperty prop(this, "Layout."+layoutProperty, QtQml::qmlContext(m_currentRepresentationItem.data()));
    prop.write(expr->evaluate());
}

void Applet::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    Q_UNUSED(oldGeometry)

    QQuickItem::geometryChanged(newGeometry, oldGeometry);
    m_compactRepresentationCheckTimer.start();
}



//// Slots

void Applet::compactRepresentationCheck()
{
    bool full = false;

    if (m_switchWidth > 0 && m_switchHeight > 0) {
        full = width() > m_switchWidth && height() > m_switchHeight;
    //if a size to switch wasn't set, determine what representation to always chose
    } else {
        //preferred representation set?
        if (m_preferredRepresentation) {
            full = m_preferredRepresentation.data() == m_fullRepresentation.data();
        //Otherwise, base on FormFactor
        } else {
            full = (m_applet->formFactor() != Plasma::Types::Horizontal && m_applet->formFactor() != Plasma::Types::Vertical);
        }
    }

    if ((full && m_fullRepresentationItem && m_fullRepresentationItem.data() == m_currentRepresentationItem.data()) ||
        (!full && m_compactRepresentationItem && m_compactRepresentationItem.data() == m_currentRepresentationItem.data())
    ) {
        return;
    }

    //Expanded
    if (full) {
        QQuickItem *item = qobject_cast<QQuickItem *>(createFullRepresentationItem());

        if (item) {
            item->setParentItem(this);
            {
                //set anchors
                QQmlExpression expr(QtQml::qmlContext(this), item, "parent");
                QQmlProperty prop(item, "anchors.fill");
                prop.write(expr.evaluate());
            }
            if (m_compactRepresentationItem) {
                m_compactRepresentationItem.data()->setProperty("visible", false);
            }
            if (m_compactRepresentationExpanderItem) {
                m_compactRepresentationExpanderItem.data()->setProperty("compactRepresentation", QVariant());
                m_compactRepresentationExpanderItem.data()->setProperty("fullRepresentation", QVariant());
            }

            m_currentRepresentationItem = item;
            connectLayoutAttached(item);
        }

    //Icon
    } else {
        QQuickItem *fullItem = qobject_cast<QQuickItem *>(createFullRepresentationItem());
        QQuickItem *compactItem = qobject_cast<QQuickItem *>(createCompactRepresentationItem());
        QObject *compactExpanderItem = createCompactRepresentationExpanderItem();

        if (fullItem && compactItem && compactExpanderItem) {
            //set the root item as the main visible item
            compactItem->setParentItem(this);
            compactItem->setVisible(true);
            {
                //set anchors
                QQmlExpression expr(QtQml::qmlContext(this), compactItem, "parent");
                QQmlProperty prop(compactItem, "anchors.fill");
                prop.write(expr.evaluate());
            }

            compactExpanderItem->setProperty("compactRepresentation", QVariant::fromValue(compactItem));
            compactExpanderItem->setProperty("fullRepresentation", QVariant::fromValue(fullItem));
            m_currentRepresentationItem = compactItem;
            connectLayoutAttached(compactItem);
        }
    }
}

void Applet::minimumWidthChanged()
{
    propagateSizeHint("minimumWidth");
}

void Applet::minimumHeightChanged()
{
    propagateSizeHint("minimumHeight");
}

void Applet::preferredWidthChanged()
{
    propagateSizeHint("preferredWidth");
}

void Applet::preferredHeightChanged()
{
    propagateSizeHint("preferredHeight");
}

void Applet::maximumWidthChanged()
{
    propagateSizeHint("maximumWidth");
}

void Applet::maximumHeightChanged()
{
    propagateSizeHint("maximumHeight");
}

void Applet::fillWidthChanged()
{
    propagateSizeHint("fillWidth");
}

void Applet::fillHeightChanged()
{
    propagateSizeHint("fillHeight");
}



#include "moc_applet.cpp"


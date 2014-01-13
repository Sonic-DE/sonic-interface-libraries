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

#include <QDebug>
*

Applet::Applet(QQmlEngine *parent)
    : Plasma::Theme(parent),
      m_engine(parent)
{
    m_defaultIconSize = KIconLoader::global()->currentSize(KIconLoader::Desktop);

    m_iconSizes = new QQmlPropertyMap(this);
    m_iconSizes->insert("desktop", QVariant(KIconLoader::global()->currentSize(KIconLoader::Desktop)));
    m_iconSizes->insert("panel", QVariant(KIconLoader::global()->currentSize(KIconLoader::Panel)));
    m_iconSizes->insert("toolbar", KIconLoader::global()->currentSize(KIconLoader::Toolbar));
    m_iconSizes->insert("small", KIconLoader::global()->currentSize(KIconLoader::Small));
    m_iconSizes->insert("dialog", KIconLoader::global()->currentSize(KIconLoader::Dialog));

    connect(this, &Plasma::Theme::themeChanged, this, &Applet::themeChanged);
    connect(KIconLoader::global(), SIGNAL(iconLoaderSettingsChanged()), this, SLOT(iconLoaderSettingsChanged()));

    updateSpacing();
    installEventFilter(qApp);
}

Applet::~Applet()
{
}

int Applet::minimumWidth() const
{
    return m_minimumWidth;
}

void Applet::setMinimumWidth(int width)
{
    if (m_minimumWidth == width) {
        return;
    }

    m_minimumWidth = width;
    emit minimumWidthChanged();
}

int Applet::minimumHeight() const
{
    return m_minimumHeight;
}

void Applet::setMinimumHeight(int width)
{
    if (m_minimumHeight == width) {
        return;
    }

    m_minimumHeight = width;
    emit minimumHeightChanged();
}

int Applet::implicitWidth() const
{
    return m_implicitWidth;
}

void Applet::setImplicitWidth(int width)
{
    if (m_implicitWidth == width) {
        return;
    }

    m_implicitWidth = width;
    emit implicitWidthChanged();
}

int Applet::implicitHeight() const
{
    return m_implicitHeight;
}

void Applet::setImplicitHeight(int width)
{
    if (m_implicitHeight == width) {
        return;
    }

    m_implicitHeight = width;
    emit implicitHeightChanged();
}

int Applet::maximumWidth() const
{
    return m_maximumWidth;
}

void Applet::setMaximumWidth(int width)
{
    if (m_maximumWidth == width) {
        return;
    }

    m_maximumWidth = width;
    emit maximumWidthChanged();
}

int Applet::maximumHeight() const
{
    return m_maximumHeight;
}

void Applet::setMaximumHeight(int width)
{
    if (m_maximumHeight == width) {
        return;
    }

    m_maximumHeight = width;
    emit maximumHeightChanged();
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
    emit compactRepresentationChanged();
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
    emit fullRepresentationChanged();
}

#include "moc_applet.cpp"


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



Applet::Applet(QObject *parent)
    : QObject(parent)
{
}

Applet::~Applet()
{
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

#include "moc_applet.cpp"


/*
    SPDX-FileCopyrightText: 2023 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "dialogbackground_p.h"

#include <QQmlComponent>

#include "sharedqmlengine.h"

namespace PlasmaQuick
{
///////////////////////DialogBackground

DialogBackground::DialogBackground(QQuickItem *parent)
    : QQuickItem(parent)
    , m_sharedEngine(new SharedQmlEngine(this))
{
    QQmlComponent component(m_sharedEngine->engine().get(), QStringLiteral(":/DialogBackground.qml"));
    QObject *object = m_sharedEngine->createObjectFromComponent(&component);

    m_frameSvgItem = qobject_cast<QQuickItem *>(object);
    Q_ASSERT(m_frameSvgItem);
}

DialogBackground::~DialogBackground()
{
}

QString DialogBackground::imagePath() const
{
    return m_frameSvgItem->property("imagePath").toString();
}

void DialogBackground::setImagePath(const QString &path)
{
    m_frameSvgItem->setProperty("imagePath", path);
}

void DialogBackground::setEnabledBorders(const Plasma::FrameSvg::EnabledBorders borders)
{
    m_frameSvgItem->setProperty("enabledBorders", QVariant::fromValue(borders));
}

Plasma::FrameSvg::EnabledBorders DialogBackground::enabledBorders() const
{
    return m_frameSvgItem->property("enabledBorders").value<Plasma::FrameSvg::EnabledBorders>();
}

qreal DialogBackground::leftMargin() const
{
    // assume margins is valid, as we asserted it's a valid FrameSvgItem
    QObject *margins = m_frameSvgItem->property("fixedMargins").value<QObject *>();
    return margins->property("left").value<qreal>();
}

qreal DialogBackground::topMargin() const
{
    QObject *margins = m_frameSvgItem->property("fixedMargins").value<QObject *>();
    return margins->property("top").value<qreal>();
}

qreal DialogBackground::rightMargin() const
{
    QObject *margins = m_frameSvgItem->property("fixedMargins").value<QObject *>();
    return margins->property("right").value<qreal>();
}

qreal DialogBackground::bottomMargin() const
{
    QObject *margins = m_frameSvgItem->property("fixedMargins").value<QObject *>();
    return margins->property("bottom").value<qreal>();
}

}

#include "private/moc_dialogbackground_p.cpp"

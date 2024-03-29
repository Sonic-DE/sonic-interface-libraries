/*
    SPDX-FileCopyrightText: 2023 Marco Martin <mart@kde.org>

    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#ifndef PLASMOIDATTACHED_P_H
#define PLASMOIDATTACHED_P_H

#include "plasmoid/containmentitem.h"
#include "plasmoid/plasmoiditem.h"
#include "plasmoid/wallpaperitem.h"
#include <Plasma/Applet>
#include <Plasma/Containment>
#include <Plasma/Corona>
#include <appletquickitem.h>

//
//  W A R N I N G
//  -------------
//
// This file is not part of the public Plasma API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

struct AppletForeign {
    Q_GADGET
    QML_ANONYMOUS
    QML_FOREIGN(Plasma::Applet)
};

struct ContainmentForeign {
    Q_GADGET
    QML_ANONYMOUS
    QML_FOREIGN(Plasma::Containment)
};

struct CoronaForeign {
    Q_GADGET
    QML_ANONYMOUS
    QML_FOREIGN(Plasma::Corona)
};

struct PlasmoidItemForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(PlasmoidItem)
    QML_FOREIGN(PlasmoidItem)
};

struct ContainmentItemForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(ContainmentItem)
    QML_FOREIGN(ContainmentItem)
};

struct WallpaperItemForeign {
    Q_GADGET
    QML_NAMED_ELEMENT(WallpaperItem)
    QML_FOREIGN(WallpaperItem)
};

namespace PlasmaQuick
{

// Class used exclusively to generate the Plasmoid.* attached properties, which are Applet instances
class PlasmoidAttached : public Plasma::Applet
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Plasmoid)
    QML_UNCREATABLE("Attached property and enums only")
    QML_ATTACHED(Plasma::Applet)

public:
    ////NEEDED BY QML TO CREATE ATTACHED PROPERTIES
    static Plasma::Applet *qmlAttachedProperties(QObject *object);
};

// Class used exclusively to generate the Containment.* attached properties, which are Containment instances
class ContainmentAttached : public Plasma::Containment
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Containment)
    QML_UNCREATABLE("Attached property and enums only")
    QML_ATTACHED(Plasma::Containment)

public:
    ////NEEDED BY QML TO CREATE ATTACHED PROPERTIES
    static Plasma::Containment *qmlAttachedProperties(QObject *object);
};

}

QML_DECLARE_TYPEINFO(PlasmaQuick::PlasmoidAttached, QML_HAS_ATTACHED_PROPERTIES)
QML_DECLARE_TYPEINFO(PlasmaQuick::ContainmentAttached, QML_HAS_ATTACHED_PROPERTIES)

#endif

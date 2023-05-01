/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plasmawindow.h"

#include "../declarativeimports/core/framesvgitem.h"
#include "dialogshadows_p.h"

#include <QMarginsF>
#include <QQuickItem>

#include <KWindowEffects>
#include <KX11Extras>

using namespace Plasma;

namespace PlasmaQuick
{
class PlasmaWindowPrivate
{
public:
    QMargins margins() const;
    QPointer<QQuickItem> mainItem;
    Plasma::FrameSvgItem *frameSvgItem;
};

PlasmaWindow::PlasmaWindow(QWindow *parent)
    : QQuickWindow(parent)
    , d(new PlasmaWindowPrivate)
{
    setColor(QColor(Qt::transparent));
    setFlags(Qt::FramelessWindowHint);

    d->frameSvgItem = new Plasma::FrameSvgItem(contentItem());
    d->frameSvgItem->setImagePath(QStringLiteral("dialogs/background"));
    connect(d->frameSvgItem->fixedMargins(), &Plasma::FrameSvgItemMargins::marginsChanged, this, &PlasmaWindow::updateMainItemGeometry);
    connect(d->frameSvgItem, &Plasma::FrameSvgItem::maskChanged, this, &PlasmaWindow::handleFrameChanged);

    DialogShadows::self()->addWindow(this, d->frameSvgItem->enabledBorders());
}

PlasmaWindow::~PlasmaWindow()
{
}

void PlasmaWindow::setMainItem(QQuickItem *mainItem)
{
    if (d->mainItem == mainItem)
        return;

    d->mainItem = mainItem;

    if (d->mainItem) {
        mainItem->setParentItem(contentItem());
        updateMainItemGeometry();
    }
    Q_EMIT mainItemChanged();
}

QQuickItem *PlasmaWindow::mainItem() const
{
    return d->mainItem;
}

void PlasmaWindow::setMainItemSize(const QSize &contentSize)
{
    QSize newSize = contentSize.grownBy(d->margins());
    resize(newSize);
}

static FrameSvg::EnabledBorders edgeToBorder(Qt::Edges edges)
{
    FrameSvg::EnabledBorders borders = FrameSvg::EnabledBorders(0);

    if (edges & Qt::Edge::TopEdge) {
        borders |= FrameSvg::EnabledBorder::TopBorder;
    }
    if (edges & Qt::Edge::BottomEdge) {
        borders |= FrameSvg::EnabledBorder::BottomBorder;
    }
    if (edges & Qt::Edge::LeftEdge) {
        borders |= FrameSvg::EnabledBorder::LeftBorder;
    }
    if (edges & Qt::Edge::RightEdge) {
        borders |= FrameSvg::EnabledBorder::RightBorder;
    }

    return borders;
}

void PlasmaWindow::setBorders(Qt::Edges bordersToShow)
{
    d->frameSvgItem->setEnabledBorders(edgeToBorder(bordersToShow));
    DialogShadows::self()->setEnabledBorders(this, d->frameSvgItem->enabledBorders());
}

void PlasmaWindow::resizeEvent(QResizeEvent *e)
{
    QQuickWindow::resizeEvent(e);

    const QSize windowSize = e->size();
    d->frameSvgItem->setSize(windowSize);
    if (d->mainItem) {
        const QSize contentSize = windowSize.shrunkBy(d->margins());
        d->mainItem->setSize(contentSize);
    }
}

void PlasmaWindow::handleFrameChanged()
{
    auto theme = Plasma::Theme();
    const QRegion mask = d->frameSvgItem->frameSvg()->mask();
    KWindowEffects::enableBlurBehind(this, theme.blurBehindEnabled(), mask);
    KWindowEffects::enableBackgroundContrast(this,
                                             theme.backgroundContrastEnabled(),
                                             theme.backgroundContrast(),
                                             theme.backgroundIntensity(),
                                             theme.backgroundSaturation(),
                                             mask);

    if (KX11Extras::compositingActive()) {
        setMask(QRegion());
    } else {
        setMask(mask);
    }
}

void PlasmaWindow::updateMainItemGeometry()
{
    if (!d->mainItem) {
        return;
    }
    const QMargins frameMargins = d->margins();
    const QSize windowSize = size();

    d->mainItem->setX(frameMargins.left());
    d->mainItem->setY(frameMargins.top());

    const QSize contentSize = windowSize.shrunkBy(frameMargins);
    d->mainItem->setSize(contentSize);
}

QMargins PlasmaWindowPrivate::margins() const
{
    return QMargins(frameSvgItem->fixedMargins()->left(),
                    frameSvgItem->fixedMargins()->top(),
                    frameSvgItem->fixedMargins()->right(),
                    frameSvgItem->fixedMargins()->bottom());
}
}

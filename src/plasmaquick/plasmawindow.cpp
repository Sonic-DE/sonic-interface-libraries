/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plasmawindow.h"

#include "../declarativeimports/core/framesvgitem.h"
#include "dialogshadows_p.h"

#include <QMarginsF>
#include <QQuickItem>

#include <kwindoweffects.h>
#include <kx11extras.h>

using namespace PlasmaQuick;
using namespace Plasma;

namespace PlasmaQuick
{
class PlasmaWindowPrivate
{
public:
    QPointer<QQuickItem> mainItem;
    Plasma::FrameSvgItem *frameSvgItem;
};
}

PlasmaWindow::PlasmaWindow(QWindow *parent)
    : QQuickWindow(parent)
    , d(new PlasmaWindowPrivate)
{
    setColor(QColor(Qt::transparent));
    setFlags(Qt::FramelessWindowHint);

    d->frameSvgItem = new Plasma::FrameSvgItem(contentItem());
    d->frameSvgItem->setImagePath(QStringLiteral("dialogs/background"));
    connect(d->frameSvgItem->fixedMargins(), &Plasma::FrameSvgItemMargins::marginsChanged, this, &PlasmaWindow::repositionMainItem);
    connect(d->frameSvgItem, &Plasma::FrameSvgItem::repaintNeeded, this, &PlasmaWindow::handleFrameChanged);

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
        repositionMainItem();
    }
    Q_EMIT mainItemChanged();
}

QQuickItem *PlasmaWindow::mainItem() const
{
    return d->mainItem;
}

void PlasmaWindow::setMainItemSize(const QSize &contentSize)
{
    QSize newSize = contentSize.grownBy(margins());
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
        const QSize contentSize = windowSize.shrunkBy(margins());
        d->mainItem->setSize(contentSize);
    }
    // if we did resize, we'll trigger repaintNeeded, thus triggering any blurring updates
}

void PlasmaWindow::handleFrameChanged()
{
    auto theme = Plasma::Theme();

    // dave, we were doing this before show. Now just do everything at once, and fix kwindow system if it's an issue
    const QRegion mask = d->frameSvgItem->frameSvg()->mask();
    KWindowEffects::enableBlurBehind(this, theme.blurBehindEnabled(), mask);
    KWindowEffects::enableBackgroundContrast(this,
                                             theme.backgroundContrastEnabled(),
                                             theme.backgroundContrast(),
                                             theme.backgroundIntensity(),
                                             theme.backgroundSaturation(),
                                             mask);

    // Dave, is FrameSVG already watching for this changing?
    if (KX11Extras::compositingActive()) {
        setMask(QRegion());
    } else {
        setMask(mask);
    }
}

void PlasmaWindow::repositionMainItem()
{
    if (!d->mainItem) {
        return;
    }
    const QMargins frameMargins = margins();
    d->mainItem->setX(frameMargins.left());
    d->mainItem->setY(frameMargins.top());
}

QMargins PlasmaWindow::margins() const
{
    return QMargins(d->frameSvgItem->fixedMargins()->left(),
                    d->frameSvgItem->fixedMargins()->top(),
                    d->frameSvgItem->fixedMargins()->right(),
                    d->frameSvgItem->fixedMargins()->bottom());
}

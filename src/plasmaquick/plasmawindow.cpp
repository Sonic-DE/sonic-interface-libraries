/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#include "plasmawindow.h"

#include "dialogshadows_p.h"
#include "private/dialogbackground_p.h"

#include <QMarginsF>
#include <QQmlProperty>
#include <QQuickItem>
#include <qpa/qplatformwindow.h> // for QWINDOWSIZE_MAX

#include <KWindowEffects>
#include <KWindowSystem>
#include <KX11Extras>

#include <Plasma/Theme>

#include <QTimer>

using namespace Plasma;

namespace PlasmaQuick
{

// This is a proxy object that connects to the Layout attached property of an item
// it also handles turning properties to proper defaults
// we need a wrapper as QQmlProperty can't disconnect
class LayoutChangedProxy : public QObject
{
    Q_OBJECT
public:
    LayoutChangedProxy(QQuickItem *item);
    QSize minimumSize() const;
    QSize maximumSize() const;
    QSize implicitSize() const;
Q_SIGNALS:
    void implicitSizeChanged();
    void minimumSizeChanged();
    void maximumSizeChanged();

private:
    QQmlProperty m_minimumWidth;
    QQmlProperty m_maximumWidth;
    QQmlProperty m_minimumHeight;
    QQmlProperty m_maximumHeight;
    QQmlProperty m_preferredWidth;
    QQmlProperty m_preferredHeight;
    QPointer<QQuickItem> m_item;
};

class PlasmaWindowPrivate
{
public:
    PlasmaWindowPrivate(PlasmaWindow *_q)
        : q(_q)
    {
    }
    void handleFrameChanged();
    void updateMainItemGeometry();
    PlasmaWindow *q;
    QPointer<QQuickItem> mainItem;
    QScopedPointer<LayoutChangedProxy> layoutChangedProxy;
    DialogBackground *dialogBackground;
    PlasmaWindow::BackgroundHints backgroundHints = PlasmaWindow::StandardBackground;
};

PlasmaWindow::PlasmaWindow(QWindow *parent)
    : QQuickWindow(parent)
    , d(new PlasmaWindowPrivate(this))
{
    setColor(QColor(Qt::transparent));
    setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    d->dialogBackground = new DialogBackground(contentItem());
    d->dialogBackground->setImagePath(QStringLiteral("dialogs/background"));
    connect(d->dialogBackground, &DialogBackground::fixedMarginsChanged, this, [this]() {
        d->updateMainItemGeometry();
        Q_EMIT marginsChanged();
    });
    connect(d->dialogBackground, &DialogBackground::maskChanged, this, [this]() {
        d->handleFrameChanged();
    });

    DialogShadows::self()->addWindow(this, d->dialogBackground->enabledBorders());
}

PlasmaWindow::~PlasmaWindow()
{
}

void PlasmaWindow::setMainItem(QQuickItem *mainItem)
{
    if (d->mainItem == mainItem)
        return;

    if (d->mainItem) {
        d->mainItem->setParentItem(nullptr);
    }

    d->mainItem = mainItem;
    Q_EMIT mainItemChanged();

    if (d->mainItem) {
        mainItem->setParentItem(contentItem());

        // update window to mainItem size hints
        d->layoutChangedProxy.reset(new LayoutChangedProxy(d->mainItem));

        auto updateMaximumSize = [this]() {
            setMaximumSize(d->layoutChangedProxy->maximumSize());
        };
        connect(d->layoutChangedProxy.data(), &LayoutChangedProxy::maximumSizeChanged, this, updateMaximumSize);
        connect(this, &PlasmaWindow::marginsChanged, this, updateMaximumSize);
        updateMaximumSize();

        auto updateMinimumSize = [this]() {
            setMinimumSize(d->layoutChangedProxy->minimumSize().grownBy(margins()));
        };
        connect(d->layoutChangedProxy.data(), &LayoutChangedProxy::minimumSizeChanged, this, updateMinimumSize);
        connect(this, &PlasmaWindow::marginsChanged, this, updateMinimumSize);

        auto updateImplicitSize = [this]() {
            resize(d->layoutChangedProxy->implicitSize().grownBy(margins()));
        };
        connect(d->layoutChangedProxy.data(), &LayoutChangedProxy::implicitSizeChanged, this, updateImplicitSize);
        connect(this, &PlasmaWindow::marginsChanged, this, updateImplicitSize);

        // update item size to window actual size
        d->updateMainItemGeometry();
    } else {
        d->layoutChangedProxy.reset();
    }
}

QQuickItem *PlasmaWindow::mainItem() const
{
    return d->mainItem;
}

static KSvg::FrameSvg::EnabledBorders edgeToBorder(Qt::Edges edges)
{
    KSvg::FrameSvg::EnabledBorders borders = KSvg::FrameSvg::EnabledBorders(0);

    if (edges & Qt::Edge::TopEdge) {
        borders |= KSvg::FrameSvg::EnabledBorder::TopBorder;
    }
    if (edges & Qt::Edge::BottomEdge) {
        borders |= KSvg::FrameSvg::EnabledBorder::BottomBorder;
    }
    if (edges & Qt::Edge::LeftEdge) {
        borders |= KSvg::FrameSvg::EnabledBorder::LeftBorder;
    }
    if (edges & Qt::Edge::RightEdge) {
        borders |= KSvg::FrameSvg::EnabledBorder::RightBorder;
    }

    return borders;
}

static Qt::Edges bordersToEdge(KSvg::FrameSvg::EnabledBorders borders)
{
    Qt::Edges edges;
    if (borders & KSvg::FrameSvg::EnabledBorder::TopBorder) {
        edges |= Qt::Edge::TopEdge;
    }
    if (borders & KSvg::FrameSvg::EnabledBorder::BottomBorder) {
        edges |= Qt::Edge::BottomEdge;
    }
    if (borders & KSvg::FrameSvg::EnabledBorder::LeftBorder) {
        edges |= Qt::Edge::LeftEdge;
    }
    if (borders & KSvg::FrameSvg::EnabledBorder::RightBorder) {
        edges |= Qt::Edge::RightEdge;
    }
    return edges;
}

void PlasmaWindow::setBorders(Qt::Edges bordersToShow)
{
    d->dialogBackground->setEnabledBorders(edgeToBorder(bordersToShow));
    DialogShadows::self()->setEnabledBorders(this, d->dialogBackground->enabledBorders());
    Q_EMIT bordersChanged();
}

Qt::Edges PlasmaWindow::borders()
{
    return bordersToEdge(d->dialogBackground->enabledBorders());
}

void PlasmaWindow::showEvent(QShowEvent *e)
{
    // EWMH states that the state is reset every hide
    // Qt supports external factors setting state before the next show
    if (KWindowSystem::isPlatformX11()) {
        KWindowSystem::setState(winId(), NET::SkipTaskbar | NET::SkipPager | NET::SkipSwitcher);
    }
    QQuickWindow::showEvent(e);
}

void PlasmaWindow::resizeEvent(QResizeEvent *e)
{
    QQuickWindow::resizeEvent(e);
    const QSize windowSize = e->size();
    d->dialogBackground->setSize(windowSize);
    if (d->mainItem) {
        const QSize contentSize = windowSize.shrunkBy(margins());
        d->mainItem->setSize(contentSize);
    }
}

void PlasmaWindowPrivate::handleFrameChanged()
{
    auto theme = Plasma::Theme();
    const QRegion mask = dialogBackground->mask();
    KWindowEffects::enableBlurBehind(q, theme.blurBehindEnabled(), mask);
    KWindowEffects::enableBackgroundContrast(q,
                                             theme.backgroundContrastEnabled(),
                                             theme.backgroundContrast(),
                                             theme.backgroundIntensity(),
                                             theme.backgroundSaturation(),
                                             mask);

    if (KX11Extras::compositingActive()) {
        q->setMask(QRegion());
    } else {
        q->setMask(mask);
    }
}

void PlasmaWindowPrivate::updateMainItemGeometry()
{
    if (!mainItem) {
        return;
    }
    const QMargins frameMargins = q->margins();
    const QSize windowSize = q->size();

    mainItem->setX(frameMargins.left());
    mainItem->setY(frameMargins.top());

    const QSize contentSize = windowSize.shrunkBy(frameMargins);
    mainItem->setSize(contentSize);
}

QMargins PlasmaWindow::margins() const
{
    return QMargins(d->dialogBackground->leftMargin(),
                    d->dialogBackground->topMargin(),
                    d->dialogBackground->rightMargin(),
                    d->dialogBackground->bottomMargin());
}

PlasmaWindow::BackgroundHints PlasmaWindow::backgroundHints() const
{
    return d->backgroundHints;
}

void PlasmaWindow::setBackgroundHints(BackgroundHints hints)
{
    if (d->backgroundHints == hints) {
        return;
    }
    d->backgroundHints = hints;

    auto prefix = QStringLiteral("");
    if (d->backgroundHints == PlasmaWindow::SolidBackground) {
        prefix = QStringLiteral("solid/");
    }
    d->dialogBackground->setImagePath(prefix + QStringLiteral("dialogs/background"));

    Q_EMIT backgroundHintsChanged();
}

qreal PlasmaWindow::topMargin() const
{
    return d->dialogBackground->topMargin();
}

qreal PlasmaWindow::bottomMargin() const
{
    return d->dialogBackground->bottomMargin();
}

qreal PlasmaWindow::leftMargin() const
{
    return d->dialogBackground->leftMargin();
}

qreal PlasmaWindow::rightMargin() const
{
    return d->dialogBackground->rightMargin();
}

LayoutChangedProxy::LayoutChangedProxy(QQuickItem *item)
    : m_item(item)
{
    m_minimumWidth = QQmlProperty(item, QStringLiteral("Layout.minimumWidth"), qmlContext(item));
    m_minimumHeight = QQmlProperty(item, QStringLiteral("Layout.minimumHeight"), qmlContext(item));
    m_maximumWidth = QQmlProperty(item, QStringLiteral("Layout.maximumWidth"), qmlContext(item));
    m_maximumHeight = QQmlProperty(item, QStringLiteral("Layout.maximumHeight"), qmlContext(item));
    m_preferredWidth = QQmlProperty(item, QStringLiteral("Layout.preferredWidth"), qmlContext(item));
    m_preferredHeight = QQmlProperty(item, QStringLiteral("Layout.preferredHeight"), qmlContext(item));

    m_minimumWidth.connectNotifySignal(this, QMetaMethod::fromSignal(&LayoutChangedProxy::minimumSizeChanged).methodIndex());
    m_minimumHeight.connectNotifySignal(this, QMetaMethod::fromSignal(&LayoutChangedProxy::minimumSizeChanged).methodIndex());
    m_maximumWidth.connectNotifySignal(this, QMetaMethod::fromSignal(&LayoutChangedProxy::maximumSizeChanged).methodIndex());
    m_maximumHeight.connectNotifySignal(this, QMetaMethod::fromSignal(&LayoutChangedProxy::maximumSizeChanged).methodIndex());
    m_preferredWidth.connectNotifySignal(this, QMetaMethod::fromSignal(&LayoutChangedProxy::implicitSizeChanged).methodIndex());
    m_preferredHeight.connectNotifySignal(this, QMetaMethod::fromSignal(&LayoutChangedProxy::implicitSizeChanged).methodIndex());
    connect(item, &QQuickItem::implicitWidthChanged, this, &LayoutChangedProxy::implicitSizeChanged);
    connect(item, &QQuickItem::implicitHeightChanged, this, &LayoutChangedProxy::implicitSizeChanged);
}

QSize LayoutChangedProxy::maximumSize() const
{
    QSize size(QWINDOWSIZE_MAX, QWINDOWSIZE_MAX);
    qreal width = m_maximumWidth.read().toReal();
    if (qIsFinite(width) && width > 0) {
        size.setWidth(width);
    }
    qreal height = m_maximumHeight.read().toReal();
    if (qIsFinite(height) && height > 0) {
        size.setHeight(height);
    }
    return size;
}

QSize LayoutChangedProxy::implicitSize() const
{
    QSize size(0, 0);

    // Layout.preferredSize has precedent over implicit in layouts
    // so mimic that behaviour here
    if (m_item) {
        size = QSize(m_item->implicitWidth(), m_item->implicitHeight());
    }
    qreal width = m_preferredWidth.read().toReal();
    if (qIsFinite(width) && width > 0) {
        size.setWidth(width);
    }
    qreal height = m_preferredHeight.read().toReal();
    if (qIsFinite(height) && height > 0) {
        size.setHeight(height);
    }
    return size;
}

QSize LayoutChangedProxy::minimumSize() const
{
    QSize size(0, 0);
    qreal width = m_minimumWidth.read().toReal();
    if (qIsFinite(width) && width > 0) {
        size.setWidth(width);
    }
    qreal height = m_minimumHeight.read().toReal();
    if (qIsFinite(height) && height > 0) {
        size.setHeight(height);
    }
    return size;
}
}

#include "plasmawindow.moc"

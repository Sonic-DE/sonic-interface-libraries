/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>
    SPDX-License-Identifier: LGPL-2.0-or-later
*/

#pragma once

#include <QQuickWindow>
#include <QScopedPointer>

#include <plasmaquick/plasmaquick_export.h>

namespace Plasma
{
class FrameSvgItem;
}

namespace PlasmaQuick
{
class PlasmaWindowPrivate;

/*
 * Creates a QQuickWindow themed in a Plasma style with background
 */
class PLASMAQUICK_EXPORT PlasmaWindow : public QQuickWindow
{
    Q_OBJECT

    /**
     * The main QML item that will be displayed in the Dialog
     */
    Q_PROPERTY(QQuickItem *mainItem READ mainItem WRITE setMainItem NOTIFY mainItemChanged)

    Q_CLASSINFO("DefaultProperty", "mainItem")
public:
    PlasmaWindow(QWindow *parent = nullptr);
    ~PlasmaWindow();

    /**
     * The main QML item that will be displayed in the Dialog
     */
    void setMainItem(QQuickItem *mainItem);

    QQuickItem *mainItem() const;

    /**
     * Resizes the window to the given size plus any additional frameMargins
     */
    void setMainItemSize(const QSize &contentSize);

    /**
     * Changes which rounded corners are shown on the window.
     * Margins remain the same
     * The default is all borders
     */
    void setBorders(Qt::Edges bordersToShow);

    // DICUSSION POINT: image path in public API?

Q_SIGNALS:
    void mainItemChanged();

private:
    void handleResize();
    void handleFrameChanged();
    void repositionMainItem();
    QMargins margins() const;

    const QScopedPointer<PlasmaWindowPrivate> d;
};
}

// TODO:
//  ask about FrameSVG::Edges vs Qt::Edges (IMHO fix FrameSVG?)
//  API review from Marco
//  test non-composited

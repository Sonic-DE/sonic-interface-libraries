/*
    SPDX-FileCopyrightText: 2023 David Edmundson <davidedmundson@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "waylandintegration_p.h"

#include <qpa/qplatformwindow_p.h>

#include <QGuiApplication>
#include <QWindow>

#include <QWaylandClientExtensionTemplate>

class PlasmaShellManager : public QWaylandClientExtensionTemplate<PlasmaShellManager>, public QtWayland::org_kde_plasma_shell
{
public:
    PlasmaShellManager()
        : QWaylandClientExtensionTemplate<PlasmaShellManager>(8)
    {
    }
    QHash<QWindow *, PlasmaWaylandShellIntegration *> m_windows;
};

Q_GLOBAL_STATIC(PlasmaShellManager, privateWaylandIntegrationSelf)

class PlasmaShellSurface : public QtWayland::org_kde_plasma_surface
{
public:
    PlasmaShellSurface(struct ::org_kde_plasma_surface *impl)
        : QtWayland::org_kde_plasma_surface(impl)
    {
    }
    ~PlasmaShellSurface()
    {
        destroy();
    }
};

PlasmaWaylandShellIntegration *PlasmaWaylandShellIntegration::get(QWindow *window)
{
    PlasmaWaylandShellIntegration *&it = privateWaylandIntegrationSelf->m_windows[window];
    if (!it) {
        it = new PlasmaWaylandShellIntegration(window);
    }
    return it;
}

PlasmaWaylandShellIntegration::~PlasmaWaylandShellIntegration()
{
    privateWaylandIntegrationSelf->m_windows.remove(m_window);
}

PlasmaWaylandShellIntegration::PlasmaWaylandShellIntegration(QWindow *window)
    : QObject(window)
    , m_window(window)
{
    m_window->create();
    if (auto waylandWindow = window->nativeInterface<QNativeInterface::Private::QWaylandWindow>()) {
        connect(waylandWindow, &QNativeInterface::Private::QWaylandWindow::surfaceCreated, this, [this] {
            surfaceCreated();
        });
        connect(waylandWindow, &QNativeInterface::Private::QWaylandWindow::surfaceDestroyed, this, [this] {
            surfaceDestroyed();
        });
        if (waylandWindow->surface()) {
            surfaceCreated();
        }
    }
}

void PlasmaWaylandShellIntegration::setPosition(const QPoint &position)
{
    if (position == m_position) {
        return;
    }

    m_position = position;
    if (m_shellSurface) {
        m_shellSurface->set_position(position.x(), position.y());
    }
}

void PlasmaWaylandShellIntegration::setPanelBehavior(QtWayland::org_kde_plasma_surface::panel_behavior panelBehavior)
{
    if (panelBehavior == m_panelBehavior) {
        return;
    }
    m_panelBehavior = panelBehavior;
    if (m_shellSurface) {
        m_shellSurface->set_panel_behavior(panelBehavior);
    }
}

void PlasmaWaylandShellIntegration::setRole(QtWayland::org_kde_plasma_surface::role role)
{
    if (role == m_role) {
        return;
    }
    m_role = role;
    if (m_shellSurface) {
        m_shellSurface->set_role(role);
    }
}

void PlasmaWaylandShellIntegration::setTakesFocus(bool takesFocus)
{
    if (takesFocus == m_takesFocus) {
        return;
    }
    m_takesFocus = takesFocus;
    if (m_shellSurface) {
        m_shellSurface->set_panel_takes_focus(takesFocus);
    }
}

void PlasmaWaylandShellIntegration::surfaceCreated()
{
    struct wl_surface *surface = nullptr;

    if (!privateWaylandIntegrationSelf->isActive()) {
        return;
    }

    if (auto waylandWindow = m_window->nativeInterface<QNativeInterface::Private::QWaylandWindow>()) {
        surface = waylandWindow->surface();
    }

    if (!surface) {
        return;
    }

    m_shellSurface = std::make_unique<PlasmaShellSurface>(privateWaylandIntegrationSelf->get_surface(surface));
    if (m_shellSurface) {
        m_shellSurface->set_position(m_position.x(), m_position.y());
        m_shellSurface->set_panel_takes_focus(m_takesFocus);
        m_shellSurface->set_role(m_role);
        m_shellSurface->set_skip_switcher(true);
        m_shellSurface->set_skip_taskbar(true);
    }
}

void PlasmaWaylandShellIntegration::surfaceDestroyed()
{
    m_shellSurface.reset();
}

#include "moc_waylandintegration_p.cpp"

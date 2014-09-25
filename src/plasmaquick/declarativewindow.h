/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2014  David Edmundson <david@davidedmundson.co.uk>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef DECLARATIVEWINDOW_H
#define DECLARATIVEWINDOW_H

#include <QQuickWindow>
#include <QQmlParserStatus>

#include <plasmaquick_export.h>

class QQuickItem;

namespace PlasmaQuick
{
class DeclarativeWindowPrivate;

class PLASMAQUICK_EXPORT DeclarativeWindow : public QQuickWindow, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChangedProxy)


public:
    DeclarativeWindow(QQuickItem *parent);
    ~DeclarativeWindow();

    bool isComponentComplete() const;

    virtual void classBegin() Q_DECL_OVERRIDE;
    virtual void componentComplete() Q_DECL_OVERRIDE;

    bool isVisible() const;
    void setVisible(bool visible);

Q_SIGNALS:
    void visibleChangedProxy(bool); //redeclaration of QQuickWindow::visibleChanged

private:
    friend class DeclarativeWindowPrivate;
    DeclarativeWindowPrivate *const d;
};

}

#endif // DECLARATIVEWINDOW_H

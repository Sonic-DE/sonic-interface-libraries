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

#ifndef APPLET_P
#define APPLET_P

#include <QObject>

/**
 * QML wrapper for kdelibs Plasma::Theme
 *
 * Exposed as `Theme` in QML.
 */
class Applet : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int minimumWidth READ minimumWidth NOTIFY minimumWidthChanged)
    Q_PROPERTY(int minimumHeight READ minimumHeight NOTIFY minimumHeightChanged)
    Q_PROPERTY(int implicitWidth READ implicitWidth NOTIFY implicitWidthChanged)
    Q_PROPERTY(int implicitHeight READ implicitHeight NOTIFY implicitHeightChanged)
    Q_PROPERTY(int maximumWidth READ maximumWidth NOTIFY maximumWidthChanged)
    Q_PROPERTY(int maximumHeight READ maximumHeight NOTIFY maximumHeightChanged)
    Q_PROPERTY(int compactRepresentation READ compactRepresentation NOTIFY compactRepresentationChanged)
    Q_PROPERTY(int fullRepresentation READ fullRepresentation NOTIFY fullRepresentationChanged)


public:
    Applet(QQmlEngine *parent = 0);
    ~Applet();


    int minimumWidth() const;
    void setMinimumWidth(int width);

    int minimumHeight() const;
    void setMinimumHeight(int width);



    int implicitWidth() const;
    void setImplicitWidth(int width);

    int implicitHeight() const;
    void setImplicitHeight(int width);



    int maximumWidth() const;
    void setMaximumWidth(int width);

    int maximumWidth() const;
    void setMaximumWidth(int width);



    QQmlComponent *compactRepresentation();
    void setCompactRepresentation(QQmlComponent *component);

    QQmlComponent *fullRepresentation();
    void setFullRepresentation(QQmlComponent *component);

Q_SIGNALS:
    void minimumWidthChanged(int width);
    void minimumHeightChanged(int height);

    void maximumWidthChanged(int width);
    void maximumWidthChanged(int height);

    void implicitWidthChanged(int width);
    void implicitHeightChanged(int height);

    void compactRepresentationChanged(QQmlComponent *compactRepresentation);
    void fullRepresentation(QQmlComponent *fullRepresentation);

private:
    int m_minimumWidth;
    int m_minimumHeight;
    int m_maximumWidth;
    int m_maximumHeight;
    int m_implicitWidth;
    int m_implicitHeight;

    QWeakPointer<QQmlComponent *> m_compactRepresentation;
    QWeakPointer<QQmlComponent *> m_fullRepresentation;
};

#endif

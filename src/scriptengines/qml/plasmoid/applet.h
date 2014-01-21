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

#ifndef APPLETCOMPONENT_P
#define APPLETCOMPONENT_P

#include <QQuickItem>
#include <QWeakPointer>
#include <QQmlComponent>
#include <QTimer>

class QQmlComponent;

namespace Plasma {
    class Applet;
}

namespace KDeclarative {
    class QmlObject;
}

/**
 * CompactRepresentation and FullRepresentation will have a SizeHint attached object
 * example:
 * Applet {
 *     compactRepresentation: Component {
 *         Item {
 *             SizeHint.minimumWidth: 200
 *             ...
 *         }
 *     }
 * }
 */
class SizeHintAttachedType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int minimumWidth READ minimumWidth WRITE setMinimumWidth NOTIFY minimumWidthChanged)
    Q_PROPERTY(int minimumHeight READ minimumHeight WRITE setMinimumHeight NOTIFY minimumHeightChanged)
    Q_PROPERTY(int implicitWidth READ implicitWidth WRITE setImplicitWidth NOTIFY implicitWidthChanged)
    Q_PROPERTY(int implicitHeight READ implicitHeight WRITE setImplicitHeight NOTIFY implicitHeightChanged)
    Q_PROPERTY(int maximumWidth READ maximumWidth WRITE setMaximumWidth NOTIFY maximumWidthChanged)
    Q_PROPERTY(int maximumHeight READ maximumHeight WRITE setMaximumHeight NOTIFY maximumHeightChanged)

public:
    SizeHintAttachedType(QObject *parent);
    ~SizeHintAttachedType();

    int minimumWidth() const;
    void setMinimumWidth(int width);

    int minimumHeight() const;
    void setMinimumHeight(int height);



    int implicitWidth() const;
    void setImplicitWidth(int width);

    int implicitHeight() const;
    void setImplicitHeight(int height);



    int maximumWidth() const;
    void setMaximumWidth(int width);

    int maximumHeight() const;
    void setMaximumHeight(int height);

    static SizeHintAttachedType *qmlAttachedProperties(QObject *object)
    {
        return new SizeHintAttachedType(object);
    }
Q_SIGNALS:
    void minimumWidthChanged(int width);
    void minimumHeightChanged(int height);

    void maximumWidthChanged(int width);
    void maximumHeightChanged(int height);

    void implicitWidthChanged(int width);
    void implicitHeightChanged(int height);


private:
    int m_minimumWidth;
    int m_minimumHeight;
    int m_implicitWidth;
    int m_implicitHeight;
    int m_maximumWidth;
    int m_maximumHeight;
};
QML_DECLARE_TYPE(SizeHintAttachedType)
QML_DECLARE_TYPEINFO(SizeHintAttachedType, QML_HAS_ATTACHED_PROPERTIES)



class Applet : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(int switchWidth READ switchWidth WRITE setSwitchWidth NOTIFY switchWidthChanged)
    Q_PROPERTY(int switchHeight READ switchHeight WRITE setSwitchHeight NOTIFY switchHeightChanged)

    Q_PROPERTY(QQmlComponent *compactRepresentation READ compactRepresentation WRITE setCompactRepresentation NOTIFY compactRepresentationChanged)
    Q_PROPERTY(QObject *compactRepresentationItem READ compactRepresentationItem NOTIFY compactRepresentationItemChanged)

    Q_PROPERTY(QQmlComponent *fullRepresentation READ fullRepresentation WRITE setFullRepresentation NOTIFY fullRepresentationChanged)
    Q_PROPERTY(QObject *fullRepresentationItem READ fullRepresentationItem NOTIFY fullRepresentationItemChanged)


    /**
     * this is supposed to be either one between compactRepresentation or fullRepresentation
     */
    Q_PROPERTY(QQmlComponent *preferredRepresentation READ preferredRepresentation WRITE setPreferredRepresentation NOTIFY preferredRepresentationChanged)

    //FIXME: is it wise to expose this?
    Q_PROPERTY(QQmlComponent *compactRepresentation READ compactRepresentation WRITE setCompactRepresentation NOTIFY compactRepresentationChanged)

    ///////////////Wrapping main plasmoid api
    /**
     * True when the applet is showing its full representation. either as the main only view, or in a popup.
     * Setting it will open or close the popup if the plasmoid is iconified, however it won't have effect if the applet is open
     */
    Q_PROPERTY(bool expanded WRITE setExpanded READ isExpanded NOTIFY expandedChanged)

public:
    Applet(QQuickItem *parent = 0);
    ~Applet();

    int switchWidth() const;
    void setSwitchWidth(int width);

    int switchHeight() const;
    void setSwitchHeight(int width);


    QQmlComponent *compactRepresentation();
    void setCompactRepresentation(QQmlComponent *component);

    QQmlComponent *fullRepresentation();
    void setFullRepresentation(QQmlComponent *component);

    QQmlComponent *preferredRepresentation();
    void setPreferredRepresentation(QQmlComponent *component);


    QQmlComponent *compactRepresentationExpander();
    void setCompactRepresentationExpander(QQmlComponent *component);

    QObject *compactRepresentationItem();
    QObject *fullRepresentationItem();
    QObject *compactRepresentationExpanderItem();

    //////////////////Wrapping main plasmoid API
    bool isExpanded() const;
    void setExpanded(bool expanded);

    //Reimplemented
    virtual void classBegin();
    virtual void componentComplete();
    
Q_SIGNALS:
    void switchWidthChanged(int width);
    void switchHeightChanged(int height);

    void compactRepresentationChanged(QQmlComponent *compactRepresentation);
    void fullRepresentationChanged(QQmlComponent *fullRepresentation);
    void preferredRepresentationChanged(QQmlComponent *preferredRepresentation);

    void compactRepresentationExpanderChanged(QQmlComponent *compactRepresentationExpander);

    void compactRepresentationItemChanged(QObject *compactRepresentationItem);
    void fullRepresentationItemChanged(QObject *fullRepresentationItem);
    void compactRepresentationExpanderItemChanged(QObject *compactRepresentationExpanderItem);

    //Plasmoid api
    void expandedChanged(bool expanded);

protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry);

    QObject *createCompactRepresentationItem();
    QObject *createFullRepresentationItem();
    QObject *createCompactRepresentationExpanderItem();

    //look into item, and return the Layout attached property, if found
    void connectLayoutAttached(QObject *item);
    void propagateSizeHint(const QByteArray &layoutProperty);

private Q_SLOTS:
    void compactRepresentationCheck();

    //handlers of Layout signals
    void minimumWidthChanged();
    void minimumHeightChanged();
    void preferredWidthChanged();
    void preferredHeightChanged();
    void maximumWidthChanged();
    void maximumHeightChanged();
    void fillWidthChanged();
    void fillHeightChanged();

private:
    int m_switchWidth;
    int m_switchHeight;

    QWeakPointer<QQmlComponent> m_compactRepresentation;
    QWeakPointer<QQmlComponent> m_fullRepresentation;
    QWeakPointer<QQmlComponent> m_preferredRepresentation;
    QWeakPointer<QQmlComponent> m_compactRepresentationExpander;

    QWeakPointer<QObject> m_compactRepresentationItem;
    QWeakPointer<QObject> m_fullRepresentationItem;
    QWeakPointer<QObject> m_compactRepresentationExpanderItem;
    QWeakPointer<QObject> m_currentRepresentationItem;

    //Attached layout objects: own and the representation's one
    QWeakPointer<QObject> m_representationLayout;
    QWeakPointer<QObject> m_ownLayout;

    QTimer m_compactRepresentationCheckTimer;

    QQmlEngine *m_engine;
    Plasma::Applet *m_applet;
    KDeclarative::QmlObject *m_qmlObject;

    bool m_expanded : 1;
};


#endif

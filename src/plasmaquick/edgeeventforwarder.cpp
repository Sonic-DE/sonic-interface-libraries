// license

#include "edgeeventforwarder.h"

#include <QCoreApplication>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QEvent>
#include <QMouseEvent>
#include <QWindow>

using namespace PlasmaQuick;

class EdgeEventForwarderPrivate
{
public:
    QWindow *window;
    QMargins margins;
    Qt::Edges activeEdges;
    bool mainItemContainsPosition(const QPointF &position);
    QPointF positionAdjustedForMainItem(const QPointF &position);
};

EdgeEventForwarder::EdgeEventForwarder(QWindow *parent)
    : QObject(parent)
    , d(new EdgeEventForwarderPrivate)
{
    d->window = parent;
    d->window->installEventFilter(this);
}

EdgeEventForwarder::~EdgeEventForwarder()
{
    d->window->removeEventFilter(this);
}

void EdgeEventForwarder::setMargins(const QMargins &margins)
{
    d->margins = margins;
}

QMargins EdgeEventForwarder::margins()
{
    return d->margins;
}

void EdgeEventForwarder::setActiveEdges(Qt::Edges edges)
{
    d->activeEdges = edges;
}

Qt::Edges EdgeEventForwarder::activeEdges()
{
    return d->activeEdges;
}

bool EdgeEventForwarder::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)

    switch (event->type()) {
    case QEvent::MouseMove:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease: {
        QMouseEvent *me = static_cast<QMouseEvent *>(event);
        // don't mess with position if the cursor is actually outside the view:
        // somebody is doing a click and drag that must not break when the cursor is outside

        if (d->window->geometry().contains(me->globalPosition().toPoint()) && !d->mainItemContainsPosition(me->scenePosition())) {
            QMouseEvent me2(me->type(),
                            d->positionAdjustedForMainItem(me->scenePosition()),
                            d->positionAdjustedForMainItem(me->scenePosition()),
                            d->positionAdjustedForMainItem(me->scenePosition()) + d->window->position(),
                            me->button(),
                            me->buttons(),
                            me->modifiers());

            if (d->window->isVisible()) {
                QCoreApplication::sendEvent(d->window, &me2);
            }
            return true;
        }
        break;
    }

    case QEvent::Wheel: {
        QWheelEvent *we = static_cast<QWheelEvent *>(event);

        const QPoint pos = we->position().toPoint();

        if (!d->mainItemContainsPosition(pos)) {
            QWheelEvent we2(d->positionAdjustedForMainItem(pos),
                            d->positionAdjustedForMainItem(pos) + d->window->position(),
                            we->pixelDelta(),
                            we->angleDelta(),
                            we->buttons(),
                            we->modifiers(),
                            we->phase(),
                            false /*inverted*/);

            if (d->window->isVisible()) {
                QCoreApplication::sendEvent(this, &we2);
            }
            return true;
        }
        break;
    }

    case QEvent::DragEnter: {
        QDragEnterEvent *de = static_cast<QDragEnterEvent *>(event);
        if (!d->mainItemContainsPosition(de->position())) {
            QDragEnterEvent de2(d->positionAdjustedForMainItem(de->position()).toPoint(),
                                de->possibleActions(),
                                de->mimeData(),
                                de->buttons(),
                                de->modifiers());

            if (d->window->isVisible()) {
                QCoreApplication::sendEvent(this, &de2);
            }
            return true;
        }
        break;
    }
    // DragLeave just works
    case QEvent::DragLeave:
        break;
    case QEvent::DragMove: {
        QDragMoveEvent *de = static_cast<QDragMoveEvent *>(event);
        if (!d->mainItemContainsPosition(de->position())) {
            QDragMoveEvent de2(d->positionAdjustedForMainItem(de->position()).toPoint(), de->possibleActions(), de->mimeData(), de->buttons(), de->modifiers());

            if (d->window->isVisible()) {
                QCoreApplication::sendEvent(this, &de2);
            }
            return true;
        }
        break;
    }
    case QEvent::Drop: {
        QDropEvent *de = static_cast<QDropEvent *>(event);
        if (!d->mainItemContainsPosition(de->position())) {
            QDropEvent de2(d->positionAdjustedForMainItem(de->position()), de->possibleActions(), de->mimeData(), de->buttons(), de->modifiers());

            if (d->window->isVisible()) {
                QCoreApplication::sendEvent(this, &de2);
            }
            return true;
        }
        break;
    }

    default:
        break;
    }
    return false;
}

bool EdgeEventForwarderPrivate::mainItemContainsPosition(const QPointF &position)
{
    // TODO active margins only
    const QRectF itemRect = QRectF(QPointF(0, 0), window->size()).marginsRemoved(margins);
    return itemRect.contains(position);
}

QPointF EdgeEventForwarderPrivate::positionAdjustedForMainItem(const QPointF &position)
{
    // TODO active margins only

    const QRectF itemRect = QRectF(QPointF(0, 0), window->size()).marginsRemoved(margins);
    return QPointF(qBound(itemRect.left(), position.x(), itemRect.right()), qBound(itemRect.top(), position.y(), itemRect.bottom()));
}

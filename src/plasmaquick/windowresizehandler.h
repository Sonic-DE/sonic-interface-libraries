#pragma once

#include <QMargins>
#include <QObject>
#include <memory>

#include <plasmaquick/plasmaquick_export.h>

class QWindow;

class WindowResizeHandlerPrivate;

/**
 * @brief The EdgeEventForwarder class
 * This class forwards edge events to be replayed within the given margin
 * This is useful if children do not touch the edge of a window, but want to get input events
 */

namespace PlasmaQuick
{

class PLASMAQUICK_EXPORT WindowResizeHandler : public QObject
{
    Q_OBJECT
public:
    WindowResizeHandler(QWindow *parent);
    ~WindowResizeHandler();

    void setMargins(const QMargins &margins);
    QMargins margins() const;

    void setActiveEdges(Qt::Edges edges);
    Qt::Edges activeEdges() const;

    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    std::unique_ptr<WindowResizeHandlerPrivate> d;
};

}

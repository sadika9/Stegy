#include "sgraphicsview.h"

#include <QWheelEvent>

SGraphicsView::SGraphicsView(QWidget *parent) :
    QGraphicsView(parent)
{

}

void SGraphicsView::wheelEvent(QWheelEvent *e)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    double scaleFactor = 1.15;
    if (e->delta() > 0)
    {
        // zoom in
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        // zoom out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

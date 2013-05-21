#ifndef SGRAPHICSVIEW_H
#define SGRAPHICSVIEW_H

/**
  SGraphicsView is based on the example on http://www.qtcentre.org
  visit http://www.qtcentre.org/wiki/index.php?title=QGraphicsView:_Smooth_Panning_and_Zooming
  for more information.
  */

#include <QGraphicsView>

class SGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SGraphicsView(QWidget *parent = 0);
    
protected:
    virtual void wheelEvent(QWheelEvent *e);
};

#endif // SGRAPHICSVIEW_H

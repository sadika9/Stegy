/*
Copyright (C) 2013  Sadika Sumanapala

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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

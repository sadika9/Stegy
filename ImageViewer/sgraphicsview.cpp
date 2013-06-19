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

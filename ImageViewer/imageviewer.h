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

#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QDialog>

namespace Ui {
class ImageViewer;
}

class ImageViewer : public QDialog
{
    Q_OBJECT
    
public:
    explicit ImageViewer(bool resize = false, QImage image = QImage(), QWidget *parent = 0);
    ~ImageViewer();

    QImage image() const;
    void setImage(const QImage &image);

    bool resizeWindow() const;
    void setResizeWindow(bool resizeWindow);

public slots:
    void updateImageDetails();
    void updateView();
    void updateWindowSize();

signals:
    void imageChanged();
    
private:
    Ui::ImageViewer *ui;

    QImage m_image;

    bool m_resizeWindow;
};

#endif // IMAGEVIEWER_H

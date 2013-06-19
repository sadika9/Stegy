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

#include "imageviewer.h"
#include "ui_imageviewer.h"

#include <QGraphicsPixmapItem>

ImageViewer::ImageViewer(bool resize, QImage image, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageViewer),
    m_image(image),
    m_resizeWindow(resize)
{
    ui->setupUi(this);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    setWindowTitle(tr("Image Viewer"));

    ui->graphicsView->setScene(new QGraphicsScene);

    updateImageDetails();
    updateView();

    connect(this, &ImageViewer::imageChanged, this, &ImageViewer::updateImageDetails);
    connect(this, &ImageViewer::imageChanged, this, &ImageViewer::updateView);
    connect(this, &ImageViewer::imageChanged, this, &ImageViewer::updateWindowSize);
}

ImageViewer::~ImageViewer()
{
    delete ui;
}

void ImageViewer::updateImageDetails()
{
    QString text = QString("%1 x %2").
            arg(QString::number(m_image.width())).
            arg(QString::number(m_image.height()));

    ui->imageDetailsLabel->setText(text);
}

void ImageViewer::updateView()
{
    ui->graphicsView->scene()->clear();

    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_image));

    ui->graphicsView->scene()->addItem(pixItem);
}

void ImageViewer::updateWindowSize()
{
    if (m_resizeWindow)
        resize(m_image.width() + 2, m_image.height() + ui->imageDetailsLabel->height());
}

QImage ImageViewer::image() const
{
    return m_image;
}

void ImageViewer::setImage(const QImage &image)
{
    m_image = image;

    emit imageChanged();
}

bool ImageViewer::resizeWindow() const
{
    return m_resizeWindow;
}

void ImageViewer::setResizeWindow(bool resizeWindow)
{
    m_resizeWindow = resizeWindow;
}

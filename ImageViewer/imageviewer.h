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

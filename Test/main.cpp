#include <QApplication>
#include "imageviewer.h"
#include <QDebug>
#include "lsbcrypt.h"

#include <QImageWriter>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QImage cover("/home/sadika/Pictures/Chrysanthemum.jpg");  //ImageViewer wc(true, cover);wc.show();

    cover = cover.convertToFormat(QImage::Format_ARGB32);
    QImage secret("/home/sadika/Pictures/cat.jpg");


    lsb::LsbCrypt cr("ccv");
    cr.setBitsPerByte(8);
    //QImage i = cr.encryptImage(cover, secret);
    QImage i = cr.encryptString(cover, QString("hello world"));
    cr.saveImage("/home/sadika/rrr.png", i);

    lsb::LsbCrypt dr("ccv");
    char format[] = "png";
    QImage input =  QImage("/home/sadika/rrr", format);
    //QImage j = dr.decryptImage(input);
    QString st = dr.decryptString(input);
    qDebug() << st;

    qDebug() << dr.getFormat(input);


    return 0;




    ImageViewer iw2(true);
    //iw2.setImage(j);
    iw2.show();


    return a.exec();
}

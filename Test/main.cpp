#include <QApplication>
#include "imageviewer.h"
#include <QDebug>
#include "lsbcrypt.h"

#include <QImageWriter>

#include <opencv2/opencv.hpp>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*QImage cover("/home/sadika/Pictures/Chrysanthemum.jpg");  //ImageViewer wc(true, cover);wc.show();

    cover = cover.convertToFormat(QImage::Format_ARGB32);
    QImage secret("/home/sadika/Pictures/cat.jpg");


    lsb::LsbCrypt cr("ccv");
    cr.setBitsPerByte(1);
    QImage i = cr.hideImage(cover, secret);
    //QImage i = cr.encryptString(cover, QString("hello world"));
    cr.saveImage("/home/sadika/rrr.png", i);

    lsb::LsbCrypt dr("ccv");
    char format[] = "png";
    QImage input =  QImage("/home/sadika/rrr", format);
    QImage j = dr.unhideImage(input);
    //QString st = dr.decryptString(input); qDebug() << st;


    qDebug() << dr.getFormat(input);


    //return 0;




    ImageViewer iw2(true);
    iw2.setImage(j);
    iw2.show();*/


    using namespace cv;
    // Read image
    Mat img = imread("/home/sadika/Pictures/Chrysanthemum.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    // Make sure the both image dimensions are a multiple of 2
    Mat img2;
    int w = img.cols;
    int h = img.rows;
    int w2,h2;
    if (w % 2 == 0)
        w2 = w;
    else
        w2 = w+1;
    if (h % 2 == 0)
        h2 = h;
    else
        h2 = h+1;
    copyMakeBorder(img, img2, 0, h2-h, 0, w2-w, IPL_BORDER_REPLICATE);

    // Grayscale image is 8bits per pixel,
    // but dct() method wants float values!
    Mat img3 = Mat( img2.rows, img2.cols, CV_64F);
    img2.convertTo(img3, CV_64F);
    imwrite("/home/sadika/orig.png", img3);

    // Let's do the DCT now: image => frequencies
    Mat freq;
    dct(img3, freq);

    // Save a visualization of the DCT coefficients
    imwrite("/home/sadika/dct.png", freq);

    for (int Start=100; Start>0; Start-=1)
    {
        // Set some frequencies to 0
        for (int y=Start; y<freq.rows; y++)
        {
            for (int x=Start; x<freq.cols; x++)
            {
                freq.at<double>(y,x) = 0.0;
            }
        }

        // Do inverse DCT: (some low) frequencies => image
        Mat dst;
        idct(freq, dst);

        // Show frame nr
        char txt[100];
        sprintf(txt, "%04d", Start);
        cv::putText( dst, txt, Point(10,20),
         CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0,0,0) );

        // Save visualization of reconstructed image where
        //we have thrown away some of the high frequencies
        char fname[500];
        sprintf(fname, "/home/sadika/idct_%04d.png", Start);
        imwrite(fname, dst);
    }

return 0;
    return a.exec();
}

#include "decryptwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DecryptWidget w;
    w.show();
    
    return a.exec();
}

#include "encryptwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EncryptWidget w;
    w.show();
    
    return a.exec();
}

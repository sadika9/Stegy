#include "stegymain.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StegyMain w;
    w.show();
    
    return a.exec();
}

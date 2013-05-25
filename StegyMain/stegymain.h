#ifndef STEGYMAIN_H
#define STEGYMAIN_H

#include <QMainWindow>
#include <QSignalMapper>

namespace Ui {
class StegyMain;
}

class StegyMain : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit StegyMain(QWidget *parent = 0);
    ~StegyMain();

private slots:
    void setPage(QString action);
    void showMessage(QString message);
    void about();
    
private:
    Ui::StegyMain *ui;

    QSignalMapper *signalMapper;
};

#endif // STEGYMAIN_H

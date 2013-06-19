#include "stegymain.h"
#include "ui_stegymain.h"
#include <QMessageBox>
#include "aboutdialog.h"

StegyMain::StegyMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::StegyMain)
{
    ui->setupUi(this);

    signalMapper = new QSignalMapper(this);

    signalMapper->setMapping(ui->actionEncrypt, "ACTION_ENCRYPT");
    signalMapper->setMapping(ui->actionDecrypt, "ACTION_DECRYPT");

    connect(ui->actionEncrypt, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionDecrypt, SIGNAL(triggered()), signalMapper, SLOT(map()));

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(setPage(QString)));

    connect(ui->encryptPage, &EncryptWidget::statusMessage, this, &StegyMain::showMessage);
    connect(ui->decryptPage, &DecryptWidget::statusMessage, this, &StegyMain::showMessage);

    connect(ui->actionAbout, &QAction::triggered, this, &StegyMain::about);

    setWindowTitle(tr("Stegy"));
    setWindowIcon(QIcon(":/Icons/61.png"));

    statusBar()->showMessage(tr("Ready"), 2000);
}

StegyMain::~StegyMain()
{
    delete ui;
}

void StegyMain::setPage(QString action)
{
    if (action == "ACTION_ENCRYPT")
    {
        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(1);
    }
}

void StegyMain::showMessage(QString message)
{
    statusBar()->showMessage(message, 2000);
}

void StegyMain::about()
{
    AboutDialog *about = new AboutDialog(this);
    about->show();
}

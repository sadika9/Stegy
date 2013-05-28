#include "stegymain.h"
#include "ui_stegymain.h"
#include <QMessageBox>

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
    QMessageBox::about(this,
                           tr("About Stegy"),
                           tr("A simple steganography tool by Sadika Sumanapala."));
    /*QMessageBox::about(this,
                       tr("About Stegy"),
                       tr("This is simple steganography software."
                           "This application contains code copyrighted by André Somers. For this code, the following license applies:\n"
                          "Copyright (c) 2011, Andre Somers""\n"
                          "All rights reserved.""\n""\n"

                          "Redistribution and use in source and binary forms, with or without""\n"
                          "modification, are permitted provided that the following conditions are met:""\n"
                          "    * Redistributions of source code must retain the above copyright""\n"
                          "      notice, this list of conditions and the following disclaimer.""\n"
                          "    * Redistributions in binary form must reproduce the above copyright""\n"
                          "      notice, this list of conditions and the following disclaimer in the""\n"
                          "      documentation and/or other materials provided with the distribution.""\n"
                          "    * Neither the name of the Rathenau Instituut, Andre Somers nor the""\n"
                          "      names of its contributors may be used to endorse or promote products""\n"
                          "      derived from this software without specific prior written permission.""\n""\n"

                          " THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND""\n"
                          "ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED""\n"
                          "WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE""\n"
                          "DISCLAIMED. IN NO EVENT SHALL ANDRE SOMERS BE LIABLE FOR ANY""\n"
                          "DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES""\n"
                          "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;""\n"
                          " LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND""\n"
                          "ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT""\n"
                          "(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS""\n"
                          "SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\"))""\n"));*/
}

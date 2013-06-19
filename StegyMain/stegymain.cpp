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

    signalMapper->setMapping(ui->actionEncode, "ACTION_ENCODE");
    signalMapper->setMapping(ui->actionDecode, "ACTION_DECODE");

    connect(ui->actionEncode, SIGNAL(triggered()), signalMapper, SLOT(map()));
    connect(ui->actionDecode, SIGNAL(triggered()), signalMapper, SLOT(map()));

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(setPage(QString)));

    connect(ui->encodePage, &EncodeWidget::statusMessage, this, &StegyMain::showMessage);
    connect(ui->decodePage, &DecodeWidget::statusMessage, this, &StegyMain::showMessage);

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
    if (action == "ACTION_ENCODE")
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
    AboutDialog about;
    about.exec();
}

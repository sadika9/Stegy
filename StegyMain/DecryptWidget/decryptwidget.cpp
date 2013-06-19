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

#include "decryptwidget.h"
#include "ui_decryptwidget.h"

#include <QtWidgets>
#include "lsbcrypt.h"
#include "../lsbcrypterrormessages.h"

#include <QDebug>

DecryptWidget::DecryptWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DecryptWidget)
{
    ui->setupUi(this);

    ui->previewButton->setEnabled(false);
    ui->decryptButton->setEnabled(false);

    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->setScene(new QGraphicsScene);

    ui->plainTextEdit->setReadOnly(true);

    ui->plainTextEdit->setVisible(false);
    ui->graphicsView->setVisible(false);
    ui->saveButton->setVisible(false);

    connect(ui->browseButton, &QPushButton::clicked, this, &DecryptWidget::openImage);
    connect(ui->previewButton, &QPushButton::clicked, this, &DecryptWidget::previewStegoImage);
    connect(ui->decryptButton, &QPushButton::clicked, this, &DecryptWidget::decryptImage);
    connect(ui->saveButton, &QPushButton::clicked, this, &DecryptWidget::saveSecretData);
}

DecryptWidget::~DecryptWidget()
{
    delete ui;
}

void DecryptWidget::openImage()
{
    QString fileName =
            QFileDialog::getOpenFileName(this,
                                         tr("Open Stego Image"),
                                         QDir::currentPath(),
                                         tr("Portable Network Graphics (*.png);;Image Files (*.png *.bmp);;All Files (*.*)"));

    if (fileName.isNull())
    {
        //QMessageBox::warning(this, tr("Invalid file"), tr("Invalid file."));
        ui->previewButton->setEnabled(false);
        ui->decryptButton->setEnabled(false);
        ui->saveButton->setVisible(false);
        ui->plainTextEdit->setVisible(false);
        ui->graphicsView->setVisible(false);
        ui->stegoPathEdit->setText("");
        ui->passphraseEdit->setText("");

        return;
    }

    if (!m_stegoImage.load(fileName))
    {
        QMessageBox::warning(this, tr("Invalid image"), tr("Invalid image file or format not supported."));
        ui->previewButton->setEnabled(false);
        ui->decryptButton->setEnabled(false);
        return;
    }

    ui->stegoPathEdit->setText(fileName);
    ui->previewButton->setEnabled(true);
    ui->decryptButton->setEnabled(true);

    ui->saveButton->setVisible(false);
    ui->plainTextEdit->setVisible(false);
    ui->graphicsView->setVisible(false);

    emit statusMessage(tr("Stego image loaded."));
}

void DecryptWidget::saveSecretData()
{
    if (m_format == lsb::LsbCrypt::Format_Text)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Text"),
                                                        QDir::currentPath(),
                                                        tr("Text files (*.txt)"));

        QFile file(fileName);
        //if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            //QMessageBox::warning(this, tr("Cannot save the file"), tr("An error occurred while attempting to save the file."));

        file.open(QIODevice::WriteOnly | QIODevice::Text);

        QTextStream out(&file);
        out << m_secretText;
        file.close();

        emit statusMessage(tr("File Saved."));
    }
    else if (m_format == lsb::LsbCrypt::Format_Image)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Image"),
                                                        QDir::currentPath(),
                                                        tr("Portable Network Graphics (*.png);;JPEG Image (*.jpg);;All Images (*.png *.xpm *.jpg)"));

        m_secretImage.save(fileName);
        //if(!m_secretImage.save(fileName))
            //QMessageBox::warning(this, tr("Cannot save the file"), tr("An error occurred while attempting to save the file."));

        emit statusMessage(tr("File Saved."));
    }
}

void DecryptWidget::previewStegoImage()
{
    ui->graphicsView->scene()->clear();
    QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_stegoImage));
    ui->graphicsView->scene()->addItem(pixItem);

    ui->graphicsView->setVisible(true);
    ui->plainTextEdit->setVisible(false);
    ui->stackedWidget->setCurrentIndex(1);
}

void DecryptWidget::decryptImage()
{
    using namespace lsb;

    if (m_stegoImage.isNull())
    {
        QMessageBox::warning(this, tr("Invalid file"), tr("Invalid file. Could not decrypt"));
        return;
    }

    ui->plainTextEdit->setVisible(false);
    ui->graphicsView->setVisible(false);

    m_format = LsbCrypt::getFormat(m_stegoImage);

    LsbCrypt crypt(ui->passphraseEdit->text());

    if (m_format == LsbCrypt::Format_Text)
    {
        m_secretText = crypt.unhideString(m_stegoImage);

        if (crypt.lastError() == LsbCrypt::Error_NoError)
        {
            ui->plainTextEdit->setPlainText(m_secretText);


            ui->graphicsView->setVisible(false);
            ui->plainTextEdit->setVisible(true);
            ui->stackedWidget->setCurrentIndex(0);

            ui->saveButton->setVisible(true);

            emit statusMessage(tr("Decoded."));
        }
    }
    else if (m_format == LsbCrypt::Format_Image)
    {
        m_secretImage = crypt.unhideImage(m_stegoImage);

        if (crypt.lastError() == LsbCrypt::Error_NoError)
        {
            ui->graphicsView->scene()->clear();
            QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_secretImage));
            ui->graphicsView->scene()->addItem(pixItem);

            ui->graphicsView->setVisible(true);
            ui->plainTextEdit->setVisible(false);
            ui->stackedWidget->setCurrentIndex(1);

            ui->saveButton->setVisible(true);

            emit statusMessage(tr("Decoded."));
        }
    }
    lsbCryptErrorMessages(crypt.lastError(), this);
}

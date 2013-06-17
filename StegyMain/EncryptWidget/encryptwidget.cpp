#include "encryptwidget.h"
#include "ui_encryptwidget.h"

#include <QtWidgets>
#include "lsbcrypt.h"
#include "../lsbcrypterrormessages.h"

EncryptWidget::EncryptWidget(QWidget *parent) :
    QWidget(parent),
    hasCoverImage(false),
    hasSecretImage(false),
    hasSecretText(false),
    ui(new Ui::EncryptWidget)
{
    ui->setupUi(this);

    ui->imageRadio->setChecked(true);
    ui->formatStack->setCurrentIndex(1);

    ui->graphicsView->setScene(new QGraphicsScene);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    hidePreview();
    disableButtons();
    setupConnections();
}

EncryptWidget::~EncryptWidget()
{
    delete signalMapper;
    delete ui;
}

void EncryptWidget::showPreview(QString image)
{

    if (image == "CoverPreviewButton")
    {
        ui->graphicsView->scene()->clear();
        QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_coverImage));
        ui->graphicsView->scene()->addItem(pixItem);

        ui->graphicsView->setVisible(true);
        ui->hidePreviewButton->setVisible(true);

        QList<int> sizes = ui->splitter->sizes();
        sizes.replace(1, ui->graphicsView->scene()->width());
        ui->splitter->setSizes(sizes);
    }
    else if (image == "SecretPreviewButton")
    {
        ui->graphicsView->scene()->clear();
        QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_secretImage));
        ui->graphicsView->scene()->addItem(pixItem);

        ui->graphicsView->setVisible(true);
        ui->hidePreviewButton->setVisible(true);

        QList<int> sizes = ui->splitter->sizes();
        sizes.replace(1, ui->graphicsView->scene()->width());
        ui->splitter->setSizes(sizes);
    }
    else if (image == "EncryptButton")
    {
        ui->graphicsView->scene()->clear();
        QGraphicsPixmapItem *pixItem = new QGraphicsPixmapItem(QPixmap::fromImage(m_stegoImage));
        ui->graphicsView->scene()->addItem(pixItem);

        ui->graphicsView->setVisible(true);
        ui->hidePreviewButton->setVisible(true);

        QList<int> sizes = ui->splitter->sizes();
        sizes.replace(1, ui->graphicsView->scene()->width());
        ui->splitter->setSizes(sizes);
    }
}

void EncryptWidget::hidePreview()
{
    ui->graphicsView->setVisible(false);
    ui->hidePreviewButton->setVisible(false);

    QList<int> sizes = ui->splitter->sizes();
    sizes.replace(1, 0);
    ui->splitter->setSizes(sizes);
}

void EncryptWidget::formatChanged()
{
    if (ui->imageRadio->isChecked())
    {
        m_format = lsb::LsbCrypt::Format_Image;
        ui->formatStack->setCurrentIndex(1);
    }
    else
    {
        m_format = lsb::LsbCrypt::Format_Text;
        ui->formatStack->setCurrentIndex(0);
    }
}

void EncryptWidget::openImage(QString image)
{
    if (image == "CoverBrowseButton")
    {
        ui->coverPreviewButton->setEnabled(false);
        ui->coverPathEdit->setText("");
        hasCoverImage = false;
        //enableEncryptButton();

        QString fileName =
                QFileDialog::getOpenFileName(this,
                                             tr("Open Cover Image"),
                                             QDir::currentPath(),
                                             tr("Image Files (*.png *.jpg *.bmp);;All Files (*.*)"));

        if (fileName.isNull())
        {
            //QMessageBox::warning(this, tr("Invalid file"), tr("Invalid file."));
            return;
        }

        if (!m_coverImage.load(fileName))
        {
            QMessageBox::warning(this,
                                 tr("Invalid image"),
                                 tr("Invalid image file or format not supported."));
            return;
        }

        if (m_coverImage.format() != QImage::Format_ARGB32 &&
                m_coverImage.format() != QImage::Format_RGB32 &&
                m_coverImage.format() != QImage::Format_ARGB32_Premultiplied)
        {
            QMessageBox::StandardButton btn =
                    QMessageBox::question(this,
                                          tr("Unsupported image format"),
                                          tr("Format of the image is not supported.\nConvert to ARGB32?"));

            if (btn == QMessageBox::No)
            {
                return;
            }
            m_coverImage = m_coverImage.convertToFormat(QImage::Format_ARGB32);

            if (m_coverImage.isNull())
            {
                QMessageBox::warning(this,
                                     tr("Invalid image"),
                                     tr("Invalid image file or format not supported."));
                return;
            }
        }

        ui->coverPreviewButton->setEnabled(true);
        ui->coverPathEdit->setText(fileName);
        hasCoverImage = true;
        //enableEncryptButton();

        emit statusMessage(tr("Cover image loaded."));
    }
    else if (image == "SecretBrowseButton")
    {
        ui->secretPreviewButton->setEnabled(false);
        ui->secretPathEdit->setText("");
        hasSecretImage = false;
        //enableEncryptButton();

        QString fileName =
                QFileDialog::getOpenFileName(this,
                                             tr("Open Secret Image"),
                                             QDir::currentPath(),
                                             tr("Image Files (*.png *.jpg *.bmp);;All Files (*.*)"));

        if (fileName.isNull())
        {
            //QMessageBox::warning(this, tr("Invalid file"), tr("Invalid file."));
            return;
        }

        if (!m_secretImage.load(fileName))
        {
            QMessageBox::warning(this,
                                 tr("Invalid image"),
                                 tr("Invalid image file or format not supported."));
            return;
        }

        ui->secretPreviewButton->setEnabled(true);
        ui->secretPathEdit->setText(fileName);
        hasSecretImage = true;
        //enableEncryptButton();

        emit statusMessage(tr("Secret image loaded."));
    }
}

void EncryptWidget::enableEncryptButton()
{
    if (hasCoverImage && (hasSecretImage || !ui->secretTextEdit->toPlainText().isEmpty()))
        ui->encryptButton->setEnabled(true);
    else
        ui->encryptButton->setEnabled(false);

    ui->saveButton->setEnabled(false);
}

void EncryptWidget::encryptImage()
{
    using namespace lsb;

    formatChanged();

    LsbCrypt crypt(ui->passphraseEdit->text());
    crypt.setBitsPerByte(getBitsPerByte());

    if (m_format == LsbCrypt::Format_Text)
    {
        if (crypt.availableSize(m_coverImage) < crypt.encryptedObjectSize(ui->passphraseEdit->text()))
        {
            QMessageBox::warning(this, tr("Too Large"), tr("Secret text is too large to hide inside cover image."));
            return;
        }

        m_stegoImage = crypt.hideString(m_coverImage, ui->secretTextEdit->toPlainText());

        if (crypt.lastError() != LsbCrypt::Error_NoError)
            lsbCryptErrorMessages(crypt.lastError(), this);

        ui->saveButton->setEnabled(true);
        showPreview("EncryptButton");

        emit statusMessage(tr("Encrypted."));
    }
    else if (m_format == LsbCrypt::Format_Image)
    {
        if (crypt.availableSize(m_coverImage) < crypt.encryptedObjectSize(m_secretImage))
        {
            QMessageBox::warning(this, tr("Too Large"), tr("Secret image is too large to hide inside cover image."));
            return;
        }

        m_stegoImage = crypt.hideImage(m_coverImage, m_secretImage);

        if (crypt.lastError() != LsbCrypt::Error_NoError)
            lsbCryptErrorMessages(crypt.lastError(), this);

        ui->saveButton->setEnabled(true);
        showPreview("EncryptButton");

        emit statusMessage(tr("Encrypted."));
    }
}

void EncryptWidget::saveImage()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Image"),
                                                    QDir::currentPath(),
                                                    tr("Portable Network Graphics (*.png);;All Files (*.*)"));
    QRegExp rx("(.jpg|.jpeg|.png|.bmp)$");
    if (!fileName.contains(rx))
    {
        fileName.append(".png");
    }


    lsb::LsbCrypt::saveImage(fileName.toLatin1(), m_stegoImage);

    emit statusMessage(tr("Stego image saved."));
}

void EncryptWidget::disableButtons()
{
    ui->coverPreviewButton->setEnabled(false);
    ui->secretPreviewButton->setEnabled(false);
    ui->encryptButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
}

int EncryptWidget::getBitsPerByte()
{
    /*
    // hard coding is used since there is a problem with the lsbcrypt algorithm
    // currently 1, 2, 4 & 8 bits per byte are supported.

    switch (ui->qualitySlider->value())
    {
    case 3:
        return 1;
    case 2:
        return 2;
    case 1:
        return 4;
    case 0:
        return 8;
    default:
        return 1;
    }*/

    return 9 - ui->qualitySlider->value();
}

void EncryptWidget::setupConnections()
{
    signalMapper = new QSignalMapper(this);


    connect(ui->hidePreviewButton, &QPushButton::clicked, this, &EncryptWidget::hidePreview);

    connect(ui->imageRadio, &QRadioButton::toggled, this, &EncryptWidget::formatChanged);
    connect(ui->textRadio, &QRadioButton::toggled, this, &EncryptWidget::formatChanged);



    signalMapper->setMapping(ui->coverBrowseButton, QString("CoverBrowseButton"));
    signalMapper->setMapping(ui->secretBrowseButton, QString("SecretBrowseButton"));
    signalMapper->setMapping(ui->coverPreviewButton, QString("CoverPreviewButton"));
    signalMapper->setMapping(ui->secretPreviewButton, QString("SecretPreviewButton"));
    signalMapper->setMapping(ui->encryptButton, QString("EncryptButton"));

    connect(ui->coverBrowseButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->secretBrowseButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->coverPreviewButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->secretPreviewButton, SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui->encryptButton, SIGNAL(clicked()), signalMapper, SLOT(map()));

    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(openImage(QString)));
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(showPreview(QString)));


    connect(ui->coverBrowseButton, &QPushButton::clicked, this, &EncryptWidget::enableEncryptButton);
    connect(ui->secretBrowseButton, &QPushButton::clicked, this, &EncryptWidget::enableEncryptButton);
    connect(ui->secretTextEdit, &QPlainTextEdit::modificationChanged, this, &EncryptWidget::enableEncryptButton);

    connect(ui->encryptButton, &QPushButton::clicked, this, &EncryptWidget::encryptImage);

    connect(ui->saveButton, &QPushButton::clicked, this, &EncryptWidget::saveImage);
}

#ifndef ENCRYPTWIDGET_H
#define ENCRYPTWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include "lsbcrypt.h"

namespace Ui {
class EncryptWidget;
}

class EncryptWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EncryptWidget(QWidget *parent = 0);
    ~EncryptWidget();

private slots:
    void showPreview(QString image);
    void hidePreview();
    void formatChanged();
    void openImage(QString image);
    void enableEncryptButton();
    void encryptImage();
    void saveImage();
    //void setButtonEnabled(QString button);
    
signals:
    void statusMessage(QString);
    void enableButton(QString button, bool enable);

private:
    void disableButtons();
    void setupConnections();

    int getBitsPerByte();

    lsb::LsbCrypt::Format m_format;

    QImage m_coverImage;
    QImage m_secretImage;
    QImage m_stegoImage;

    bool hasCoverImage;
    bool hasSecretImage;
    bool hasSecretText;

    Ui::EncryptWidget *ui;
    QSignalMapper *signalMapper;
};

#endif // ENCRYPTWIDGET_H

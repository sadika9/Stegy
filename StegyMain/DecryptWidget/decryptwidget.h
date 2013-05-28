#ifndef DECRYPTWIDGET_H
#define DECRYPTWIDGET_H

#include <QWidget>
#include "lsbcrypt.h"

namespace Ui {
class DecryptWidget;
}

class DecryptWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit DecryptWidget(QWidget *parent = 0);
    ~DecryptWidget();

private slots:
    void openImage();
    void saveSecretData();
    void previewStegoImage();
    void decryptImage();

signals:
    void statusMessage(QString);
    
private:
    Ui::DecryptWidget *ui;

    QImage m_stegoImage;
    QImage m_secretImage;

    QString m_secretText;

    lsb::LsbCrypt::Format m_format;
};

#endif // DECRYPTWIDGET_H

#ifndef ENCODEWIDGET_H
#define ENCODEWIDGET_H

#include <QWidget>
#include <QSignalMapper>
#include "lsbsteg.h"

namespace Ui {
class EncodeWidget;
}

class EncodeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit EncodeWidget(QWidget *parent = 0);
    ~EncodeWidget();

private slots:
    void showPreview(QString image);
    void hidePreview();
    void formatChanged();
    void openImage(QString image);
    void enableEncodeButton();
    void encodeImage();
    void saveImage();
    //void setButtonEnabled(QString button);
    
signals:
    void statusMessage(QString);
    void enableButton(QString button, bool enable);

private:
    void disableButtons();
    void setupConnections();

    int getBitsPerByte();

    lsb::LsbSteg::Format m_format;

    QImage m_coverImage;
    QImage m_secretImage;
    QImage m_stegoImage;

    bool hasCoverImage;
    bool hasSecretImage;
    bool hasSecretText;

    Ui::EncodeWidget *ui;
    QSignalMapper *signalMapper;
};

#endif // ENCODEWIDGET_H

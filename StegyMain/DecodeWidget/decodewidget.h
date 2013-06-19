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

    lsb::LsbSteg::Format m_format;
};

#endif // DECRYPTWIDGET_H

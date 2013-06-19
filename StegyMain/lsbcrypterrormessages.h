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

#ifndef LSBCRYPTERRORMESSAGES_H
#define LSBCRYPTERRORMESSAGES_H

#include "lsbcrypt.h"
#include <QMessageBox>
#include <QApplication>

static void lsbCryptErrorMessages(lsb::LsbCrypt::Error error, QWidget *parent = 0)
{
    using namespace lsb;

    QString msg = "";

    switch (error)
    {
    case LsbCrypt::Error_NoError:
        return;
        break;
    case LsbCrypt::Error_InvalidBitsPerByte:
        msg = qApp->tr("Invalid Bits Per Byte");
        break;
    case LsbCrypt::Error_InvalidByteArraySize:
        msg = qApp->tr("Invalid Byte Array Size:");
    case LsbCrypt::Error_InvalidCoverImage:
        msg = qApp->tr("Invalid Cover Image");
        break;
    case LsbCrypt::Error_InvalidSecretImage:
        msg = qApp->tr("Invalid Secret Image");
    case LsbCrypt::Error_InvalidStegoImage:
        msg = qApp->tr("Invalid Stego Image");
        break;
    case LsbCrypt::Error_InvalidText:
        msg = qApp->tr("Invalid Text");
        break;
    case LsbCrypt::Error_SecretTextTooLarge:
        msg = qApp->tr("Secret Text Too Large");
        break;
    case LsbCrypt::Error_EncryptionError:
        msg = qApp->tr("LsbCrypt Error: Encryption Error");
        break;
    case LsbCrypt::Error_DecryptionError:
        msg = qApp->tr("Decryption Error");
        break;
    case LsbCrypt::Error_SecretImageTooLarge:
        msg = qApp->tr("Secret Image Too Large");
        break;
    case LsbCrypt::Error_WrongFormat:
        msg = qApp->tr("Wrong Format");
        break;
    case LsbCrypt::Error_InvalidFactor:
        msg = qApp->tr("InvalidFactor");
        break;
    default:
        msg = qApp->tr("Unknown Error");
        break;
    }

    QMessageBox::warning(parent, qApp->tr("Lsb Error"), msg);
}

#endif // LSBCRYPTERRORMESSAGES_H

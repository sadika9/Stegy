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
        msg = qApp->tr("LsbCrypt Error: Invalid Bits Per Byte");
        break;
    case LsbCrypt::Error_InvalidByteArraySize:
        msg = qApp->tr("LsbCrypt Error: Invalid Byte Array Size:");
    case LsbCrypt::Error_InvalidCoverImage:
        msg = qApp->tr("LsbCrypt Error: Invalid Cover Image");
        break;
    case LsbCrypt::Error_InvalidSecretImage:
        msg = qApp->tr("LsbCrypt Error: Invalid Secret Image");
    case LsbCrypt::Error_InvalidStegoImage:
        msg = qApp->tr("LsbCrypt Error: Invalid Stego Image");
        break;
    case LsbCrypt::Error_InvalidText:
        msg = qApp->tr("LsbCrypt Error: Invalid Text");
        break;
    case LsbCrypt::Error_SecretTextTooLarge:
        msg = qApp->tr("LsbCrypt Error: Secret Text Too Large");
        break;
    case LsbCrypt::Error_EncryptionError:
        msg = qApp->tr("LsbCrypt Error: Encryption Error");
        break;
    case LsbCrypt::Error_DecryptionError:
        msg = qApp->tr("LsbCrypt Error: Decryption Error");
        break;
    case LsbCrypt::Error_SecretImageTooLarge:
        msg = qApp->tr("LsbCrypt Error: Secret Image Too Large");
        break;
    case LsbCrypt::Error_WrongFormat:
        msg = qApp->tr("LsbCrypt Error: Wrong Format");
        break;
    case LsbCrypt::Error_InvalidFactor:
        msg = qApp->tr("LsbCrypt Error: InvalidFactor");
        break;
    default:
        msg = qApp->tr("LsbCrypt Error: Unknown Error");
        break;
    }

    QMessageBox::warning(parent, qApp->tr("LsbCrypt Error"), msg);
}

#endif // LSBCRYPTERRORMESSAGES_H

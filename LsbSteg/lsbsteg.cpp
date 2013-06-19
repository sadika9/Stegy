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

#include "lsbsteg.h"
#include "passtokey.h"
#include <cmath>

#include <QDebug>

namespace lsb {
LsbSteg::LsbSteg(QString passowrd):
    m_dataPos(0),
    m_bitsPerByte(1),
    m_password(passowrd),
    m_lastError(Error_NoError)
{
}

QImage LsbSteg::hideImage(QImage &coverImage, QImage &secretImage)
{
    // reset data position since we are about to start encryption.
    m_dataPos = 0;

    // check whether input images are valid.
    if (secretImage.isNull())
    {
        m_lastError = Error_InvalidSecretImage;
        qWarning() << Q_FUNC_INFO << "InvalidSecretImage";
        return QImage();
    }

    if (coverImage.isNull())
    {
        m_lastError = Error_InvalidCoverImage;
        qWarning() << Q_FUNC_INFO << "InvalidCoverImage";
        return QImage();
    }

    // Copy parameter images. We need this step because QImage uses implicit sharing.
    m_stegoImage = coverImage.copy();

    // write format i.e this stego image contains an image.
    writeFormat(Format_Image);


    // store how many bits are used to store image data per byte.
    writeBitsPerByte();
    if (m_lastError == Error_InvalidBitsPerByte)
    {
        qWarning() << Q_FUNC_INFO << "InvalidBitsPerByte";
        return QImage();
    }

    // begining encryption process
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    QDataStream stream(&buffer);
    stream.setVersion(QDataStream::Qt_5_0);
    stream << secretImage;


    // generating 64 bit key from the password and use that key in SimpleCrypt
    quint64 key = passToKey(m_password);
    SimpleCrypt crypt(key);
    QByteArray cypherArray =crypt.encryptToByteArray(buffer.data());
    if (crypt.lastError() != crypt.ErrorNoError)
    {
        m_lastError = Error_EncryptionError;
        qWarning() << Q_FUNC_INFO << "EncryptionError";
        return QImage();
    }
    buffer.close();


    int cypherArraySize = cypherArray.size();

    if (!canHideImage(cypherArraySize))
    {
        m_lastError = Error_SecretImageTooLarge;
        qWarning() << Q_FUNC_INFO << "SecretImageTooLarge";
        return QImage();
    }

    writeByteArraySize(cypherArraySize);
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << Q_FUNC_INFO << "InvalidByteArraySize";
        return QImage();
    }

    /*for (int i = 0; i < cypherArraySize; ++i){
      //  qDebug() << i;
        LsbCryptWrite::writeData(cypherArray.data()[i], m_stegoImage.bits(), sizeof (char) * 8, m_bitsPerByte, &m_dataPos);

    }*/

    writeStegoData(cypherArray);
    if (m_lastError == Error_InvalidFactor)
    {
        qWarning() << Q_FUNC_INFO << "InvalidFactor";
        return QImage();
    }



    m_lastError = Error_NoError;
    return m_stegoImage;
}

QImage LsbSteg::unhideImage(QImage &stegoImage)
{
    // reset data position; going to start decryption
    m_dataPos = 0;

    // check whether input image is valid.
    if (stegoImage.isNull())
    {
        m_lastError = Error_InvalidStegoImage;
        qWarning() << Q_FUNC_INFO << "InvalidStegoImage";
        return QImage();
    }
    m_stegoImage = stegoImage.copy();


    // read the format of the content which stego image has.
    readFormat();
    if (m_format != Format_Image)
    {
        m_lastError = Error_WrongFormat;
        qWarning() << Q_FUNC_INFO << "WrongFormat";
        return QImage();
    }

    readBitsPerByte();
    if (m_lastError == Error_InvalidBitsPerByte)
    {
        qWarning() << Q_FUNC_INFO << "InvalidBitsPerByte";
        return QImage();
    }


    int cypherByteArraySize = readByteArraySize();
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << Q_FUNC_INFO << "InvalidByteArraySize";
        return QImage();
    }

    // reading byteArray

    //LsbCryptRead::readData(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);
    //LsbCryptRead::readData2(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);

    char data[cypherByteArraySize];
    readStegoData(data, cypherByteArraySize);
    if (m_lastError == Error_InvalidFactor)
    {
        qWarning() << Q_FUNC_INFO << "InvalidFactor";
        return QImage();
    }


    QByteArray cypherByteArray = QByteArray::fromRawData(reinterpret_cast<char*>(data), sizeof data);

    // decrypting
    quint64 key = passToKey(m_password);
    SimpleCrypt crypt(key);

    QByteArray plainArray = crypt.decryptToByteArray(cypherByteArray);

    if (crypt.lastError() != SimpleCrypt::ErrorNoError)
    {
        m_lastError = Error_DecryptionError;
        qWarning() << Q_FUNC_INFO << "DecryptionError" << "crypt.lastError()" << crypt.lastError();
        return QImage();
    }

    QBuffer buffer(&plainArray);
    buffer.open(QIODevice::ReadOnly);
    QDataStream stream(&buffer);
    stream.setVersion(QDataStream::Qt_5_0);

    stream >> m_secretImage;
    if (m_secretImage.isNull())
    {
        m_lastError = Error_InvalidSecretImage;
    }

    m_lastError = Error_NoError;
    return m_secretImage;
}

QImage LsbSteg::hideString(QImage &coverImage, QString text)
{
    // reset data position since we are about to start encryption.
    m_dataPos = 0;

    // check whether input images are valid.
    if (text.isEmpty())
    {
        m_lastError = Error_InvalidText;
        qWarning() << Q_FUNC_INFO << "InvalidText";
        return QImage();
    }

    if (coverImage.isNull())
    {
        m_lastError = Error_InvalidCoverImage;
        qWarning() << Q_FUNC_INFO << "InvalidCoverImage";
        return QImage();
    }

    // Copy parameter images. We need this step because QImage uses implicit sharing.
    m_stegoImage = coverImage.copy();


    // write format i.e this stego image contains an image.
    writeFormat(Format_Text);

    // store how many bits are used to store image data per byte.
    writeBitsPerByte();
    if (m_lastError == Error_InvalidBitsPerByte)
    {
        qWarning() << Q_FUNC_INFO << "InvalidBitsPerByte";
        return QImage();
    }


    // begining encryption process
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    QDataStream stream(&buffer);
    stream.setVersion(QDataStream::Qt_5_0);
    stream << text;


    // generating 64 bit key from the password and use that key in SimpleCrypt
    quint64 key = passToKey(m_password);
    SimpleCrypt crypt(key);
    QByteArray cypherArray =crypt.encryptToByteArray(buffer.data());
    if (crypt.lastError() != crypt.ErrorNoError)
    {
        m_lastError = Error_EncryptionError;
        qWarning() << Q_FUNC_INFO << "EncryptionError";
        return QImage();
    }
    buffer.close();
    int cypherArraySize = cypherArray.size();
    if (!canHideImage(cypherArraySize))
    {
        m_lastError = Error_SecretTextTooLarge;
        qWarning() << Q_FUNC_INFO << "SecretTextTooLarge";
        return QImage();
    }

    writeByteArraySize(cypherArraySize);
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << Q_FUNC_INFO << "InvalidByteArraySize";
        return QImage();
    }

    /*for (int i = 0; i < cypherArraySize; ++i)
        LsbCryptWrite::writeData(cypherArray.data()[i], m_stegoImage.bits(), sizeof (char) * 8, m_bitsPerByte, &m_dataPos);*/

    writeStegoData(cypherArray);
    if (m_lastError == Error_InvalidFactor)
    {
        qWarning() << Q_FUNC_INFO << "InvalidFactor";
        return QImage();
    }


    m_lastError = Error_NoError;
    return m_stegoImage;
}

QString LsbSteg::unhideString(QImage &stegoImage)
{
    // reset data position; going to start decryption
    m_dataPos = 0;

    // check whether input image is valid.
    if (stegoImage.isNull())
    {
        m_lastError = Error_InvalidStegoImage;
        qWarning() << Q_FUNC_INFO << "InvalidStegoImage";
        return QString();
    }
    m_stegoImage = stegoImage.copy();

    // read the format of the content which stego image has.
    readFormat();
    if (m_format != Format_Text)
    {
        m_lastError = Error_WrongFormat;
        qWarning() << Q_FUNC_INFO << "WrongFormat";
        return QString();
    }


    readBitsPerByte();
    if (m_lastError == Error_InvalidBitsPerByte)
    {
        qWarning() << Q_FUNC_INFO << "InvalidBitsPerByte";
        return QString();
    }

    int cypherByteArraySize = readByteArraySize();
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << Q_FUNC_INFO << "InvalidByteArraySize";
        return QString();
    }

    qDebug();// don't remove this. there is a problem, this statement makes this decryption work

    // reading byteArray
    //LsbCryptRead::readData(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);
    //LsbCryptRead::readData2(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);


    char data[cypherByteArraySize];
    readStegoData(data, cypherByteArraySize);
    if (m_lastError == Error_InvalidFactor)
    {
        qWarning() << Q_FUNC_INFO << "InvalidFactor";
        return QString();
    }


    QByteArray cypherByteArray = QByteArray::fromRawData(reinterpret_cast<char*>(data), sizeof data);

    // decrypting
    quint64 key = passToKey(m_password);
    SimpleCrypt crypt(key);
    QByteArray plainArray = crypt.decryptToByteArray(cypherByteArray);
    if (crypt.lastError() != SimpleCrypt::ErrorNoError)
    {
        m_lastError = Error_DecryptionError;
        qWarning() << Q_FUNC_INFO << "DecryptionError" << "crypt.lastError()" << crypt.lastError();
        return QString();
    }

    QBuffer buffer(&plainArray);
    buffer.open(QIODevice::ReadOnly);
    QDataStream stream(&buffer);
    stream.setVersion(QDataStream::Qt_5_0);

    QString text;
    stream >> text;
    if (text.isEmpty())
    {
        m_lastError = Error_InvalidText;
    }

    m_lastError = Error_NoError;
    return text;
}

void LsbSteg::writeBitsPerByte()
{
    if (m_bitsPerByte < 1 || m_bitsPerByte > bitsPerByteMaxValue)
    {
        m_lastError = Error_InvalidBitsPerByte;
        return;
    }

    int size = ceil((double) nBitsPerByte_Bits / nBitsPerByte_BitsPerByte);

    for (int i = 0; i < size; ++i)
    {
        bool bit = LsbStegRead::getBit((m_bitsPerByte - 1), i);

        LsbStegWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos++, 0);
    }

    /*LsbCryptWrite::writeData(
                (m_bitsPerByte - 1), m_stegoImage.bits(),
                nMagicBits, nMagicBitsPerByte, &m_dataPos);*/
}

void LsbSteg::readBitsPerByte()
{
    m_bitsPerByte = LsbStegRead::parseInt(
                m_stegoImage.bits(), &m_dataPos, nBitsPerByte_Bits, nBitsPerByte_BitsPerByte);

    m_bitsPerByte += 1;

    if (m_bitsPerByte < 1 || m_bitsPerByte > bitsPerByteMaxValue)
    {
        m_lastError = Error_InvalidBitsPerByte;
        return;
    }
}

void LsbSteg::writeFormat(Format format)
{
    m_format = format;

    bool bit = (m_format == Format_Image) ? true : false;

    LsbStegWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos, 0);
    ++m_dataPos;

    /*if (m_format == Format_Text)    // text -> 0
    {
        LsbCryptWrite::writeData(
                    0, m_stegoImage.bits(),
                    nFormatBits, nFormatBitsPerByte, &m_dataPos);

    }
    else    // image -> 1
    {
        LsbCryptWrite::writeData(
                    1, m_stegoImage.bits(),
                    nFormatBits, nFormatBitsPerByte, &m_dataPos);
    }*/
}

void LsbSteg::readFormat()
{
    int fmt = LsbStegRead::parseInt(
                m_stegoImage.bits(), &m_dataPos, nFormatBits, nFormatBitsPerByte);

    if (fmt == 0)
        m_format = Format_Text;
    else
        m_format = Format_Image;
}

void LsbSteg::writeByteArraySize(int size)
{
    if (size < 0)
    {
        m_lastError = Error_InvalidByteArraySize;
        return;
    }

    for (int i = 0, bitPos = 0; i < sizeOfInt; ++i)
    {
        bool bit = LsbStegRead::getBit(size, i);

        LsbStegWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos, bitPos++);

        if (bitPos == m_bitsPerByte)
        {
            bitPos = 0;
            ++m_dataPos;
        }
    }
    ++m_dataPos;

    /*LsbCryptWrite::writeData(
                size, m_stegoImage.bits(), m_sizeOfInt, m_bitsPerByte, &m_dataPos);*/
}

int LsbSteg::readByteArraySize()
{
    /*int size = LsbCryptRead::parseInt(
                m_stegoImage.bits(), &m_dataPos, m_sizeOfInt,  m_bitsPerByte);*/

    int size = 0;

    for (int i = 0, bitPos = 0; i < sizeOfInt; ++i)
    {
        bool bit = LsbStegRead::getBit(m_stegoImage.bits()[m_dataPos], bitPos++);

        if (bit)
            size |= (1 << i);
        else
            size |= (0 << i);

        if (bitPos == m_bitsPerByte)
        {
            bitPos = 0;
            ++m_dataPos;
        }
    }
    ++m_dataPos;

    if (size < 0)
    {
        m_lastError = Error_InvalidByteArraySize;
        return 0;
    }

    return size;
}

void LsbSteg::writeStegoData(QByteArray cypherArray)
{
    // factor is used to stretch data array all over the image.
    int factor = floor((double) availableSize(m_stegoImage) / (cypherArray.size() * 8));
    if (factor < 0)
    {
        m_lastError = Error_InvalidFactor;
        qWarning() << Q_FUNC_INFO << "Error_InvalidFactor";
        return;
    }

    for (int i = 0, bitPos = 0; i < cypherArray.size(); ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            bool bit = LsbStegRead::getBit(cypherArray.data()[i], j);

            LsbStegWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos, bitPos++);

            if (bitPos == m_bitsPerByte)
            {
                bitPos = 0;
                //++m_dataPos;
                m_dataPos += factor;
            }
        }
    }
}

void LsbSteg::readStegoData(char toArray[], int cypherByteArraySize)
{
    // factor is used to stretch data array all over the image.
    int factor = floor((double) availableSize(m_stegoImage) / (cypherByteArraySize * 8));
    if (factor < 0)
    {
        m_lastError = Error_InvalidFactor;
        qWarning() << Q_FUNC_INFO << "Error_InvalidFactor";
        return;
    }


    for (int i = 0; i < cypherByteArraySize; ++i)
    {
        toArray[i] = 0;
    }

    for (int i = 0, bitPos = 0; i < cypherByteArraySize; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            bool bit = LsbStegRead::getBit(m_stegoImage.bits()[m_dataPos], bitPos++);

            if (bit)
                toArray[i] |= (1 << j);
            else
                toArray[i] |= (0 << j);

            if (bitPos == m_bitsPerByte)
            {
                bitPos = 0;
                //++m_dataPos;
                m_dataPos += factor;
            }
        }
    }
    ++m_dataPos;
}

int LsbSteg::bitsPerByte() const
{
    return m_bitsPerByte;
}

void LsbSteg::setBitsPerByte(int bitsPerByte)
{
    m_bitsPerByte = bitsPerByte;
}

bool LsbSteg::saveImage(const char *filename, QImage image, int cvType)
{

    cv::Mat mat(image.height(), image.width(), cvType, image.bits(), image.bytesPerLine());

    cv::vector<int> compressionParams;
    compressionParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compressionParams.push_back(9);

    return cv::imwrite(filename, mat, compressionParams);
}

QImage LsbSteg::errorImage()
{
    return QImage();
}

QString LsbSteg::password() const
{
    return m_password;
}

void LsbSteg::setPassword(const QString &password)
{
    m_password = password;
}

LsbSteg::Error LsbSteg::lastError() const
{
    return m_lastError;
}

LsbSteg::Format LsbSteg::getFormat(QImage &image)
{
    int dataPos = 0;

    int fmt = LsbStegRead::parseInt(
                image.bits(), &dataPos, nFormatBits, nFormatBitsPerByte);

    if (fmt == 0)
        return Format_Text;
    else
        return Format_Image;
}

int LsbSteg::encryptedObjectSize(QImage &secret)
{
    int usedSize = ceil((double) nBitsPerByte_Bits / nBitsPerByte_BitsPerByte);
    usedSize += ceil((double) nFormatBits / nFormatBitsPerByte);
    usedSize += sizeOfInt;

    // calculating byte array size
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    QDataStream stream(&buffer);
    stream.setVersion(QDataStream::Qt_5_0);
    stream << secret;

    SimpleCrypt crypt(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QByteArray cypherArray =crypt.encryptToByteArray(buffer.data());
    if (crypt.lastError() != crypt.ErrorNoError)
    {
        m_lastError = Error_EncryptionError;
        qWarning() << Q_FUNC_INFO << "EncryptionError";
    }
    buffer.close();

    usedSize += cypherArray.size();

    return usedSize;
}

int LsbSteg::encryptedObjectSize(QString text)
{
    int usedSize = ceil((double) nBitsPerByte_Bits / nBitsPerByte_BitsPerByte);
    usedSize += ceil((double) nFormatBits / nFormatBitsPerByte);
    usedSize += sizeOfInt;

    // calculating byte array size
    QBuffer buffer;
    buffer.open(QIODevice::WriteOnly);

    QDataStream stream(&buffer);
    stream.setVersion(QDataStream::Qt_5_0);
    stream << text;

    SimpleCrypt crypt(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QByteArray cypherArray =crypt.encryptToByteArray(buffer.data());
    if (crypt.lastError() != crypt.ErrorNoError)
    {
        m_lastError = Error_EncryptionError;
        qWarning() << Q_FUNC_INFO << "EncryptionError";
    }
    buffer.close();

    usedSize += cypherArray.size();

    return usedSize;
}

int LsbSteg::availableSize(QImage &image)
{
    /*int availableSize; =
            (cover.byteCount() * m_bitsPerByte) -
            (ceil((double) nMagicBits / nMagicBitsPerByte) +
             ceil((double) nFormatBits / nFormatBitsPerByte) + m_sizeOfInt);*/

    int headerSize = 0;

    // size taken for store format
    headerSize += ceil((double) nFormatBits / nFormatBitsPerByte);  // bytes
    // size to store bits per byte value
    headerSize += ceil((double)nBitsPerByte_Bits / nBitsPerByte_BitsPerByte);  // bytes
    // size taken to store the size of byte array
    headerSize += ceil((double) sizeOfInt / m_bitsPerByte);  // bytes

    // total available bytes to hide data is given by..
    int availableSize = (image.byteCount() - headerSize) * m_bitsPerByte;

    return availableSize;
}


bool LsbSteg::canHideImage(int byteArraySize)
{
    if (availableSize(m_stegoImage) <= byteArraySize * 8)
        return false;

    return true;
}

}   // namespace lsb

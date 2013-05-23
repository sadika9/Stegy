#include "lsbcrypt.h"
#include "passtokey.h"
#include <cmath>

#include <QDebug>

namespace lsb {
LsbCrypt::LsbCrypt(QString passowrd):
    m_dataPos(0),
    m_bitsPerByte(1),
    m_password(passowrd),
    m_lastError(Error_NoError)
{
}

QImage LsbCrypt::encryptImage(QImage &coverImage, QImage &secretImage)
{
    // reset data position since we are about to start encryption.
    m_dataPos = 0;

    // check whether input images are valid.
    if (secretImage.isNull())
    {
        m_lastError = Error_InvalidSecretImage;
        qWarning() << "encryptImage()" << "InvalidSecretImage";
        return QImage();
    }

    if (coverImage.isNull())
    {
        m_lastError = Error_InvalidCoverImage;
        qWarning() << "encryptImage()" << "InvalidCoverImage";
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
        qWarning() << "encryptImage()" << "InvalidBitsPerByte";
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
        qWarning() << "encryptImage()" << "EncryptionError";
        return QImage();
    }
    buffer.close();


    int cypherArraySize = cypherArray.size();

    if (!canHideImage(cypherArraySize))
    {
        m_lastError = Error_SecretImageTooLarge;
        qWarning() << "encryptImage()" << "SecretImageTooLarge";
        return QImage();
    }

    writeByteArraySize(cypherArraySize);
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << "encryptImage()" << "InvalidByteArraySize";
        return QImage();
    }

    /*for (int i = 0; i < cypherArraySize; ++i){
      //  qDebug() << i;
        LsbCryptWrite::writeData(cypherArray.data()[i], m_stegoImage.bits(), sizeof (char) * 8, m_bitsPerByte, &m_dataPos);

    }*/

    writeStegoData(cypherArray);



    m_lastError = Error_NoError;
    return m_stegoImage;
}

QImage LsbCrypt::decryptImage(QImage &stegoImage)
{
    // reset data position; going to start decryption
    m_dataPos = 0;

    // check whether input image is valid.
    if (stegoImage.isNull())
    {
        m_lastError = Error_InvalidStegoImage;
        qWarning() << "decryptImage()" << "InvalidStegoImage";
        return QImage();
    }
    m_stegoImage = stegoImage.copy();


    // read the format of the content which stego image has.
    readFormat();
    if (m_format != Format_Image)
    {
        m_lastError = Error_WrongFormat;
        qWarning() << "decryptImage()" << "WrongFormat";
        return QImage();
    }

    readBitsPerByte();
    if (m_lastError == Error_InvalidBitsPerByte)
    {
        qWarning() << "decryptImage()" << "InvalidBitsPerByte";
        return QImage();
    }


    int cypherByteArraySize = readByteArraySize();
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << "decryptImage()" << "InvalidByteArraySize";
        return QImage();
    }

    // reading byteArray

    //LsbCryptRead::readData(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);
    //LsbCryptRead::readData2(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);

    char data[cypherByteArraySize];
    readStegoData(data, cypherByteArraySize);


    QByteArray cypherByteArray = QByteArray::fromRawData(reinterpret_cast<char*>(data), sizeof data);

    // decrypting
    quint64 key = passToKey(m_password);
    SimpleCrypt crypt(key);

    QByteArray plainArray = crypt.decryptToByteArray(cypherByteArray);

    if (crypt.lastError() != SimpleCrypt::ErrorNoError)
    {
        m_lastError = Error_DecryptionError;
        qWarning() << "decryptImage()" << "DecryptionError" << "crypt.lastError()" << crypt.lastError();
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

QImage LsbCrypt::encryptString(QImage &coverImage, QString text)
{
    // reset data position since we are about to start encryption.
    m_dataPos = 0;

    // check whether input images are valid.
    if (text.isEmpty())
    {
        m_lastError = Error_InvalidText;
        qWarning() << "encryptString()" << "InvalidText";
        return QImage();
    }

    if (coverImage.isNull())
    {
        m_lastError = Error_InvalidCoverImage;
        qWarning() << "encryptString()" << "InvalidCoverImage";
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
        qWarning() << "encryptString()" << "InvalidBitsPerByte";
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
        qWarning() << "encryptString()" << "EncryptionError";
        return QImage();
    }
    buffer.close();
    int cypherArraySize = cypherArray.size();
    if (!canHideImage(cypherArraySize))
    {
        m_lastError = Error_SecretTextTooLarge;
        qWarning() << "encryptString()" << "SecretTextTooLarge";
        return QImage();
    }

    writeByteArraySize(cypherArraySize);
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << "encryptString()" << "InvalidByteArraySize";
        return QImage();
    }

    /*for (int i = 0; i < cypherArraySize; ++i)
        LsbCryptWrite::writeData(cypherArray.data()[i], m_stegoImage.bits(), sizeof (char) * 8, m_bitsPerByte, &m_dataPos);*/

    writeStegoData(cypherArray);

    m_lastError = Error_NoError;
    return m_stegoImage;
}

QString LsbCrypt::decryptString(QImage &stegoImage)
{
    // reset data position; going to start decryption
    m_dataPos = 0;

    // check whether input image is valid.
    if (stegoImage.isNull())
    {
        m_lastError = Error_InvalidStegoImage;
        qWarning() << "decryptString()" << "InvalidStegoImage";
        return QString();
    }
    m_stegoImage = stegoImage.copy();

    // read the format of the content which stego image has.
    readFormat();
    if (m_format != Format_Text)
    {
        m_lastError = Error_WrongFormat;
        qWarning() << "decryptString()" << "WrongFormat";
        return QString();
    }


    readBitsPerByte();
    if (m_lastError == Error_InvalidBitsPerByte)
    {
        qWarning() << "decryptString()" << "InvalidBitsPerByte";
        return QString();
    }

    int cypherByteArraySize = readByteArraySize();
    if (m_lastError == Error_InvalidByteArraySize)
    {
        qWarning() << "decryptString()" << "InvalidByteArraySize";
        return QString();
    }

    qDebug();// don't remove this. there is a problem, this statement makes this decryption work

    // reading byteArray
    //LsbCryptRead::readData(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);
    //LsbCryptRead::readData2(m_stegoImage.bits(), data, &m_dataPos, m_bitsPerByte, sizeof data);


    char data[cypherByteArraySize];
    readStegoData(data, cypherByteArraySize);


    QByteArray cypherByteArray = QByteArray::fromRawData(reinterpret_cast<char*>(data), sizeof data);

    // decrypting
    quint64 key = passToKey(m_password);
    SimpleCrypt crypt(key);
    QByteArray plainArray = crypt.decryptToByteArray(cypherByteArray);
    if (crypt.lastError() != SimpleCrypt::ErrorNoError)
    {
        m_lastError = Error_DecryptionError;
        qWarning() << "decryptString()" << "DecryptionError" << "crypt.lastError()" << crypt.lastError();
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

void LsbCrypt::writeBitsPerByte()
{
    if (m_bitsPerByte < 1 || m_bitsPerByte > m_bitsPerByteMaxValue)
    {
        m_lastError = Error_InvalidBitsPerByte;
        return;
    }

    int size = ceil((double) nMagicBits / nMagicBitsPerByte);

    for (int i = 0; i < size; ++i)
    {
        bool bit = LsbCryptRead::getBit((m_bitsPerByte - 1), i);

        LsbCryptWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos++, 0);
    }

    /*LsbCryptWrite::writeData(
                (m_bitsPerByte - 1), m_stegoImage.bits(),
                nMagicBits, nMagicBitsPerByte, &m_dataPos);*/
}

void LsbCrypt::readBitsPerByte()
{
    m_bitsPerByte = LsbCryptRead::parseInt(
                m_stegoImage.bits(), &m_dataPos, nMagicBits, nMagicBitsPerByte);

    m_bitsPerByte += 1;

    if (m_bitsPerByte < 1 || m_bitsPerByte > m_bitsPerByteMaxValue)
    {
        m_lastError = Error_InvalidBitsPerByte;
        return;
    }
}

void LsbCrypt::writeFormat(Format format)
{
    m_format = format;

    bool bit = (m_format == Format_Image) ? true : false;

    LsbCryptWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos, 0);
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

void LsbCrypt::readFormat()
{
    int fmt = LsbCryptRead::parseInt(
                m_stegoImage.bits(), &m_dataPos, nFormatBits, nFormatBitsPerByte);

    if (fmt == 0)
        m_format = Format_Text;
    else
        m_format = Format_Image;
}

void LsbCrypt::writeByteArraySize(int size)
{
    if (size < 0)
    {
        m_lastError = Error_InvalidByteArraySize;
        return;
    }

    for (int i = 0, bitPos = 0; i < m_sizeOfInt; ++i)
    {
        bool bit = LsbCryptRead::getBit(size, i);

        LsbCryptWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos, bitPos++);

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

int LsbCrypt::readByteArraySize()
{
    /*int size = LsbCryptRead::parseInt(
                m_stegoImage.bits(), &m_dataPos, m_sizeOfInt,  m_bitsPerByte);*/

    int size = 0;

    for (int i = 0, bitPos = 0; i < m_sizeOfInt; ++i)
    {
        bool bit = LsbCryptRead::getBit(m_stegoImage.bits()[m_dataPos], bitPos++);

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

void LsbCrypt::writeStegoData(QByteArray cypherArray)
{
    // factor is used to stretch data array all over the image.
    int factor = floor((double) availableSize(m_stegoImage) / (cypherArray.size() * 8));

    for (int i = 0; i < cypherArray.size(); ++i)
    {
        for (int j = 0, bitPos = 0; j < 8; ++j)
        {
            bool bit = LsbCryptRead::getBit(cypherArray.data()[i], j);

            LsbCryptWrite::writeBit(bit, m_stegoImage.bits(), m_dataPos, bitPos++);

            if (bitPos == m_bitsPerByte)
            {
                bitPos = 0;
                //++m_dataPos;
                m_dataPos += factor;
            }
        }
    }
}

void LsbCrypt::readStegoData(char toArray[], int cypherByteArraySize)
{
    // factor is used to stretch data array all over the image.
    int factor = floor((double) availableSize(m_stegoImage) / (cypherByteArraySize * 8));

    for (int i = 0; i < cypherByteArraySize; ++i)
    {
        toArray[i] = 0;
    }

    for (int i = 0; i < cypherByteArraySize; ++i)
    {
        for (int j = 0, bitPos = 0; j < 8; ++j)
        {
            bool bit = LsbCryptRead::getBit(m_stegoImage.bits()[m_dataPos], bitPos++);

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

int LsbCrypt::bitsPerByte() const
{
    return m_bitsPerByte;
}

void LsbCrypt::setBitsPerByte(int bitsPerByte)
{
    m_bitsPerByte = bitsPerByte;
}

bool LsbCrypt::saveImage(const char *filename, QImage image, int cvType)
{

    cv::Mat mat(image.height(), image.width(), cvType, image.bits(), image.bytesPerLine());

    cv::vector<int> compressionParams;
    compressionParams.push_back(CV_IMWRITE_PNG_COMPRESSION);
    compressionParams.push_back(9);

    return cv::imwrite(filename, mat, compressionParams);
}

QImage LsbCrypt::errorImage()
{
    return QImage();
}

QString LsbCrypt::password() const
{
    return m_password;
}

void LsbCrypt::setPassword(const QString &password)
{
    m_password = password;
}

LsbCrypt::Error LsbCrypt::lastError() const
{
    return m_lastError;
}

LsbCrypt::Format LsbCrypt::getFormat(QImage &image)
{
    int dataPos = 0;

    int fmt = LsbCryptRead::parseInt(
                image.bits(), &dataPos, nFormatBits, nFormatBitsPerByte);

    if (fmt == 0)
        return Format_Text;
    else
        return Format_Image;
}

int LsbCrypt::encryptedObjectSize(QImage &secret)
{
    int usedSize = ceil((double) nMagicBits / nMagicBitsPerByte);
    usedSize += ceil((double) nFormatBits / nFormatBitsPerByte);
    usedSize += m_sizeOfInt;

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
        qWarning() << "encryptImage()" << "EncryptionError";
    }
    buffer.close();

    usedSize += cypherArray.size();

    return usedSize;
}

int LsbCrypt::encryptedObjectSize(QString text)
{
    int usedSize = ceil((double) nMagicBits / nMagicBitsPerByte);
    usedSize += ceil((double) nFormatBits / nFormatBitsPerByte);
    usedSize += m_sizeOfInt;

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
        qWarning() << "encryptImage()" << "EncryptionError";
    }
    buffer.close();

    usedSize += cypherArray.size();

    return usedSize;
}

int LsbCrypt::availableSize(QImage &image)
{
    /*int availableSize; =
            (cover.byteCount() * m_bitsPerByte) -
            (ceil((double) nMagicBits / nMagicBitsPerByte) +
             ceil((double) nFormatBits / nFormatBitsPerByte) + m_sizeOfInt);*/

    int headerSize = 0;

    // size taken for store format
    headerSize += ceil((double) nFormatBits / nFormatBitsPerByte);  // bytes
    // size to store bits per byte value
    headerSize += ceil((double)nMagicBits / nMagicBitsPerByte);  // bytes
    // size taken to store the size of byte array
    headerSize += ceil((double) m_sizeOfInt / m_bitsPerByte);  // bytes

    // total available bytes to hide data is given by..
    int availableSize = (image.byteCount() - headerSize) * m_bitsPerByte;

    return availableSize;
}


bool LsbCrypt::canHideImage(int byteArraySize)
{
    if (availableSize(m_stegoImage) <= byteArraySize * 8)
        return false;

    return true;
}

}   // namespace lsb

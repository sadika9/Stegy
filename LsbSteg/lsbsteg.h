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

#ifndef LSBSTEG_H
#define LSBSTEG_H

#include <QImage>
#include <QBuffer>
#include <../SimpleCrypt/simplecrypt.h>
#include <opencv2/opencv.hpp>
#include <QDebug>

namespace lsb {

class LsbStegRead
{
public:
    template <typename T>
    static bool getBit(T data, int bitNum)
    {
        return (data & (1 << bitNum)) ? true : false;
    }

    static int nextBitPos(int *currentPos, int max = 8)
    {
        ++(*currentPos);

        if (*currentPos >= max)
            *currentPos = 0;

        return *currentPos;
    }

    template <typename Tp>
    static int parseInt(Tp data, int *dataPos, int nBits, int nBitsPerByte)
    {
        if (nBitsPerByte < 1)
            return 0;

        int num = 0;

        for (int i = 0, j = 0; i < nBits; ++i, ++j)
        {

            if (j >= nBitsPerByte)
            {
                j = 0;
                ++(*dataPos);
            }

            auto ele = data[*dataPos];
            bool bit = getBit(ele, j);

            if (bit == true)
                num |= 1 << i;
            else
                num |= 0 << i;
        }

        ++(*dataPos);

        return num;
    }

    template <typename Tp1, typename Tp2>
    static void readData(Tp1 data, Tp2 dest, int *dataPos, int nBitsPerDataPos, int totBytes)
    {
        if (nBitsPerDataPos < 1)
            return;

        for (int i = 0, j = 0, pos = 0; pos < totBytes; ++i, ++j)
        {

            if (j >= nBitsPerDataPos)
            {
                j = 0;
                ++(*dataPos);
            }

            bool bit = getBit(data[*dataPos], j);

            if (i >= 8)
            {
                i = 0;
                ++pos;
                dest[pos] = 0;
            }

            if (bit == true)
                dest[pos] |= 1 << i;
            else
                dest[pos] |= 0 << i;

        }

    }

    template <typename Tp1, typename Tp2>
    static void readData2(Tp1 data, Tp2 dest, int *dataPos, int nBitsPerDataPos, int destSize)
    {
        for (int i = 0, j = 0, pos = 0; pos < destSize; ++i, ++j)
        {
            if (j == nBitsPerDataPos)
            {
                j = 0;
                ++(*dataPos);
            }

            if (i >= 8)
            {
                i = 0;
                ++pos;
                dest[pos] = 0;
            }

            bool bit = getBit(data[*dataPos], j);

            dest[pos] &= ~(1 << i);
            if (bit)
                dest[pos] |= (1 << i);

        }
    }

};

class LsbStegWrite
{
public:
    template <typename Tp>
    static void writeBit(bool bit, Tp dest, int destPos, int destPosBit)
    {
        dest[destPos] &= ~(1 << destPosBit);
        dest[destPos] |= (bit << destPosBit);
    }

    template <typename T, typename Tp>
    static void writeData(T data, Tp dest, int nBits, int bitsPerByte, int *dataPos)
    {
        for (int i = 0, bitPos = 0; i < nBits; ++i, ++bitPos)
        {
            bool bit = LsbStegRead::getBit(data, i);

            if (bitPos >= bitsPerByte)
            {
                bitPos = 0;

                ++(*dataPos);
            }

            writeBit(bit, dest, *dataPos, bitPos);
            //qDebug() << "dataPos" << *dataPos  << "bitPos" << bitPos << "bit" << bit;
        }
        ++(*dataPos);
    }
};

class LsbSteg
{
public:
    enum Format{Format_Text = 0,
                Format_Image};

    enum Error{Error_NoError = 0,
               Error_InvalidBitsPerByte,
               Error_InvalidByteArraySize,
               Error_InvalidCoverImage,
               Error_InvalidSecretImage,
               Error_InvalidStegoImage,
               Error_InvalidText,
               Error_SecretTextTooLarge,
               Error_EncryptionError,
               Error_DecryptionError,
               Error_SecretImageTooLarge,
               Error_WrongFormat,
               Error_InvalidFactor};

    LsbSteg(QString passowrd);

    QImage hideImage(QImage &coverImage, QImage &secretImage);
    QImage unhideImage(QImage &stegoImage);

    QImage hideString(QImage &coverImage, QString text);
    QString unhideString(QImage &stegoImage);

    QString password() const;
    void setPassword(const QString &password);

    Error lastError() const;

    int encryptedObjectSize(QImage &secret);
    int encryptedObjectSize(QString text);

    int availableSize(QImage &image);

    int bitsPerByte() const;
    void setBitsPerByte(int bitsPerByte);

    static bool saveImage(const char* filename, QImage image, int cvType = CV_8UC4);

    static Format getFormat(QImage &image);

private:

    inline QImage errorImage();

    bool canHideImage(int byteArraySize);

    void writeBitsPerByte();
    void readBitsPerByte();

    void writeFormat(Format format);
    void readFormat();

    void writeByteArraySize(int size);
    int readByteArraySize();

    void writeStegoData(QByteArray cypherArray);
    void readStegoData(char toArray[], int cypherByteArraySize);

    int m_dataPos;
    int m_bitsPerByte;

    static const int nFormatBits = 1;
    static const int nFormatBitsPerByte = 1;

    static const int sizeOfInt = 32;

    static const int bitsPerByteMaxValue = 8;

    static const int nBitsPerByte_Bits = 8;
    static const int nBitsPerByte_BitsPerByte = 1;

    QImage m_stegoImage;
    QImage m_secretImage;

    QString m_password;

    Format m_format;

    Error m_lastError;
};
}


#endif // LSBSTEG_H

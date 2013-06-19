/*
Following function is based on work done by Andre Somers.
original code is on qt-project.org forum
http://qt-project.org/forums/viewthread/4565/P45
*/

#ifndef PASSTOKEY_H
#define PASSTOKEY_H

#include <QCryptographicHash>
#include <QBuffer>

quint64 passToKey(const QString &password)
{
    QCryptographicHash hasher(QCryptographicHash::Md5);

    hasher.addData("!01d4a76c1biewoe,eb2c5e9d");
    hasher.addData(password.toUtf8());

    QByteArray hash;
    hash = hasher.result();

    QBuffer buffer(&hash);
    buffer.open(QIODevice::ReadOnly);

    QDataStream stream(&buffer);
    quint64 part1, part2;
    stream >> part1 >> part2;

    quint64 key = part1 ^ part2;

    return key;
}

#endif // PASSTOKEY_H

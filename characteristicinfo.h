// Copyright (C) 2013 BlackBerry Limited. All rights reserved.
// Copyright (C) 2017 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#ifndef CHARACTERISTICINFO_H
#define CHARACTERISTICINFO_H

#include <QLowEnergyCharacteristic>

#include <QObject>
#include <QString>


class CharacteristicInfo: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString characteristicName READ getName NOTIFY characteristicChanged)
    Q_PROPERTY(QString characteristicUuid READ getUuid NOTIFY characteristicChanged)
    Q_PROPERTY(QString characteristicValue READ getValue NOTIFY characteristicChanged)
    Q_PROPERTY(QString characteristicPermission READ getPermission NOTIFY characteristicChanged)


public:
    CharacteristicInfo() = default;
    CharacteristicInfo(const QLowEnergyCharacteristic &characteristic);
    void setCharacteristic(const QLowEnergyCharacteristic &characteristic);
    QString getName() const;
    QString getUuid() const;
    QString getValue() const;
    QString getPermission() const;
    QLowEnergyCharacteristic getCharacteristic() const;

signals:
    void characteristicChanged();

private:
    QLowEnergyCharacteristic m_characteristic;
};

#endif // CHARACTERISTICINFO_H

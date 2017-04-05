/*
 * This file is part of Liri Browser
 *
 * Copyright (C) 2017 Tim Süberkrüb <tim.sueberkrueb@web.de>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
*/

#ifndef EXTENSIONELEMENT_H
#define EXTENSIONELEMENT_H

#include <QObject>

class ExtensionElement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool valid READ valid WRITE setValid NOTIFY validChanged)
    Q_PROPERTY(QString extensionName READ extensionName WRITE setExtensionName NOTIFY extensionNameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit ExtensionElement(QObject *parent = nullptr);

    virtual ExtensionElement* clone(QObject *parent = nullptr);

    bool valid() const { return m_valid; }
    void setValid(bool valid) { validChanged(m_valid = valid); }

    QString extensionName() const { return m_extensionName; }
    void setExtensionName(QString extensionName) { extensionNameChanged(m_extensionName = extensionName); }

    QString name() const { return m_name; }
    void setName(QString name) { nameChanged(m_name = name); }

signals:
    void validChanged(bool valid);
    void extensionNameChanged(QString extensionName);
    void nameChanged(QString name);

protected:
    bool m_valid;
    QString m_extensionName;
    QString m_name;
};

#endif // EXTENSIONELEMENT_H

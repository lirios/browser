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

#ifndef EXTENSIONSEARCHENGINEPARAMETER_H
#define EXTENSIONSEARCHENGINEPARAMETER_H

#include <QObject>
#include <QFlag>


class ExtensionSearchEngineParameter : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionSearchEngineParameter(QObject *parent = nullptr);

    ExtensionSearchEngineParameter* clone(QObject* parent = nullptr);

    enum Type {
        Get,
        Hashbang
    };

    enum SearchContext {
        None = 0x0,
        Search = 0x1,
        Homepage = 0x2
    };

    Q_DECLARE_FLAGS(ContextFlag, SearchContext)
    Q_FLAGS(ContextFlag)

    Type type() const { return m_type; }
    void setType(Type type) { typeChanged(m_type = type); }

    QString name() const { return m_name; }
    void setName(QString name) { nameChanged(m_name = name); }

    QString value() const { return m_value; }
    void setValue(QString value) { valueChanged(m_value = value); }

    ContextFlag context() const { return m_context; }
    void setContext(ContextFlag context) { contextChanged(m_context = context); }

signals:
    void typeChanged(Type type);
    void nameChanged(QString name);
    void valueChanged(QString value);
    void contextChanged(ContextFlag context);

private:
    Type m_type;
    QString m_name;
    QString m_value;
    ContextFlag m_context;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ExtensionSearchEngineParameter::ContextFlag)

#endif // EXTENSIONSEARCHENGINEPARAMETER_H

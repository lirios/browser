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

#ifndef EXTENSIONSEARCHENGINE_H
#define EXTENSIONSEARCHENGINE_H

#include <QObject>
#include <QList>

#include "extensionelement.h"
#include "extensionsearchengineparameter.h"

class ExtensionSearchEngine : public ExtensionElement
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString summary READ summary WRITE setSummary NOTIFY summaryChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString urlBaseSearch READ urlBaseSearch WRITE setUrlBaseSearch NOTIFY urlBaseSearchChanged)
    Q_PROPERTY(QString urlBaseHomepage READ urlBaseHomepage WRITE setUrlBaseHomepage NOTIFY urlBaseHomepageChanged)
    Q_PROPERTY(QList<ExtensionSearchEngineParameter*>* parameters READ parameters NOTIFY parametersChanged)
public:
    explicit ExtensionSearchEngine(QObject *parent = nullptr);

    ExtensionSearchEngine* clone(QObject* parent = nullptr);

    QString title() const { return m_title; }
    void setTitle(QString title) { titleChanged(m_title = title); }

    QString summary() const { return m_summary; }
    void setSummary(QString summary) { summaryChanged(m_summary = summary); }

    QString description() const { return m_description; }
    void setDescription(QString description) { descriptionChanged(m_description = description); }

    QString urlBaseSearch() const { return m_urlBaseSearch; }
    void setUrlBaseSearch(QString urlBaseSearch) { urlBaseSearchChanged(m_urlBaseSearch = urlBaseSearch); }

    QString urlBaseHomepage() const { return m_urlBaseHomepage; }
    void setUrlBaseHomepage(QString urlBaseHomepage) { urlBaseHomepageChanged(m_urlBaseHomepage = urlBaseHomepage); }

    QList<ExtensionSearchEngineParameter*>* parameters() const { return m_parameters; }

signals:
    void titleChanged(QString title);
    void summaryChanged(QString summary);
    void descriptionChanged(QString description);
    void urlBaseSearchChanged(QString urlBaseSearchChanged);
    void urlBaseHomepageChanged(QString urlBaseHomepage);
    void parametersChanged(QList<ExtensionSearchEngineParameter*>* parameters);

private:
    QString m_title;
    QString m_summary;
    QString m_description;
    QString m_urlBaseSearch;
    QString m_urlBaseHomepage;
    QList<ExtensionSearchEngineParameter*>* m_parameters;
};

#endif // EXTENSIONSEARCHENGINE_H

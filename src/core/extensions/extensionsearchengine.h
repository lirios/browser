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
    Q_PROPERTY(QList<ExtensionSearchEngineParameter*> parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
public:
    explicit ExtensionSearchEngine(QObject *parent = nullptr);

    ExtensionSearchEngine* clone(QObject* parent = nullptr);

    QString title() const { return m_title; }
    void setTitle(QString title) { titleChanged(m_title = title); }

    QString summary() const { return m_summary; }
    void setSummary(QString summary) { summaryChanged(m_summary = summary); }

    QString description() const { return m_description; }
    void setDescription(QString description) { descriptionChanged(m_description = description); }

    QList<ExtensionSearchEngineParameter*> parameters() const { return m_parameters; }
    void setParameters(QList<ExtensionSearchEngineParameter*> parameters) { parametersChanged(parameters); }

signals:
    void titleChanged(QString title);
    void summaryChanged(QString summary);
    void descriptionChanged(QString description);
    void parametersChanged(QList<ExtensionSearchEngineParameter*> parameters);

private:
    QString m_title;
    QString m_summary;
    QString m_description;
    QList<ExtensionSearchEngineParameter*> m_parameters;
};

#endif // EXTENSIONSEARCHENGINE_H

#ifndef EXTENSIONSEARCHENGINEPARAMETER_H
#define EXTENSIONSEARCHENGINEPARAMETER_H

#include <QObject>
#include <QFlag>


class ExtensionSearchEngineParameter : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionSearchEngineParameter(QObject *parent = nullptr);

    enum SearchContext {
        None = 0x0,
        Search = 0x1,
        Homepage = 0x2
    };

    Q_DECLARE_FLAGS(ContextFlag, SearchContext)
    Q_FLAGS(ContextFlag)

    QString name() const { return m_name; }
    void setName(QString name) { nameChanged(m_name = name); }

    QString value() const { return m_value; }
    void setValue(QString value) { valueChanged(m_value = value); }

    ContextFlag context() const { return m_context; }
    void setContext(ContextFlag context) { contextChanged(m_context = context); }

signals:
    void nameChanged(QString name);
    void valueChanged(QString value);
    void contextChanged(ContextFlag context);

private:
    QString m_name;
    QString m_value;
    ContextFlag m_context;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ExtensionSearchEngineParameter::ContextFlag)

#endif // EXTENSIONSEARCHENGINEPARAMETER_H

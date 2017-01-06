#ifndef EXTENSIONSEARCHENGINESMODEL_H
#define EXTENSIONSEARCHENGINESMODEL_H

#include <QAbstractListModel>

#include "extensionsearchengine.h"

class ExtensionSearchEnginesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit ExtensionSearchEnginesModel(QObject *parent = nullptr);

    enum Roles {
        ExtensionName = Qt::UserRole + 1,
        Name,
        Title,
        Summary,
        Description
    };

    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    QHash<int, QByteArray> roleNames() const;
    QVariant data(const QModelIndex &index, int role) const;

    Q_INVOKABLE int count() const;
    void add(ExtensionSearchEngine* searchEngine);
    void remove(const int row);
    void remove(ExtensionSearchEngine* searchEngine);
    Q_INVOKABLE ExtensionSearchEngine *get(const int row) const;
    Q_INVOKABLE ExtensionSearchEngine *get(const QString name) const;

signals:
    void countChanged(int count);

private:
    QList<ExtensionSearchEngine*> m_searchEngines;
    ExtensionSearchEngine* m_invalidSearchEngine;
};

#endif // EXTENSIONSEARCHENGINESMODEL_H

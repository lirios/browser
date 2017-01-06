#include "extensionsearchenginesmodel.h"

ExtensionSearchEnginesModel::ExtensionSearchEnginesModel(QObject *parent) : QAbstractListModel(parent)
{
    m_invalidSearchEngine = new ExtensionSearchEngine(this);
    m_invalidSearchEngine->setValid(false);
}

int ExtensionSearchEnginesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_searchEngines.length();
}

QHash<int, QByteArray> ExtensionSearchEnginesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ExtensionName] = "extensionName";
    roles[Name] = "name";
    roles[Title] = "title";
    roles[Summary] = "summary";
    roles[Description] = "description";
    return roles;
}

QVariant ExtensionSearchEnginesModel::data(const QModelIndex &index, int role) const
{
    ExtensionSearchEngine* searchEngine = get(index.row());
    switch (role) {
        case ExtensionName:
            return searchEngine->extensionName();
        case Name:
            return searchEngine->name();
        case Title:
            return searchEngine->title();
        case Summary:
            return searchEngine->summary();
        case Description:
            return searchEngine->description();
    }
    return QVariant();
}

int ExtensionSearchEnginesModel::count() const
{
    return m_searchEngines.length();
}

void ExtensionSearchEnginesModel::add(ExtensionSearchEngine *searchEngine)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_searchEngines.append(searchEngine);
    countChanged(count());
    endInsertRows();
}

void ExtensionSearchEnginesModel::remove(const int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    ExtensionSearchEngine* searchEngine = m_searchEngines.at(row);
    m_searchEngines.removeAt(row);
    searchEngine->deleteLater();
    countChanged(count());
    endRemoveRows();
}

void ExtensionSearchEnginesModel::remove(ExtensionSearchEngine *searchEngine)
{
    int index = m_searchEngines.indexOf(searchEngine);
    if (index != -1)
        remove(index);
}

ExtensionSearchEngine *ExtensionSearchEnginesModel::get(const int row) const
{
    if (row < 0 || row >= count()) {
        return m_invalidSearchEngine;
    }
    return m_searchEngines.at(row);
}

ExtensionSearchEngine *ExtensionSearchEnginesModel::get(const QString name) const
{
    for (ExtensionSearchEngine* searchEngine : m_searchEngines) {
        if (searchEngine->name() == name)
            return searchEngine;
    }
    return m_invalidSearchEngine;
}

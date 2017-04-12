#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QVariantList>
#include <QList>

class TabsModel;
class TabState;

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = 0);

    Q_INVOKABLE void save(TabsModel* tabs);
    Q_INVOKABLE QVariantList getTabsToRestore();
signals:

public slots:

private:
    void load();
    QByteArray json(TabsModel *tabs);

private:
    QList<TabState*> m_tabs;
};

#endif // SESSION_H

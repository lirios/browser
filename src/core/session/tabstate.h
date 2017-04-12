#ifndef TABSTATE_H
#define TABSTATE_H

#include <QObject>

class TabState : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl)
    Q_PROPERTY(float readingProgress READ readingProgress WRITE setReadingProgress)

public:
    explicit TabState(QObject *parent = 0);

    void setUrl(QString url);
    QString url() const;
    float readingProgress() const;
    void setReadingProgress(float readingProgress);

signals:

public slots:

private:
    QString m_url;
    float m_readingProgress;
};

#endif // TABSTATE_H

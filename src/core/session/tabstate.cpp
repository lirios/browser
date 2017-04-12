#include "tabstate.h"

TabState::TabState(QObject *parent) : QObject(parent)
{

}

void TabState::setUrl(QString url)
{
    if (m_url == url)
        return;

    m_url = url;
}

QString TabState::url() const
{
    return m_url;
}

float TabState::readingProgress() const
{
    return m_readingProgress;
}

void TabState::setReadingProgress(float readingProgress)
{
    m_readingProgress = readingProgress;
}

#include "darkthemetimer.h"

DarkThemeTimer::DarkThemeTimer(QObject *parent)
    : QObject(parent)
{
    m_timer = new QTimer(this);
    m_timer->setInterval(60000);
    connect(m_timer, &QTimer::timeout, this, &DarkThemeTimer::update);
}

void DarkThemeTimer::start()
{
    m_timer->start();
}

void DarkThemeTimer::stop()
{
    m_timer->stop();
}

void DarkThemeTimer::update()
{
    bool isDark = false;
    QTime now = QTime::currentTime();
    if (m_endTime > m_startTime) {
        isDark = (now > m_startTime && now < m_endTime);
    }
    else {
        isDark = (now > m_startTime || now < m_endTime );
    }
    isActiveTimeChanged(m_isActiveTime = isDark);
}

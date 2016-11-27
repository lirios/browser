#ifndef DARKTHEMETIMER_H
#define DARKTHEMETIMER_H

#include <QObject>
#include <QTime>
#include <QTimer>

class DarkThemeTimer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QTime startTime READ startTime WRITE setStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QTime endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)
    Q_PROPERTY(bool isActiveTime MEMBER m_isActiveTime NOTIFY isActiveTimeChanged)
public:
    explicit DarkThemeTimer(QObject *parent = 0);

    QTime startTime() const { return m_startTime; }
    void setStartTime(QTime startTime) { startTimeChanged(m_startTime = startTime); }

    QTime endTime() const { return m_endTime; }
    void setEndTime(QTime endTime) { endTimeChanged(m_endTime = endTime); }

    Q_INVOKABLE void start();
    Q_INVOKABLE void stop();

signals:
    void startTimeChanged(QTime startTime);
    void endTimeChanged(QTime endTime);
    void isActiveTimeChanged(bool isActiveTime);

private:
    QTime m_startTime;
    QTime m_endTime;
    QTimer* m_timer;
    bool m_isActiveTime;

public slots:
    Q_INVOKABLE void update();

};

#endif // DARKTHEMETIMER_H

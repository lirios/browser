#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QObject>
#include <QTime>

class ThemeConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool themeColorEnabled READ themeColorEnabled WRITE setThemeColorEnabled NOTIFY themeColorEnabledChanged)
    Q_PROPERTY(bool darkThemeEnabled READ darkThemeEnabled WRITE setDarkThemeEnabled NOTIFY darkThemeEnabledChanged)
    Q_PROPERTY(QTime darkThemeStartTime READ darkThemeStartTime WRITE setDarkThemeStartTime NOTIFY darkThemeStartTimeChanged)
    Q_PROPERTY(QTime darkThemeEndTime READ darkThemeEndTime WRITE setDarkThemeEndTime NOTIFY darkThemeEndTimeChanged)
public:
    explicit ThemeConfig(QObject *parent = 0);

    bool themeColorEnabled() const { return m_themeColorEnabled; }
    void setThemeColorEnabled(bool themeColorEnabled) { themeColorEnabledChanged(m_themeColorEnabled = themeColorEnabled); }

    bool darkThemeEnabled() const { return m_darkThemeEnabled; }
    void setDarkThemeEnabled(bool darkThemeEnabled) { darkThemeEnabledChanged(m_darkThemeEnabled = darkThemeEnabled); }

    QTime darkThemeStartTime() const { return m_darkThemeStartTime; }
    void setDarkThemeStartTime(QTime darkThemeStartTime) { darkThemeStartTimeChanged(m_darkThemeStartTime = darkThemeStartTime); }
    Q_INVOKABLE void setDarkThemeStartTime(QString darkThemeStartTime, QString format) {
        darkThemeStartTimeChanged(m_darkThemeStartTime = QTime::fromString(darkThemeStartTime, format));
    }

    QTime darkThemeEndTime() const { return m_darkThemeEndTime; }
    void setDarkThemeEndTime(QTime darkThemeEndTime) { darkThemeEndTimeChanged(m_darkThemeEndTime = darkThemeEndTime); }
    Q_INVOKABLE void setDarkThemeEndTime(QString darkThemeEndTime, QString format) {
        darkThemeEndTimeChanged(m_darkThemeEndTime = QTime::fromString(darkThemeEndTime, format));
    }

signals:
    void themeColorEnabledChanged(bool themeColorEnabled);
    void darkThemeEnabledChanged(bool darkThemeEnabled);
    void darkThemeStartTimeChanged(QTime darkThemeStartTime);
    void darkThemeEndTimeChanged(QTime darkThemeEndTime);

private:
    bool m_themeColorEnabled;
    bool m_darkThemeEnabled;
    QTime m_darkThemeStartTime;
    QTime m_darkThemeEndTime;
};

#endif // THEMECONFIG_H

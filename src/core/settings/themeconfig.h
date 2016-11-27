#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QObject>

class ThemeConfig : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool themeColorEnabled READ themeColorEnabled WRITE setThemeColorEnabled NOTIFY themeColorEnabledChanged)
public:
    explicit ThemeConfig(QObject *parent = 0);

    bool themeColorEnabled() const { return m_themeColorEnabled; }
    void setThemeColorEnabled(bool themeColorEnabled) { themeColorEnabledChanged(m_themeColorEnabled = themeColorEnabled); }

signals:
    void themeColorEnabledChanged(bool themeColorEnabled);

private:
    bool m_themeColorEnabled;

};

#endif // THEMECONFIG_H

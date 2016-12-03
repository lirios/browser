#include "themeconfig.h"

ThemeConfig::ThemeConfig(QObject *parent)
    : QObject(parent)
{
    m_darkThemeStartTime = QTime(21, 0);
    m_darkThemeEndTime = QTime(7, 0);
}

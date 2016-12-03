#ifndef PATHS_H
#define PATHS_H

#include <QString>
#include <QStandardPaths>

namespace Paths {
    const QString ConfigLocation = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/liri-browser/";
    const QString DataLocation = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/liri-browser/";
    const QString SettingsFile = ConfigLocation + "settings.json";
    const QString ExtensionsLocation = DataLocation + "extensions/";
}

#endif // PATHS_H

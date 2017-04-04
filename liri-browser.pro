load(liri_deployment)

TEMPLATE = app
TARGET = liri-browser

CONFIG += c++11
QT += qml quick quickcontrols2


unix:!android {
    target.path = $$LIRI_INSTALL_BINDIR

    icon.path = $$LIRI_INSTALL_PREFIX/share/icons/hicolor
    icon.files += res/icon/512x512

    desktop.path = $$LIRI_INSTALL_PREFIX/share/applications
    desktop.files += res/io.liri.Browser.desktop

    # Deprecation warning for PREFIX
    prefix = $$(PREFIX)
    !isEmpty(prefix) {
        warning("Using the PREFIX environment variable to specify the installation prefix is deprecated.")
        warning("Use qmake LIRI_INSTALL_PREFIX=<path> instead.")
        target.path = $${prefix}/bin
    }

    INSTALLS += target icon desktop
}

# Specify CONFIG+=QTWEBENGINE_ENABLED when running qmake.
# Otherwise, Liri Browser expects the Oxide web engine.
contains(CONFIG, QTWEBENGINE_ENABLED) {
    message("Found QTWEBENGINE_ENABLED in CONFIG")
    QT += webengine
    DEFINES += IS_QTWEBENGINE_ENABLED
    message("Using QtWebEngine")
}

# Enable High DPI scaling if Qt >= 5.6
greaterThan(QT_MAJOR_VERSION, 4) {
    greaterThan(QT_MINOR_VERSION, 5) {
        DEFINES += ENABLE_HIGH_DPI_SCALING
        message("Using high-dpi scaling")
    }
}

# Include sub project include files
include(src/3rdparty/3rdparty.pri)
include(src/core/core.pri)
include(src/main/main.pri)
include(src/ui/ui.pri)

RESOURCES += res/icon/icon.qrc

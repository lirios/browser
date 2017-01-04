load(liri_deployment)

TEMPLATE = app
TARGET = liri-browser

CONFIG += c++11
QT += qml quick quickcontrols2 widgets


unix:!android {
    target.path = $$LIRI_INSTALL_BINDIR

    ICONS_SIZES = 16 22 24 32 48 64 128 192 256
    for(size, ICONS_SIZES) {
        eval(icon$${size}.files = res/icons/$${size}x$${size}/io.liri.Browser.png)
        eval(icon$${size}.path = $$LIRI_INSTALL_PREFIX/share/icons/hicolor/$${size}x$${size}/apps)
        INSTALLS += icon$${size}
    }
    iconSVG.files = res/icons/scalable/io.liri.Browser.svg
    iconSVG.path = $$LIRI_INSTALL_PREFIX/share/icons/hicolor/scalable/apps
    INSTALLS += iconSVG

    desktop.path = $$LIRI_INSTALL_PREFIX/share/applications
    desktop.files += res/io.liri.Browser.desktop

    # Deprecation warning for PREFIX
    prefix = $$(PREFIX)
    !isEmpty(prefix) {
        warning("Using the PREFIX environment variable to specify the installation prefix is deprecated.")
        warning("Use qmake LIRI_INSTALL_PREFIX=<path> instead.")
        target.path = $${prefix}/bin
    }

    INSTALLS += target desktop
}

# Load QuaZIP library
linux:LIBS += -L/usr/local/lib -lquazip

# Show useful files in QtCreator
OTHER_FILES += README.md \
               LICENSE*

# Specify CONFIG+=QTWEBENGINE_ENABLED when running qmake.
# Otherwise, Liri Browser expects the Oxide web engine.
contains(CONFIG, QTWEBENGINE_ENABLED) {
    message("Found QTWEBENGINE_ENABLED in CONFIG")
    QT += webengine
    DEFINES += IS_QTWEBENGINE_ENABLED
    message("Using QtWebEngine")
}

# Include sub project include files
include(src/3rdparty/3rdparty.pri)
include(src/core/core.pri)
include(src/main/main.pri)
include(src/ui/ui.pri)

RESOURCES += res/icons/icons.qrc

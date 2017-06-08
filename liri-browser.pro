load(liri_deployment)

TEMPLATE = app
TARGET = liri-browser

CONFIG += c++11
QT += qml quick quickcontrols2 webengine


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

    INSTALLS += target desktop
}

RESOURCES += res/icons/icons.qrc

TRANSLATIONS += res/translations/browser.ts
OTHER_FILES += $$TRANSLATIONS

# Include sub project include files
include(src/3rdparty/3rdparty.pri)
include(src/core/core.pri)
include(src/main/main.pri)
include(src/ui/ui.pri)

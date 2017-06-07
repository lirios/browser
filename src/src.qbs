import qbs 1.0

QtGuiApplication {
    name: "liri-browser"
    consoleApplication: false

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick", "quickcontrols2", "webengine"] }

    files: [
        "3rdparty/regex-weburl/*.qrc",
        "core/models/*.cpp",
        "core/models/*.h",
        "core/settings/*.cpp",
        "core/settings/*.h",
        "core/global/*.cpp",
        "core/global/*.h",
        "core/utils/*.cpp",
        "core/utils/*.h",
        "main/*.cpp",
        "main/*.h",
        "ui/*.qrc",
        "../res/icons/*.qrc"
    ]
// TODO: mac

    Group {
        qbs.install: true
        qbs.installDir: {
            if (qbs.targetOS.contains("windows"))
                return "";
            else if (qbs.targetOS.contains("darwin"))
                return "Contents/MacOS";
            else
                return lirideployment.binDir;
        }
        fileTagsFilter: product.type
    }
}

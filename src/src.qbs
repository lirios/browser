import qbs 1.0

QtGuiApplication {
    name: "liri-browser"
    consoleApplication: false

    Qt.core.resourcePrefix: "/"
    Qt.core.resourceSourceBase: "../src"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick", "quickcontrols2", "webengine"] }

    Group {
        name: "Core"
        prefix: "core/"
        files: ["**"]
    }

    Group {
        name: "Main"
        files: ["main/main.cpp"]
    }

    Group {
        condition: qbs.targetOS.contains("darwin")
        name: "macOS"
        files: ["main/mac/**"]
    }

    Group {
        name: "Third party"
        prefix: "3rdparty/"
        files: ["**"]
    }

    Group {
        name: "UI"
        prefix: "ui/"
        files: ["**"]
        fileTags: ["qt.core.resource_data"]
    }

    Group {
        name: "Translations"
        files: ["*_*.ts"]
        prefix: "../res/translations/"
    }

    Group {
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/liri-browser/translations"
        fileTagsFilter: "qm"
    }

    Group {
        name: "In-app icons"
        prefix: "../res/icons/"
        files: [
            "256x256/io.liri.Browser.png",
            "512x512/io.liri.Browser.png",
            "scalable/io.liri.Browser.svg",
        ]
        fileTags: ["qt.core.resource_data"]
    }

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

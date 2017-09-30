import qbs 1.0

QtGuiApplication {
    readonly property bool isBundle: qbs.targetOS.contains("darwin") && bundle.isBundle

    name: "liri-browser"
    consoleApplication: false

    Properties {
        condition: qbs.targetOS.contains("macos")
        cpp.frameworks: ["AppKit", "Foundation"]
    }

    bundle.identifierPrefix: "io.liri"
    bundle.identifier: "io.liri.Browser"

    Qt.core.resourcePrefix: "/"
    Qt.core.resourceSourceBase: "../src"

    Depends { name: "lirideployment" }
    Depends { name: "Qt"; submodules: ["qml", "quick", "quickcontrols2", "webengine"] }
    Depends { name: "ib"; condition: qbs.targetOS.contains("macos") }

    files: [
        "core/**",
        "main/main.cpp",
        "3rdparty/**",
    ]

    Group {
        qbs.install: true
        qbs.installDir: {
            if (qbs.targetOS.contains("linux"))
                return lirideployment.binDir;
            else
                return "";
        }
        qbs.installSourceBase: isBundle ? product.buildDirectory : ""
        fileTagsFilter: isBundle ? ["bundle.content"] : ["application"]
    }

    Group {
        condition: qbs.targetOS.contains("macos")
        name: "macOS"
        files: ["main/mac/**"]
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
}

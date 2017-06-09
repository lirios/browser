import qbs 1.0

Product {
    name: "Resources"

    Depends { name: "lirideployment" }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Desktop File"
        files: ["io.liri.Browser.desktop"]
        qbs.install: true
        qbs.installDir: lirideployment.applicationsDir
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon 16x16"
        files: ["icons/16x16/io.liri.Browser.png"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/16x16/apps"
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon 32x32"
        files: ["icons/32x32/io.liri.Browser.png"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/32x32/apps"
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon 64x64"
        files: ["icons/64x64/io.liri.Browser.png"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/64x64/apps"
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon 128x128"
        files: ["icons/128x128/io.liri.Browser.png"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/128x128/apps"
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon 192x192"
        files: ["icons/192x192/io.liri.Browser.png"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/192x192/apps"
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon 256x256"
        files: ["icons/256x256/io.liri.Browser.png"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/256x256/apps"
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon 512x512"
        files: ["icons/512x512/io.liri.Browser.png"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/512x512/apps"
    }

    Group {
        condition: qbs.hostOS.contains("linux")
        name: "Icon scalable"
        files: ["icons/scalable/io.liri.Browser.svg"]
        qbs.install: true
        qbs.installDir: lirideployment.dataDir + "/icons/hicolor/512x512/apps"
    }
}

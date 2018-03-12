import qbs 1.0

DynamicLibrary {
    name: "qtsingleapplication"

    files: [
        "QtLockedFile", "QtSingleApplication",
        "*.cpp", "*.h",
    ]
    excludeFiles: ["*_win.cpp", "*_unix.cpp", "qtlockedfile.cpp"]

    Group {
        name: "lib"
        fileTagsFilter: ["dynamiclibrary"]
        qbs.install: true
        qbs.installDir: {
            if (qbs.targetOS.contains("linux"))
                return lirideployment.libDir;
            else
                return "";
        }
        qbs.installSourceBase: destinationDirectory
    }

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["widgets", "network"] }
    Depends { name: "lirideployment" }

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["widgets", "network"] }
        cpp.includePaths: [product.sourceDirectory]
    }
}

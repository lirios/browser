import qbs 1.0

StaticLibrary {
    name: "qtsingleapplication"

    files: [
        "QtLockedFile", "QtSingleApplication",
        "*.cpp", "*.h",
    ]
    excludeFiles: ["*_win.cpp", "*_unix.cpp", "qtlockedfile.cpp"]

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["widgets", "network"] }
    Depends { name: "lirideployment" }

    Export {
        Depends { name: "cpp" }
        Depends { name: "Qt"; submodules: ["widgets", "network"] }
        cpp.includePaths: [product.sourceDirectory]
    }
}

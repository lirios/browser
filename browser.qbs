import qbs 1.0

Project {
    name: "Browser"

    readonly property string version: "0.0.0"

    minimumQbsVersion: "1.6"

    qbsSearchPaths: "qbs/shared"

    references: [
        "res/res.qbs",
        "src/src.qbs",
    ]
}

import qbs 1.0

Project {
    name: "Browser"

    readonly property string version: "0.0.0"

    property bool useStaticAnalyzer: false

    property bool withFluid: false

    minimumQbsVersion: "1.6"

    qbsSearchPaths: {
        var paths = [];
        if (withFluid)
            paths.push("fluid/qbs/shared");
        return paths;
    }

    references: [
        "res/res.qbs",
        "src/src.qbs",
    ]

    SubProject {
        filePath: "fluid/fluid.qbs"

        Properties {
            condition: withFluid ||
                       qbs.targetOS.contains("windows") ||
                       qbs.targetOS.contains("macos") ||
                       qbs.targetOS.contains("android")
            autotestEnabled: false
            deploymentEnabled: false
            withDocumentation: false
            withDemo: false
        }
    }

    Product {
        name: "Documents"
        files: [
            "AUTHORS.md",
            "README.md",
            "LICENSE.*"
        ]
    }
}

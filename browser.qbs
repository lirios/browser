import qbs 1.0

Project {
    name: "Browser"

    readonly property string version: "0.0.0"

    property bool useStaticAnalyzer: false

    property bool withFluid: qbs.targetOS.contains("windows") ||
                             qbs.targetOS.contains("macos") ||
                             qbs.targetOS.contains("android")

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
            condition: withFluid
            useSystemQbsShared: false
            autotestEnabled: false
            deploymentEnabled: false
            withDocumentation: false
            withDemo: false
        }
    }

    SubProject {
        filePath: "3rdparty/qtsingleapplication/qtsingleapplication.qbs"
    }

    Product {
        name: "Browser Documents"
        files: [
            "AUTHORS.md",
            "README.md",
            "LICENSE.*"
        ]
    }
}

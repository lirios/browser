Liri Browser
============

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/browser.svg)](https://github.com/lirios/browser)
[![Build Status](https://travis-ci.org/lirios/browser.svg?branch=develop)](https://travis-ci.org/lirios/browser)
[![Snap Status](https://build.snapcraft.io/badge/lirios/browser.svg)](https://build.snapcraft.io/user/lirios/browser)
[![GitHub issues](https://img.shields.io/github/issues/lirios/browser.svg)](https://github.com/lirios/browser/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2018.svg)](https://github.com/lirios/browser/commits/develop)

A cross-platform Material Design web browser.

## Dependencies

Qt >= 5.8.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)
 * [qtwebengine](http://code.qt.io/cgit/qt/qtwebengine.git/)
 * [qtsvg](http://code.qt.io/cgit/qt/qtsvg.git/)
 * [qtgraphicaleffects](http://code.qt.io/cgit/qt/qtgraphicaleffects.git/)

The following modules and their dependencies are required:

 * [qbs-shared](https://github.com/lirios/qbs-shared.git)
 * [fluid](https://github.com/lirios/fluid.git)

## Installation

We use the [Qbs](http://doc.qt.io/qbs/) build system.

If you want to learn more, please read the [Qbs manual](http://doc.qt.io/qbs/index.html),
especially the [setup guide](http://doc.qt.io/qbs/configuring.html) and how to install artifacts
from the [installation guide](http://doc.qt.io/qbs/installing-files.html).

If you haven't already, start by setting up a `qt5` profile for `qbs`:

```sh
qbs setup-toolchains --type gcc /usr/bin/g++ gcc
qbs setup-qt $(which qmake) qt5 # make sure that qmake is in PATH
qbs config profiles.qt5.baseProfile gcc
```

Then, from the root of the repository, run:

```sh
qbs -d build -j $(nproc) profile:qt5 # use sudo if necessary
```

To the `qbs` call above you can append additional configuration parameters:

 * `modules.lirideployment.prefix:/path/to/prefix` where most files are installed (default: `/usr/local`)
 * `modules.lirideployment.dataDir:path/to/lib` where data files are installed (default: `/usr/local/share`)
 * `modules.lirideployment.libDir:path/to/lib` where libraries are installed (default: `/usr/local/lib`)
 * `modules.lirideployment.qmlDir:path/to/qml` where QML plugins are installed (default: `/usr/local/lib/qml`)
 * `modules.lirideployment.pluginsDir:path/to/plugins` where Qt plugins are installed (default: `/usr/local/lib/plugins`)
 * `modules.lirideployment.qbsModulesDir:path/to/qbs` where Qbs modules are installed (default: `/usr/local/share/qbs/modules`)

See [lirideployment.qbs](https://github.com/lirios/qbs-shared/blob/develop/modules/lirideployment/lirideployment.qbs)
for more deployment-related parameters.

You can also append the following options to the last line:

 * `project.withFluid:true`: use git submodule for Fluid

## Documentation

Find more information in the [Liri Browser documentation](https://liri-dev.readthedocs.io/en/latest/modules/browser/).

## Credits

Many thanks to ...

 * [Corbin Crutchley](https://github.com/crutchcorn) for creating the application icon
 * [Simon Peter](https://github.com/probonopd) for adding AppImage support

Please refer to [AUTHORS.md](AUTHORS.md) for a list of everyone who directly contributed code via Git.

## Licensing

Contains the [QtSingleApplication](https://github.com/qtproject/qt-solutions/tree/master/qtsingleapplication)
(see the `qtsingleapplication` folder) module which is licensed under the 3-clause BSD license (see `LICENSE.BSD`).

Liri Browser is licensed under the terms of the GNU General Public License version 3 or, at your option, any later version.

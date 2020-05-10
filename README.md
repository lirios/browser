Liri Browser
============

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/browser.svg)](https://github.com/lirios/browser)
[![GitHub issues](https://img.shields.io/github/issues/lirios/browser.svg)](https://github.com/lirios/browser/issues)
[![Snap Status](https://build.snapcraft.io/badge/lirios/browser.svg)](https://build.snapcraft.io/user/lirios/browser)
[![CI](https://github.com/lirios/browser/workflows/CI/badge.svg?branch=develop&event=push)](https://github.com/lirios/browser/actions?query=workflow%3ACI)

A cross-platform Material Design web browser.

## Dependencies

Qt >= 5.10.0 with at least the following modules is required:

 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)
 * [qtwebengine](http://code.qt.io/cgit/qt/qtwebengine.git/)
 * [qtsvg](http://code.qt.io/cgit/qt/qtsvg.git/)
 * [qtgraphicaleffects](http://code.qt.io/cgit/qt/qtgraphicaleffects.git/)

The following modules and their dependencies are required:

 * [cmake](https://gitlab.kitware.com/cmake/cmake) >= 3.10.0
 * [cmake-shared](https://github.com/lirios/cmake-shared.git) >= 1.0.0
 * [fluid](https://github.com/lirios/fluid.git) >= 1.0.0

## Installation

```sh
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/path/to/prefix ..
make
make install # use sudo if necessary
```

Replace `/path/to/prefix` to your installation prefix.
Default is `/usr/local`.

You can also append the following options to the `cmake` command:

 * `-DBROWSER_WITH_FLUID:BOOL=ON`: Build with a local copy of the Fluid sources.

## Documentation

Find more information in the [Liri Browser documentation](https://liri-dev.readthedocs.io/en/latest/modules/browser/).

## Credits

Many thanks to ...

 * [Corbin Crutchley](https://github.com/crutchcorn) for creating the application icon
 * [Simon Peter](https://github.com/probonopd) for adding AppImage support

Please refer to [AUTHORS.md](AUTHORS.md) for a list of everyone who directly contributed code via Git.

## Licensing

Contains the [QtSingleApplication](https://github.com/qtproject/qt-solutions/tree/master/qtsingleapplication)
(see the `3rdparty/qtsingleapplication` folder) module which is licensed under the 3-clause BSD license (see `LICENSE.BSD`).

Liri Browser is licensed under the terms of the GNU General Public License version 3 or, at your option, any later version.

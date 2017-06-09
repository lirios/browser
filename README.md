Liri Browser
============

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/browser.svg)](https://github.com/lirios/browser)
[![Build Status](https://travis-ci.org/lirios/browser.svg?branch=develop)](https://travis-ci.org/lirios/browser)
[![GitHub issues](https://img.shields.io/github/issues/lirios/browser.svg)](https://github.com/lirios/browser/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2017.svg)](https://github.com/lirios/browser/commits/develop)

A cross-platform material design web browser

## Dependencies
* Qt >= 5.8.0 with at least the following modules is required:
  * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
  * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
  * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)
  * [qtwebengine](http://code.qt.io/cgit/qt/qtwebengine.git/)
  * [qtsvg](http://code.qt.io/cgit/qt/qtsvg.git/)
  * [qtgraphicaleffects](http://code.qt.io/cgit/qt/qtgraphicaleffects.git/)

The following modules and their dependencies are required:
* [Fluid](https://github.com/lirios/fluid) from develop branch

## Build

Liri Browser uses [Qbs](http://doc.qt.io/qbs/) as build system.

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
```sh
... project.withFluid:true # use git submodule for Fluid
... qbs.installRoot:/install/root # e.g. /
... qbs.installPrefix:path/to/install # e.g. opt/liri or usr
```

## Documentation

Find more information in the [Liri Browser wiki](https://github.com/lirios/browser/wiki).

## Credits
Many thanks to ...
* [Corbin Crutchley](https://github.com/crutchcorn) for creating the application icon
* [Simon Peter](https://github.com/probonopd) for adding AppImage support

Please refer to [AUTHORS.md](AUTHORS.md) for a list of everyone who directly contributed code via Git.

## Licensing
Licensed under the terms of the GNU General Public License version 3 or, at your option, any later version.

Liri Browser
============

[![License](https://img.shields.io/badge/license-GPLv3.0-blue.svg)](https://www.gnu.org/licenses/gpl-3.0.html)
[![GitHub release](https://img.shields.io/github/release/lirios/browser.svg)](https://github.com/lirios/browser)
[![Build Status](https://travis-ci.org/lirios/browser.svg?branch=master)](https://travis-ci.org/lirios/browser)
[![GitHub issues](https://img.shields.io/github/issues/lirios/browser.svg)](https://github.com/lirios/browser/issues)
[![Maintained](https://img.shields.io/maintenance/yes/2016.svg)](https://github.com/lirios/browser/commits/master)

A cross-platform material design web browser

## Dependencies
* Qt >= 5.7.0 with at least the following modules is required:
 * [qtbase](http://code.qt.io/cgit/qt/qtbase.git)
 * [qtdeclarative](http://code.qt.io/cgit/qt/qtdeclarative.git)
 * [qtquickcontrols2](http://code.qt.io/cgit/qt/qtquickcontrols2.git)

The following modules and their dependencies are required:
* [fluid](https://github.com/lirios/fluid)
* [SlimeEngine](https://github.com/tim-sueberkrueb/slime-engine)
    * with either [Oxide](https://launchpad.net/oxide) or [QtWebEngine](http://code.qt.io/cgit/qt/qtwebengine.git/)

## Build

From the root of the repository, run:
```sh
cd liri-browser
mkdir build && cd build
qmake ..
make
```
Note: Oxide web engine is expected by default. To build for QtWebEngine, add `CONFIG+=QTWEBENGINE_ENABLED` to your `qmake` line:
```sh
qmake .. CONFIG+=QTWEBENGINE_ENABLED
```

Use `make distclean` from inside your `build` directory to clean up.
You need to do this before rerunning `qmake` with different options. 

## Install

From your build directory, run:
```sh
sudo make install
```
The browser will be installed to `/usr/local` by default. To specify a custom installation prefix,
set the `PREFIX` environment variable when running `qmake`. For example:
```sh
PREFIX=/opt qmake ..
```

## Licensing
Licensed under the terms of the GNU General Public License version 3 or, at your option, any later version.

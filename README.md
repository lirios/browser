# Liri Browser

A cross-platform material design web browser

## Installation

### Dependencies
* Qt >= 5.7
* [Fluid](https://github.com/lirios/fluid)
* [SlimeEngine](https://github.com/tim-sueberkrueb/slime-engine)
    * with either [Oxide](https://launchpad.net/oxide) or [QtWebEngine](http://code.qt.io/cgit/qt/qtwebengine.git/)

### Get the Source

```
git clone https://github.com/lirios/browser liri-browser
```

### Build

```
cd liri-browser
mkdir build && cd build
qmake ..
make
```
Note: Oxide web engine is expected by default. To build for QtWebEngine, add `CONFIG+=QTWEBENGINE_ENABLED` to your `qmake` line.

Use `make distclean` from inside your `build` directory to clean up.
You need to do this before rerunning `qmake` with different options. 

# Licensing
Licensed under the terms of the GNU General Public License version 3 or, at your option, any later version.

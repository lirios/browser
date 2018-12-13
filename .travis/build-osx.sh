#!/bin/bash

set -e

source /usr/local/share/liri-travis/functions

# Update brew
travis_start "brew_update"
msg "Update..."
brew update
travis_end "brew_update"

# Install dependencies
travis_start "brew_install"
msg "Install packages..."
brew install qt5
brew link qt5 --force
echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.bash_profile
export PATH="/usr/local/opt/qt/bin:$PATH"
export LDFLAGS="-L/usr/local/opt/qt/lib"
export CPPFLAGS="-I/usr/local/opt/qt/include"
export PKG_CONFIG_PATH="/usr/local/opt/qt/lib/pkgconfig"
travis_end "brew_install"

# Fluid
travis_start "fluid"
msg "Build Fluid..."
git clone -b develop git://github.com/lirios/fluid.git
cd fluid
git submodule update --init
mkdir build
cd build
cmake .. \
    -DFLUID_WITH_DOCUMENTATION:BOOL=OFF \
    -DFLUID_WITH_DEMO:BOOL=OFF \
    -DCMAKE_INSTALL_PREFIX=/usr/local/opt/qt \
    -DINSTALL_LIBDIR=/usr/local/opt/qt/lib \
    -DINSTALL_QMLDIR=/usr/local/opt/qt/qml \
    -DINSTALL_PLUGINSDIR=/usr/local/opt/qt/lib/plugins
make -j $(sysctl -n hw.ncpu)
sudo make install
cd ../..
travis_end "fluid"

# Configure
travis_start "configure"
msg "Setup CMake..."
mkdir build
cd build
cmake .. \
    -DLIRI_LOCAL_ECM:BOOL=ON \
    -DCMAKE_INSTALL_PREFIX=/opt/liri \
    -DINSTALL_LIBDIR=/opt/liri/lib \
    -DINSTALL_QMLDIR=/opt/liri/lib/qml \
    -DINSTALL_PLUGINSDIR=/opt/liri/lib/plugins
travis_end "configure"

# Build
travis_start "build"
msg "Build..."
make -j $(sysctl -n hw.ncpu)
travis_end "build"

# Package
travis_start "package"
msg "Package..."
srcdir=$(pwd)/../src
#macdeployqt src/LiriBrowser.app -dmg -qmldir=$srcdir -verbose=2
#mkdir -p travis-out
#mv src/LiriBrowser.dmg travis-out/LiriBrowser-git-$(date +%Y%m%d-%H%M%S)-$(git rev-parse --short HEAD).dmg
travis_end "package"

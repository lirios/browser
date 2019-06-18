#!/bin/bash

set -e

curl "https://raw.githubusercontent.com/lirios/infra-travis/master/installer" | bash

source /usr/local/share/liri-travis/functions

# Install packages
travis_start "install_packages"
msg "Install packages..."
dnf install -y \
    desktop-file-utils \
    libappstream-glib
travis_end "install_packages"

# Install artifacts
travis_start "artifacts"
msg "Install artifacts..."
for name in cmakeshared fluid; do
    /usr/local/bin/liri-download-artifacts $TRAVIS_BRANCH ${name}-artifacts.tar.gz
done
travis_end "artifacts"

# Configure
travis_start "configure"
msg "Setup CMake..."
mkdir build
cd build
if [ "$CXX" == "clang++" ]; then
    clazy="-DCMAKE_CXX_COMPILER=clazy"
fi
cmake .. $clazy \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DINSTALL_LIBDIR=/usr/lib64 \
    -DINSTALL_QMLDIR=/usr/lib64/qt5/qml \
    -DINSTALL_PLUGINSDIR=/usr/lib64/qt5/plugins
travis_end "configure"

# Build
travis_start "build"
msg "Build..."
make -j $(nproc)
travis_end "build"

# Install
travis_start "install"
msg "Install..."
make install
travis_end "install"

# Validate desktop file and appdata
travis_start "validate"
msg "Validate..."
for filename in $(find . -type f -name "*.desktop"); do
    desktop-file-validate $filename
done
for filename in $(find . -type f -name "*.appdata.xml"); do
    appstream-util validate-relax --nonet $filename
done
travis_end "validate"

# Package
#travis_start "appimage"
#msg "Create AppImage..."
#/usr/local/bin/liri-build-appimage io.liri.Browser
#mkdir -p travis-out
#mv LiriBrowser*.AppImage travis-out/LiriBrowser-git-$(date +%Y%m%d-%H%M%S)-$(git rev-parse --short HEAD)-x86_64.AppImage
#travis_end "appimage"

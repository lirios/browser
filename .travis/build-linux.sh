#!/bin/bash

set -e

curl "https://raw.githubusercontent.com/lirios/infra-travis/master/installer" | bash

source /usr/local/share/liri-travis/functions

# Configure qbs
travis_start "qbs_setup"
msg "Setup qbs..."
qbs setup-toolchains --detect
qbs setup-qt $(which qmake-qt5) travis-qt5
qbs config profiles.travis-qt5.baseProfile $CC
travis_end "qbs_setup"

# Install qbs-shared
travis_start "qbs_shared"
msg "Install qbs-shared..."
pushd fluid/qbs/shared >/dev/null
qbs -d build -j $(nproc) profile:travis-qt5 modules.qbs.installRoot:/ project.prefix:/usr
popd >/dev/null
travis_end "qbs_shared"

# Build and install Fluid
travis_start "build_fluid"
msg "Build Fluid..."
pushd fluid >/dev/null
qbs -d build -j $(nproc) profile:travis-qt5 \
    modules.qbs.installRoot:/ \
    modules.lirideployment.prefix:/usr \
    modules.lirideployment.libDir:/usr/lib64 \
    modules.lirideployment.qmlDir:/usr/lib64/qt5/qml \
    modules.lirideployment.pluginsDir:/usr/lib64/qt5/plugins \
    project.useSystemQbsShared:true \
    project.withDocumentation:false \
    project.withDemo:false
popd >/dev/null
travis_end "build_fluid"

# Build and install app
travis_start "build_app"
msg "Build and install app..."
qbs -d build -j $(nproc) profile:travis-qt5 \
    modules.qbs.installRoot:/appdir \
    modules.lirideployment.prefix:/usr \
    project.withFluid:false
travis_end "build_app"

# Package
travis_start "appimage"
msg "Create AppImage..."
/usr/local/bin/liri-build-appimage io.liri.Browser
mkdir -p travis-out
mv Liri_Browser*.AppImage travis-out/Liri_Browser-git-$(date +%Y%m%d-%H%M%S)-$(git rev-parse --short HEAD)-x86_64.AppImage
travis_end "appimage"

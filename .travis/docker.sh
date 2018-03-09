#!/bin/bash

set -e

source /home/.travis/library.sh

# Install
travis_start "package_install"
msg "Install packages..."
apt-get install -y \
    g++ \
    git \
    curl \
    libfuse2 \
    qbs \
    qt5-default \
    qtbase5-dev qtbase5-dev-tools qtbase5-private-dev \
    qtdeclarative5-dev qtdeclarative5-dev-tools qtdeclarative5-private-dev \
    qtquickcontrols2-5-dev \
    libqt5svg5-dev qtwebengine5-dev qtwebengine5-dev-tools \
    qttools5-dev qttools5-dev-tools qttools5-private-dev \
    qml-module-qtquick2 qml-module-qtquick-window2 qml-module-qtquick-layouts qml-module-qttest \
    qml-module-qtgraphicaleffects qml-module-qtqml-models2 \
    qml-module-qtquick-controls2 qml-module-qtquick-templates2 \
    qml-module-qt-labs-platform qml-module-qt-labs-calendar \
    qml-module-qtwebengine qml-module-qtwebchannel
travis_end "package_install"

# Configure qbs
travis_start "qbs_setup"
msg "Setup qbs..."
qbs setup-toolchains --detect
qbs setup-qt $(which qmake) travis-qt5
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
    modules.lirideployment.libDir:/usr/lib/x86_64-linux-gnu \
    modules.lirideployment.qmlDir:/usr/lib/x86_64-linux-gnu/qt5/qml \
    modules.lirideployment.pluginsDir:/usr/lib/x86_64-linux-gnu/qt5/plugins \
    project.useSystemQbsShared:true \
    project.withDocumentation:false \
    project.withDemo:false
popd >/dev/null
travis_end "build_fluid"

# Build and install app
travis_start "build_app"
msg "Build and install app..."
mkdir -p appdir
qbs -d build -j $(nproc) profile:travis-qt5 \
    modules.qbs.installRoot:/appdir \
    modules.lirideployment.prefix:/usr \
    project.withFluid:false
travis_end "build_app"

# Package
travis_start "appimage"
msg "Create AppImage..."
curl -L "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage" > linuxdeployqt.AppImage
chmod a+x linuxdeployqt*.AppImage
unset QTDIR; unset QT_PLUGIN_PATH ; unset LD_LIBRARY_PATH
./linuxdeployqt.AppImage /appdir/usr/share/applications/*.desktop -bundle-non-qt-libs -qmldir=. -verbose=2
rm /appdir/io.liri.Browser.png # Workaround for linuxedeloyqt bug
./linuxdeployqt.AppImage /appdir/usr/share/applications/*.desktop -appimage -qmldir=. -verbose=2
find ./appdir -executable -type f -exec ldd {} \; | grep " => /usr" | cut -d " " -f 2-3 | sort | uniq
appimage_url="https://transfer.sh/Liri_Browser-git-$(date +%Y%m%d-%H%M%S)-$(git rev-parse --short HEAD)-x86_64.AppImage"
msg "Upload $appimage_url"
curl --upload-file ./Liri_Browser*.AppImage $appimage_url
travis_end "appimage"

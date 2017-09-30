#!/bin/bash

set -e

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    # Configure qbs
    qbs setup-toolchains --detect
    qbs setup-qt /usr/local/opt/qt/bin/qmake travis-qt5
    qbs config profiles.travis-qt5.baseProfile xcode-macosx-x86_64
    qbs config preferences.qbsSearchPaths $(pwd)/fluid/qbs/shared

    # Build and install Fluid
    cd fluid
    sudo qbs -d build -j $(sysctl -n hw.ncpu) profile:travis-qt5 qbs.installRoot:/usr/local qbs.installPrefix:/opt/qt modules.lirideployment.qmlDir:qml project.withDocumentation:false project.withDemo:false
    sudo rm -fr build

    # Build app
    cd ../
    qbs -d build -j $(sysctl -n hw.ncpu) profile:travis-qt5 project.withFluid:false
else
    # Build qbs
    git clone -b v1.9.0 https://code.qt.io/qt-labs/qbs.git qbs-src
    cd qbs-src && qmake -r qbs.pro && make -j$(nproc) && sudo make install && cd ..

    # Configure qbs
    qbs setup-toolchains --type gcc /usr/bin/g++ gcc
    qbs setup-qt $(which qmake) travis-qt5
    qbs config profiles.travis-qt5.baseProfile gcc
    qbs config preferences.qbsSearchPaths $(pwd)/fluid/qbs/shared

    # Build and install Fluid
    cd fluid
    sudo $(which qbs) -d build -j $(nproc) profile:travis-qt5 qbs.installRoot:/opt qbs.installPrefix:qt58 modules.lirideployment.qmlDir:qml project.withDocumentation:false project.withDemo:false
    sudo rm -fr build

    # Build and install app
    cd ../
    mkdir -p appdir
    qbs -d build -j $(nproc) profile:travis-qt5 project.withFluid:false qbs.installRoot:appdir qbs.installPrefix:usr
fi

#!/bin/bash

set -e

source .travis/library.sh

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    # Update
    travis_start "brew_update"
    msg "Update..."
    brew update
    travis_end

    # Install
    travis_start "brew_install"
    msg "Install packages..."
    brew install qt5 qbs
    brew link qt5 --force
    brew link qbs --force
    travis_end

    # Configure qbs
    travis_start "qbs_setup"
    qbs setup-toolchains --detect
    qbs setup-qt /usr/local/opt/qt/bin/qmake travis-qt5
    qbs config profiles.travis-qt5.baseProfile xcode-macosx-x86_64
    qbs config preferences.qbsSearchPaths $(pwd)/fluid/qbs/shared
    travis_end "qbs_setup"

    # Build and install Fluid
    travis_start "build_fluid"
    msg "Build Fluid..."
    pushd fluid >/dev/null
    sudo qbs -d build -j $(sysctl -n hw.ncpu) profile:travis-qt5 modules.qbs.installRoot:/usr/local modules.lirideployment.prefix:/opt/qt modules.lirideployment.qmlDir:/opt/qt/qml project.withDocumentation:false project.withDemo:false
    sudo rm -fr build
    popd >/dev/null
    travis_end "build_fluid"

    # Build app
    travis_start "build_app"
    msg "Build..."
    qbs -d build -j $(sysctl -n hw.ncpu) profile:travis-qt5 project.withFluid:false
    travis_end "build_app"

    # Package
    travis_start "package"
    msg "Package..."
    srcdir=$(pwd)/src
    pushd build/default/install-root >/dev/null && macdeployqt ./liri-browser.app -dmg -qmldir=$srcdir -verbose=2 && popd >/dev/null
    mv ./build/default/install-root/liri-browser.dmg Liri_Browser.dmg
    curl --upload-file ./Liri_Browser.dmg https://transfer.sh/Liri_Browser-git-$(date +%Y%m%d-%H%M%S)-$(git rev-parse --short HEAD).dmg
    travis_end "package"
else
    image=liridev/ci-ubuntu:latest

    env_vars="-e CC=$CC -e DEBIAN_FRONTEND=noninteractive"
    for line in $(env | egrep -e '^(FTP|TRAVIS|CLAZY)'); do
        env_vars="$env_vars -e $line"
    done

    sudo docker pull $image
    sudo docker run --privileged --rm -ti -v $(pwd):/home $env_vars --workdir /home $image ./.travis/docker.sh
fi

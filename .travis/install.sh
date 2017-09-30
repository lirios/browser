#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    brew install qt5 qbs
    brew link qt5 --force
    brew link qbs --force
else
    sudo apt-get install -qq g++-7
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 90
    sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-7 90
    gcc --version

    sudo apt-get -y install qt58base qt58declarative qt58quickcontrols2 qt58svg qt58webengine qt58graphicaleffects qt58tools qt58script
    source /opt/qt58/bin/qt58-env.sh
fi

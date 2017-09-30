#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    brew install qt5 qbs
    brew link qt5 --force
    brew link qbs --force
else
    sudo apt-get -y install qt58base qt58declarative qt58quickcontrols2 qt58svg qt58webengine qt58graphicaleffects qt58qbs qt58tools
    source /opt/qt58/bin/qt58-env.sh
fi

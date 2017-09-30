#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    brew update
else
    sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
    sudo add-apt-repository ppa:beineri/opt-qt58-trusty -y
    sudo apt-get update -qq
fi

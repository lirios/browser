#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
    brew update
else
    sudo add-apt-repository ppa:beineri/opt-qt58-trusty -y
    sudo apt-get update -qq
fi

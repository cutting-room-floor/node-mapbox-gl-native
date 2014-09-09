#!/bin/bash

sudo add-apt-repository --yes ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-4.8 g++-4.8

mkdir -p $BUILD
./deps/build_mbgl.sh 1>> build.log

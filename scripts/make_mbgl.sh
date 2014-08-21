#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
make setup
CXX11=true CFLAGS="-fPIC" CXXFLAGS="-fPIC" make mbgl

ls -la mapnik-packaging/osx/out
ls -la mapnik-packaging/osx/out/build-cpp11-libstdcpp-gcc-x86_64-linux/lib
ls -la mapnik-packaging/osx/out/build-cpp11-libcpp-universal/lib

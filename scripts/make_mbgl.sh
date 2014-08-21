#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
make setup
CXX11=true CFLAGS="-fPIC" CXXFLAGS="-fPIC" make mbgl

cat mapnik-packaging/osx/out/build-cpp11-libstdcpp-gcc-x86_64-linux/lib/pkgconfig/glfw3.pc

#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
make setup
CXX11=true CFLAGS="-fPIC" CXXFLAGS="-fPIC" make mbgl

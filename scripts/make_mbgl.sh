#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
make setup
CXX11=true CXXFLAGS="-fPIC" make mbgl

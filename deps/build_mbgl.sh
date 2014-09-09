#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
./scripts/travis_before_install.sh
make setup
CXX11=true CFLAGS="-fPIC" CXXFLAGS="-fPIC" make mbgl
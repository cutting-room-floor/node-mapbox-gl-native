#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
source scripts/flags.sh
./scripts/travis_before_install.sh
make setup
CXX11=true CFLAGS="-fPIC" CXXFLAGS="-fPIC" make mbgl

#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
make setup
rm -rf mapnik-packaging/out/packages
make mbgl -j4
cd ../../

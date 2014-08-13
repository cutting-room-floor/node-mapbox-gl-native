#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
./configure
make setup
make mbgl
cd ../../

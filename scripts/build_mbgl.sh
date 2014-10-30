#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
./scripts/travis_before_install.sh
PREFIX=${MBGL_PREFIX} make install -j$(nproc)

#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
./scripts/travis_before_install.sh

if [[ "$platform" == "linux" ]]; then 
    export LD_LIBRARY_PATH=`mason prefix mesa 10.3.1`/lib:$LD_LIBRARY_PATH
fi

PREFIX=${MBGL_PREFIX} make install -j$(nproc)

cd ../../

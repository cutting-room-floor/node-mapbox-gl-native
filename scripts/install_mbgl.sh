#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
./scripts/travis_before_install.sh

if [[ ${TRAVIS_OS_NAME} == "linux" ]]; then

    export LD_LIBRARY_PATH=`mason prefix mesa 10.3.1`/lib:$LD_LIBRARY_PATH
    glxinfo

    JOBS=$(nproc)

elif [[ ${TRAVIS_OS_NAME} == "osx" ]]; then

    JOBS=$(sysctl -n hw.ncpu)

fi

PREFIX=${MBGL_PREFIX} make install -j${JOBS}

cd ../../

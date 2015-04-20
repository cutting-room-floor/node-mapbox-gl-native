#!/usr/bin/env bash

set -e
set -o pipefail

# Travis OS X has no GPU
if [[ ${TRAVIS_OS_NAME} == "linux" ]]; then
    npm test
    ./scripts/deploy_results.sh
fi

# Generate coveralls test coverage
if [[ ${COVERAGE} == true ]]; then
    cpp-coveralls
    --exclude node_modules \
    --exclude test \
    --exclude vendor \
    --exclude build/Release/obj/gen \
    --build-root build \
    --gcov-options '\-lp';
fi;

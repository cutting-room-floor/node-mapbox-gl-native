#!/usr/bin/env bash

set -e
set -o pipefail

if [[ ${COVERAGE} == true ]]; then
    pip install --upgrade pip;
    pip install cpp-coveralls;
fi;

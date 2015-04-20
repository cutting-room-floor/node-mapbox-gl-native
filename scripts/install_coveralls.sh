#!/usr/bin/env bash

set -e
set -o pipefail

if [[ ${COVERAGE} == true ]]; then
    sudo pip install cpp-coveralls;
fi;

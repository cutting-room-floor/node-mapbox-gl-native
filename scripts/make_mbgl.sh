#!/usr/bin/env bash

set -e
set -o pipefail

cd vendor/mapbox-gl-native
make setup
CXX11=true make mbgl

pwd
ls -la build
ls -la build/Release
ls -la ../../build

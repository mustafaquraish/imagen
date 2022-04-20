#!/bin/bash

parent=$(dirname $(pwd))
cur=$(basename $(pwd))

set -xe

docker run --rm -v $parent:/mnt -w /mnt/$cur emscripten/emsdk make ems
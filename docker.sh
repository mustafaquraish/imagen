#!/bin/bash

docker run --rm -v $(pwd):/mnt -w /mnt emscripten/emsdk make ems
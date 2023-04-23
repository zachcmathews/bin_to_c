#!/bin/bash

cmake -B build

pushd build
make
popd

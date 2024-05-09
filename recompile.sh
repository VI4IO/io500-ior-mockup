#!/bin/bash
pushd ior-mockup/src/
make clean
make
popd


pushd pfind-mockup/src/
make clean
make
popd

pushd io500
make clean
make
popd

#!/bin/bash
git clone git@github.com:IO500/io500.git
pushd io500
./prepare.sh
rm -rf build/ior
rm -rf build/pfind
ln -s $PWD/../ior-mockup build/ior 
ln -s $PWD/../pfind-mockup build/pfind 
popd


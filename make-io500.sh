#!/bin/bash
pushd io500
make
popd

echo "Running"

if [ ! -d "results" ]; then
  mkdir results
fi

if [ -f ./io500/io500 ]; then
  ./io500/io500 config-minimal.ini
fi

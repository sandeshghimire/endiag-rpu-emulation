#!/bin/bash

git clone https://github.com/dvidelabs/flatcc.git /tmp/flatcc
cd /tmp/flatcc
# scripts/initbuild.sh ninja
scripts/initbuild.sh make
scripts/setup.sh -a ../mymonster
ls bin
ls lib
cd ../mymonster
ls src
scripts/build.sh
ls generate

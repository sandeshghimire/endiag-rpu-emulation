#!/bin/bash

#set -o errexit
me=$(basename $0)

DRIVERS=drivers

_gitRoot() {
    git rev-parse --show-toplevel
}

# set project dir
PRJ_ROOT_DIR=$(_gitRoot)
cd $PRJ_ROOT_DIR

if [ -d $PRJ_ROOT_DIR/install ]; then
    rm -f -r $PRJ_ROOT_DIR/install
fi
mkdir -p $PRJ_ROOT_DIR/install

cd $DRIVERS
make
make install
cd $PRJ_ROOT_DIR/apps/src/message
./build.sh
cd $PRJ_ROOT_DIR/apps

mkdir -p build
cd build
cmake ..
make DESTDIR=$PRJ_ROOT_DIR/install install

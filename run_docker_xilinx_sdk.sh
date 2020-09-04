#!/bin/bash

echo "Running docker "
PRJ_DIR=$(pwd)
docker run -ti --rm \
    --net="host" -u $(id -u $USER) \
    -v $PRJ_DIR:/home/vivado/project:Z \
    petalinux-rpu-test:2019.1-latest /bin/bash

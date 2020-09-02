#!/bin/bash

PRJ=$(pwd)
INSTALLER=petalinux-v2019.1-final-installer.run
IMAGE=petalinux-rpu-test:2019.1-latest
docker build -t ${IMAGE} .

#!/bin/bash

_gitRoot() {
    git rev-parse --show-toplevel
}

HOME=$(_gitRoot)
cd protocol_buffer
./install_flatbuffers.sh
./install_flatcc.sh

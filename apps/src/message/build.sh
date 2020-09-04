#!/bin/sh
cd $(dirname $0)

FLATBUFFERS_DIR=../../../protocol_buffer/flatcc/
NAME=myissue
SCHEMA=rpu_apu_msg.fbs
OUT=build

FLATCC_EXE=$FLATBUFFERS_DIR/bin/flatcc
FLATCC_INCLUDE=$FLATBUFFERS_DIR/include
FLATCC_LIB=$FLATBUFFERS_DIR/lib

mkdir -p $OUT
$FLATCC_EXE --outfile $OUT/${NAME}_generated.h -a $SCHEMA || exit 1
cc -I$FLATCC_INCLUDE -g -o $OUT/$NAME $NAME.c -L$FLATCC_LIB -lflatccrt || exit 1
echo "running $OUT/$NAME"
if $OUT/$NAME; then
    echo "success"
else
    echo "failed"
    exit 1
fi

#!/bin/sh
cd $(dirname $0)

FLATBUFFERS_DIR=../../../protocol_buffer/flatcc/
NAME=rpu_apu_msg
SCHEMA=rpu_apu_msg.fbs
OUT=build

FLATCC_EXE=$FLATBUFFERS_DIR/bin/flatcc
FLATCC_INCLUDE=$FLATBUFFERS_DIR/include
FLATCC_LIB=$FLATBUFFERS_DIR/lib

rm -f -r *.h *.c
touch rpu_apu_msg_generated.c
$FLATCC_EXE --outfile ${NAME}_generated.h -a $SCHEMA || exit 1

#cc -I$FLATCC_INCLUDE -g -o $OUT/$NAME $NAME.c -L$FLATCC_LIB -lflatccrt || exit 1

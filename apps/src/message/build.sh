#!/bin/sh

FLATBUFFERS_DIR=../../../protocol_buffer/flatcc/
RPU_APU=rpu_apu_msg
APU_RPU=apu_rpu_msg

RPU_APU_SCHEMA=rpu_apu_msg.fbs
APU_RPU_SCHEMA=apu_rpu_msg.fbs

FLATCC_EXE=flatcc

rm -f -r *.h *.c
touch rpu_apu_msg_generated.c
touch apu_rpu_msg_generated.c

$FLATCC_EXE --outfile ${RPU_APU}_generated.h -a $RPU_APU_SCHEMA || exit 1
$FLATCC_EXE --outfile ${APU_RPU}_generated.h -a $APU_RPU_SCHEMA || exit 1

#cc -I$FLATCC_INCLUDE -g -o $OUT/$RPU_APU $RPU_APU.c -L$FLATCC_LIB -lflatccrt || exit 1

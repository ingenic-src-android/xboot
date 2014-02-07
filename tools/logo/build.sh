#!/bin/sh
export LOCAL_WORK_PATH=$PWD 
#echo $LOCAL_WORK_PATH
gcc -Wall -pedantic -idirafter $LOCAL_WORK_PATH/include -idirafter $LOCAL_WORK_PATH/include2 -idirafter $LOCAL_WORK_PATH/include -DTEXT_BASE=0x80100000 -DUSE_HOSTCC -O -c -o bmp_logo.o bmp_logo.c || exit 1

gcc -Wall -pedantic -idirafter $LOCAL_WORK_PATH/include -idirafter $LOCAL_WORK_PATH/include2 -idirafter $LOCAL_WORK_PATH/include -DTEXT_BASE=0x80100000 -DUSE_HOSTCC -O  -o bmp_logo bmp_logo.o || exit 1
strip bmp_logo || exit 1

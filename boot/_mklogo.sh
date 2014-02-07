#!/bin/sh
FILE="$1"

echo "Padding logo for ${FILE}"
dd bs=1024 count=800 if=/dev/zero of=junk1
cat  x-boot-tmp.bin junk1 > junk2
rm x-boot-tmp.bin
dd bs=1024 count=800 if=junk2 of=junk3
cat  junk3 ./logo/${FILE} > x-boot-tmp.bin
rm -f junk*

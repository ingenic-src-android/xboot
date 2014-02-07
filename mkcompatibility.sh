#!/bin/sh

xconfig_jz4760b=$1
xconfig_jz4760=$2

if [ $# -eq 2 ]; then
	make clean;
	make $xconfig_jz4760b
	make
	mv mbr-xboot.bin mbr-xboot.bin.60b
	make clean;
	make $xconfig_jz4760
	make
	gcc spl/tools/add_header/add_header.c -o spl/tools/add_header/add_header
	spl/tools/add_header/add_header mbr-xboot.bin mbr-xboot.bin.60b mbr-xboot.bin.compatibility
	echo ""
	echo ""
	echo ""
	echo "create mbr-xboot.bin.compatibility"
	cp mbr-xboot.bin mbr-xboot.bin.60
       	cp mbr-xboot.bin.compatibility mbr-xboot.bin
else
    echo "Usage:"
    echo "    $0 4760b_xconfig 4760_xconfig"
fi

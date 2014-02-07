#!/bin/sh

CPU="$1"

if [ ${CPU} = "JZ4750" ] || [ ${CPU} = "JZ4750L" ]; then
    echo "Padding nand-spl for ${CPU}"
    dd bs=1024 count=8 if=/dev/zero of=junk1
    cat nand-spl.bin junk1 > junk2
    dd bs=1024 count=8 if=junk2 of=junk-8k
    cat junk-8k junk-8k > junk-16k
    cat junk-16k junk-16k > junk-32k
    cp junk-32k nand-spl-pad.bin
    rm -f junk*
else
	if [ ${CPU} = "JZ4780" ]; then
		echo "Padding nand-spl for ${CPU}"
		dd bs=1024 count=16 if=/dev/zero of=junk1
		cat  nand-spl.bin junk1 > junk2
		dd bs=1024 count=16 if=junk2 of=nand-spl-pad.bin
		rm -f junk*
	elif [ ${CPU} = "JZ4775" ]; then
		echo "Padding nand-spl for ${CPU}"
		dd bs=1024 count=16 if=/dev/zero of=junk1
		cat  nand-spl.bin junk1 > junk2
		dd bs=1024 count=16 if=junk2 of=nand-spl-pad.bin
		rm -f junk*
	else
		echo "Padding nand-spl for ${CPU}"
		dd bs=1024 count=8 if=/dev/zero of=junk1
		cat  nand-spl.bin junk1 > junk2
		dd bs=1024 count=8 if=junk2 of=nand-spl-pad.bin
		rm -f junk*
	fi

fi

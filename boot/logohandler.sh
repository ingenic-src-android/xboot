#!/bin/sh

count=6;
for name in `sed -ne "s/.* \(.*\.rle\)/\1/p" boot/mklogo.sh`
do
	# echo ${count}
	# echo ${name}
	./boot/mkxbootimg --image boot/logo/${name} --addto $1 -o $2 --type ${count}
	# echo "./boot/mkxbootimg --image boot/logo/${name} --addto $1 -o $2 --type ${count}"
	count=$((count+1))
done
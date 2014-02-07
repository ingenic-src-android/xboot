./sign_boot x-boot3.bin -o x-boot3_digi_signature.bin
./mkxbootimg --image x-boot3_digi_signature.bin --addto x-boot3.bin -o x-boot3.bin --type 4
./pack mbr-xboot.bin x-boot3.bin -o boot.bin


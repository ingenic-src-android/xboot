##USE bmplogo to prepare battery charge logo.

Prepare 1.bmp~6.bmp reverse bmp pictures.

Make charge_logo.h
./bmp_logo 1.bmp 2.bmp 3.bmp 4.bmp 5.bmp 6.bmp > charge_logo.h
replace include/charge_logo.h with new charge_logo.h
Recompile x-boot.

If want modify bmp_logo.c. Use ./build.sh to recompile bmp_logo.


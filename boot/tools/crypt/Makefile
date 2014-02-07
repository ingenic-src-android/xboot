LIBTOMCRYPT = ./libtomcrypt/libtomcrypt.a
TOMSFASTMATH = ./tomsfastmath/libtfm.a

CRYPTO_LIBS = $(LIBTOMCRYPT) $(TOMSFASTMATH)
LIBS += $(CRYPTO_LIBS) ./common.o

CFLAGS = -I ../../../include/ -I ./tomsfastmath/src/headers/ -I ./libtomcrypt/src/headers/ -g -DUSE_TFM

all: common.o $(CRYPTO_LIBS)
	gcc $(CFLAGS) genkey.c -Wl,--start-group $(LIBS) -Wl,--end-group -o genkey
	gcc $(CFLAGS) sign_kernel.c -Wl,--start-group $(LIBS) -Wl,--end-group -o sign_kernel
	gcc $(CFLAGS) extract_publickey.c -Wl,--start-group $(LIBS) -Wl,--end-group -o extract_publickey
	gcc $(CFLAGS) sign_boot.c -Wl,--start-group $(LIBS) -Wl,--end-group -o sign_boot

common.o: common.c common.h $(CRYPTO_LIBS)
	gcc -c -o common.o -O2 $(CFLAGS) common.c
	

#CFLAGS="-g -DTFM_DESC -DUSE_TFM -I../tomsfastmath/src/headers/" make -f makefile.linux -j4

$(LIBTOMCRYPT): $(TOMSFASTMATH)
	rm -rf libtomcrypt
	cp -a ../../libtomcrypt .
	cd libtomcrypt; make -f makefile.linux clean; CFLAGS="-g -DTFM_DESC -DUSE_TFM -I../tomsfastmath/src/headers/"  make -f makefile.linux -j4

$(TOMSFASTMATH):
	rm -rf tomsfastmath
	cp -a ../../tomsfastmath .
	cd tomsfastmath; make -f makefile.linux clean; make -f makefile.linux -j4


clean:
	rm -rf genkey sign_kernel extract_publickey sign_boot libtomcrypt tomsfastmath *.o

distclean: clean
	rm -rf private.* public.*

.PHONY: clean distclean

#########################################################################

include $(TOPDIR)/config.mk

#########################################################################

LIB	= $(obj)libtomsfastmath.a

SOBJS   =
COBJS =src/addsub/fp_add.o src/addsub/fp_add_d.o src/addsub/fp_addmod.o src/addsub/fp_cmp.o \
src/addsub/fp_cmp_d.o src/addsub/fp_cmp_mag.o src/addsub/fp_sub.o src/addsub/fp_sub_d.o \
src/addsub/fp_submod.o src/addsub/s_fp_add.o src/addsub/s_fp_sub.o src/bin/fp_radix_size.o \
src/bin/fp_read_radix.o src/bin/fp_read_signed_bin.o src/bin/fp_read_unsigned_bin.o \
src/bin/fp_reverse.o src/bin/fp_s_rmap.o src/bin/fp_signed_bin_size.o src/bin/fp_to_signed_bin.o \
src/bin/fp_to_unsigned_bin.o src/bin/fp_toradix.o src/bin/fp_unsigned_bin_size.o src/bit/fp_cnt_lsb.o \
src/bit/fp_count_bits.o src/bit/fp_div_2.o src/bit/fp_div_2d.o src/bit/fp_lshd.o src/bit/fp_mod_2d.o \
src/bit/fp_rshd.o src/divide/fp_div.o src/divide/fp_div_d.o src/divide/fp_mod.o src/divide/fp_mod_d.o \
src/exptmod/fp_2expt.o src/exptmod/fp_exptmod.o src/misc/fp_ident.o src/misc/fp_set.o \
src/mont/fp_montgomery_calc_normalization.o src/mont/fp_montgomery_reduce.o \
src/mont/fp_montgomery_setup.o src/mul/fp_mul.o src/mul/fp_mul_2.o src/mul/fp_mul_2d.o \
src/mul/fp_mul_comba.o src/mul/fp_mul_comba_12.o src/mul/fp_mul_comba_17.o src/mul/fp_mul_comba_20.o \
src/mul/fp_mul_comba_24.o src/mul/fp_mul_comba_28.o src/mul/fp_mul_comba_3.o src/mul/fp_mul_comba_32.o \
src/mul/fp_mul_comba_4.o src/mul/fp_mul_comba_48.o src/mul/fp_mul_comba_6.o src/mul/fp_mul_comba_64.o \
src/mul/fp_mul_comba_7.o src/mul/fp_mul_comba_8.o src/mul/fp_mul_comba_9.o \
src/mul/fp_mul_comba_small_set.o src/mul/fp_mul_d.o src/mul/fp_mulmod.o src/numtheory/fp_gcd.o \
src/numtheory/fp_invmod.o src/numtheory/fp_isprime.o src/numtheory/fp_lcm.o \
src/numtheory/fp_prime_miller_rabin.o src/numtheory/fp_prime_random_ex.o src/sqr/fp_sqr.o \
src/sqr/fp_sqr_comba.o src/sqr/fp_sqr_comba_12.o src/sqr/fp_sqr_comba_17.o src/sqr/fp_sqr_comba_20.o \
src/sqr/fp_sqr_comba_24.o src/sqr/fp_sqr_comba_28.o src/sqr/fp_sqr_comba_3.o src/sqr/fp_sqr_comba_32.o \
src/sqr/fp_sqr_comba_4.o src/sqr/fp_sqr_comba_48.o src/sqr/fp_sqr_comba_6.o src/sqr/fp_sqr_comba_64.o \
src/sqr/fp_sqr_comba_7.o src/sqr/fp_sqr_comba_8.o src/sqr/fp_sqr_comba_9.o \
src/sqr/fp_sqr_comba_generic.o src/sqr/fp_sqr_comba_small_set.o src/sqr/fp_sqrmod.o 

CFLAGS += -Wall -W -Wshadow -Isrc/headers
CFLAGS += -O3 -funroll-loops -DTFM_MIPS

SRCS	:= $(SOBJS:.o=.S) $(COBJS:.o=.c)

OBJS	:= $(addprefix $(obj),$(SOBJS) $(COBJS))

all:	$(obj).depend $(LIB)

$(LIB):	$(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

#########################################################################

# defines $(obj).depend target
include $(TOPDIR)/rules.mk

sinclude $(obj).depend

#########################################################################

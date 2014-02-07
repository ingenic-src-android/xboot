#ifndef __UPERRORPARTITION_H_
#define __UPERRORPARTITION_H_

#define MUL_PARTS 4
enum part_attribute{
	PART_XBOOT,
	PART_KERNEL,
	PART_RECOVERY,
	PART_SYSTEM,
	PART_DATA,
	PART_MISC
};

enum operation_mode{
	ONE_PLANE,
	TWO_PLANES
};

/**
 * struct platform_nand_ex_partition
 * an element in platform_nand_partition
 * the member is as same as its
 */
struct platform_nand_ex_partition{
	long long offset;
	long long size;
	char *name;
};
/**
 * struct platform_nand_chip - chip level device structure
 * @name:		the name of this partition
 * @offset:	    offset within the master MTD space 
 * @size:	    partition size
 * @mode:		partition mode     0  DIRECT_MANAGER ; 1  ZONE_MANAGER 
 * @eccbit:		the number of eccbit  per partition 
 * @use_planes: operation mode     one_plane operation or two_plane operation
 * @part_attrib: partion attribute
 */
 
struct  platform_nand_partition{
	long long offset;
	long long size;
	const char *name;
	int mode;
	short eccbit;
	unsigned char use_planes;
	enum part_attribute part_attrib;
	struct platform_nand_ex_partition ex_partition[MUL_PARTS];
};

#endif

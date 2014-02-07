#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#include "tfm.h"

int fp_int_to_file(const char *filepath, void *fpint)
{
	printf("FP_SIZE %d\n", FP_SIZE);
	int fd = open(filepath, O_CREAT|O_APPEND|O_RDWR, 0666);
	if (fd < 0) {
		printf("open %s failed\n", filepath);
		return -1; 
	}   
	int ret = write(fd, fpint, sizeof(fp_int));
	if (ret < 0 || (unsigned long)ret < sizeof(fp_int)) {
		printf("write file failed\n");
		return -1; 
	}   
	close(fd);

	return 0;
}


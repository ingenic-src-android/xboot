#define MAX(tck, time)								\
({										\
	unsigned long value, temp;						\
	value = (tck * ps > time) ? (tck * ps) : time;				\
	value = (value % 1000 == 0) ? (value / 1000) : (value / 1000 + 1);	\
	value;									\
})

#define DDR_MAX(tck, time)								\
({										\
	unsigned long value;						\
	value = (tck * ps > time) ? (tck * ps) : time;				\
	ck = (value % ps == 0) ? 1 : 0;						\
	if (ck)									\
		value = tck;							\
	else									\
		value = (value % 1000 == 0) ? (value / 1000) : (value / 1000 + 1);	\
	value;									\
})
#define ADD(tck, time)								\
({										\
	unsigned long value;						\
	ck = (time % ps == 0) ? 1 : 0;						\
	if (ck) {									\
		value = time / ps + tck;					\
	} else {									\
		value = tck * ps + time;					\
		value = (value % 1000 == 0) ? (value / 1000) : (value / 1000 + 1);	\
	}									\
	value;									\
})

void uint2str(unsigned int pword, unsigned char* str)
{
	char alpha[] = "0123456789ABCDEF";
	unsigned int i;

	for (i = 0; i < 8; i++, pword <<= 4) {
		str[i] = (alpha[pword >> 28]);
	}
        str[i] = 0;
}

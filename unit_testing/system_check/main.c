#include <stdlib.h>
#include <stdint.h>

int main(void)
{
	/* check */
	/* size */
	uint32_t u32;
	float f;
	double d;
	
        if (sizeof(u32) != 4) /* something really wrong */
		return -1;

	if (sizeof(f) != 4)
		return -2;

	if (sizeof(d) != 8)
		return -3;
	
	/* little endian */
	u32 = 0x12345678;
	uint8_t *p = (uint8_t *) &u32;
	uint8_t little[4] = {0x78, 0x56, 0x34, 0x12};

	for (int i = 0; i < 4; ++i)
		if (little[i] != p[i])
			return -4;
	
	/* decimal number representations */
	f = 0.123456;
	u32 = 0x3dfcd680;	/* ieee 754 std */
	uint8_t *fp = (uint8_t *) &f;

	d = 0.123456789123;
	uint8_t *dp = (uint8_t *) &d;
	uint64_t u64 = 0x3fbf9add37c0a0cb;
	
	for (int i = 0; i < 4; ++i)
		if (fp[i] != p[i])
			return -5;

        p = (uint8_t *) &u64;
	for (int i = 0; i < 8; ++i)
		if (dp[i] != p[i])
			return -6;
	
	return 0;
}

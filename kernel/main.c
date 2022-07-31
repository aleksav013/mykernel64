#include <stdint.h>

int main(void)
{
	uint16_t* ct_buffer = (uint16_t*)0xb8000;
	ct_buffer[0] = 0x2f4c;
	ct_buffer[1] = 0x2f4f;
	ct_buffer[2] = 0x2f4e;
	ct_buffer[3] = 0x2f47;

	return 0;
}

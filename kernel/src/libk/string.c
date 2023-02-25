#include <types.h>
#include <libk/string.h>

size_t strlen(const char *s)
{
	if (s == NULL)
		return 0;

	char *p = (char *)s;
	while (*p)
		p++;
	return (size_t)(p - s);
}

uint64_t stoi(const char *s)
{
	uint64_t num = 0;
	size_t n = strlen(s);

	size_t i;
	for (i = 0; i < n; i++) {
		num *= 10;
		num += (uint64_t)(s[i] - '0');
	}

	return num;
}

void strrev(char *s)
{
	size_t n = strlen(s);
	char a[100];
	size_t i;
	for (i = 0; i < n; i++) {
		a[i] = s[n - 1 - i];
	}
	for (i = 0; i < n; i++) {
		s[i] = a[i];
	}
}

void itos(uint64_t num, char *s)
{
	if (num == 0) {
		s[0] = '0';
		s[1] = '\0';
		return;
	}

	size_t i;
	for (i = 0; num; num /= 10, i++) {
		s[i] = (char)('0' + num % 10);
	}
	s[i] = '\0';
	strrev(s);
}

void itoh(uint64_t num, char *s)
{
	if (num == 0) {
		s[0] = '0';
		s[1] = '\0';
		return;
	}

	size_t i;
	for (i = 0; num; num /= 16, i++) {
		if (num % 16 < 10)
			s[i] = (char)('0' + num % 16);
		else
			s[i] = (char)('A' + num % 16 - 10);
	}
	s[i] = '\0';
	strrev(s);
}

void memcpy(const void *destptr, const void *srcptr, size_t n)
{
	if (destptr == NULL || srcptr == NULL)
		return;

	uint8_t *dest = (uint8_t *)destptr;
	const uint8_t *src = (const uint8_t *)srcptr;

	size_t i;
	for (i = 0; i < n; i++) {
		dest[i] = src[i];
	}
}

void memset(const void *destptr, uint8_t value, size_t n)
{
	uint8_t *dest = (uint8_t *)destptr;
	size_t i;
	for (i = 0; i < n; i++) {
		dest[i] = value;
	}
}

int32_t memcmp(const void *aptr, const void *bptr)
{
	const uint8_t *a = (const uint8_t *)aptr;
	const uint8_t *b = (const uint8_t *)bptr;

	const size_t len_a = strlen((char *)a);
	const size_t len_b = strlen((char *)b);
	size_t i;
	for (i = 0; i < len_a && i < len_b; i++) {
		if (a[i] != b[i])
			return a[i] - b[i];
	}

	if (len_a < len_b) {
		return -1;
	}
	if (len_a > len_b) {
		return 1;
	}
	return 0;
}

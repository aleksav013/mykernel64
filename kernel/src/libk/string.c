#include <types.h>
#include <libk/string.h>

size_t strlen(const char* s)
{
	char *p = (char*)s;
	while (*p) p++;
	return (size_t)(p - s);
}

uint64_t stoi(const char *s)
{
	uint64_t num = 0;
	size_t n = strlen(s);

	for (size_t i = 0; i < n; i++) {
		num *= 10;
		num += (uint64_t)(s[i] - '0');
	}

	return num;
}

void strrev(char *s)
{
	size_t n = strlen(s);
	char a[100];
	for (size_t i = 0; i < n; i++) {
		a[i] = s[n - 1 - i];
	}
	for (size_t i = 0; i < n; i++) {
		s[i] = a[i];
	}
}

void itos(uint64_t num, char* s)
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

void itoh(uint64_t num, char* s)
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

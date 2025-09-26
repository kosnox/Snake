#include "terminal_impl_stdio.h"
#include <stdio.h>
#include <stdint.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>

void std_terminal_initialize()
{
	//setvbuf(stdin, NULL, _IONBF, 0);
	_setmode(_fileno(stdin), _O_BINARY);
}
int32_t std_terminal_read(void *arg, void *data, int32_t len)
{
	char *dst = data;
	int32_t read_count = 0;

	while (len--) {
		int val = _getch();
		if (val == EOF)
			break;
		read_count++;
		*dst = (char)val;
		dst++;
	}
	return read_count;
}

int32_t std_terminal_write(void *arg, const void *data, int32_t len)
{
	return _write(1, data, len);
}

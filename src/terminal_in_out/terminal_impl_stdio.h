#if !defined(TERMINAL_IMPL_STDIO_H)
#define TERMINAL_IMPL_STDIO_H
#if defined(__cplusplus)
extern "C" {
#endif

#include <stdint.h>
	void std_terminal_initialize();
	int32_t std_terminal_read(void *arg, void *data, int32_t len);
	int32_t std_terminal_write(void *arg, const void *data, int32_t len);

#if defined(__cplusplus)
}
#endif
#endif //


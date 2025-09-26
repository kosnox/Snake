#if !defined(TEMRINAL_H)
#define TERMINAL_H
#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>


typedef int32_t (*terminal_write_func)(void *func_arg, const void *data, int32_t len); ///< write function used by terminal
typedef int32_t (*terminal_read_func)(void *func_arg, void *data, int32_t len); ///< read function used by terminal

typedef struct {
	terminal_write_func wr_func;		///< function used to write data
	void*				wr_func_arg;	///< argument passed to write funcion
	terminal_read_func	rd_func;		///< function used by terminal to read data
	void*				rd_func_arg;	///< argument passed to read function
} terminal_init_t;

/**
 * @brief Possible color values
*/
typedef enum TerminalColor
{
	TC_BLACK = 0,
	TC_RED = 1,
	TC_GREEN = 2,
	TC_YELLOW =3,
	TC_BLUE = 4,
	TC_MAGENTA = 5,
	TC_CYAN = 6,
	TC_WHITE = 7,
	TC_DEFAULT = 9,
} eTerminalColor;

int32_t terminal_init(const terminal_init_t * config);
int32_t terminal_printf(const char *format, ...);
int32_t terminal_setCursorPos(int32_t col, int32_t row);
int32_t terminal_getCursorPos(int32_t *x, int32_t *y);
int32_t terminal_showCursor(bool visible);
int32_t terminal_setColor(eTerminalColor fg, eTerminalColor bg);
int32_t terminal_getKey();
int32_t terminal_resetStyle();
int32_t terminal_clearScreen();
#if defined(__cplusplus)
}
#endif

#endif // TERMINAL_H

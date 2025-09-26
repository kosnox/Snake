#include "terminal.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define TERM_INIT_80_25_COLOR			"\x1b[=3h"
#define TERM_RESET_COLORS				"\x1b[0;m"
#define TERMINAL_ERASE_SCREEN			"\x1b[2J"
#define TERMINAL_DSR_CURSOR_POSITION	"\x1b[6n"
#define TERMINAL_HIDE_CURSOR			"\x1b[?25l"
#define TEMRINAL_SHOW_CURSOR			"\x1b[?25h"

/*
* REPORTS
*/

static terminal_init_t terminal_configuration;

#define TXT_BUF_LEN 100
static char text_buffer[TXT_BUF_LEN];


static int8_t terminal_read_internal(char *dst, int32_t len);
static int8_t terminal_write(const char *msg);
/***
 * @brief Initialize terminal with write and read functions
 * @param config - pointer to configuration structure
 * @return -1 - initialization error
 * 0 - initialization success
*/
int32_t terminal_init(const terminal_init_t *config)
{
	if (!config)
		return -1;
	terminal_configuration = *config;

	terminal_write(TERM_INIT_80_25_COLOR);
	terminal_resetStyle();
	return 0;
}

int8_t terminal_write(const char *msg)
{
	if (!terminal_configuration.wr_func)
		return -1;
	int32_t len = strlen(msg);
	return terminal_configuration.wr_func(terminal_configuration.wr_func_arg, msg, len);
}

/**
 * @brief Read len characters from terminal, save in dst buffer
 * @param dst - destination buffer
 * @param len - number of bytes to read
 * @return negative on failure, nonnegative with number of bytes read
*/
int8_t terminal_read_internal(char *dst, int32_t len)
{
	if (!terminal_configuration.rd_func)
		return -1;
	return terminal_configuration.rd_func(terminal_configuration.rd_func_arg, dst, len);
}

/**
 * @brief Print formatted text to terminal.
 * This function is a wrapper over sprintf fuction.
 * @note maximum output length is limited by \ref TXT_BUF_LEN
 *
 * @param format - format text specification
 * @param ... - arguments for format specification
 * @return 
*/
int32_t terminal_printf(const char *format, ...)
{
	va_list args;
	va_start(args, format);
	vsprintf_s(text_buffer, TXT_BUF_LEN, format, args);
	va_end(args);
	terminal_write(text_buffer);
	return 0;
}

/**
 * @brief Set cursor position.
 * @param col - column number (1-based)
 * @param row - row number (1-based)
 * @return 0 - success, -1 - fail
*/
int32_t terminal_setCursorPos(int32_t col, int32_t row)
{
	sprintf_s(text_buffer, TXT_BUF_LEN, "\x1b[%d;%dH", row, col);
	return terminal_write(text_buffer);
}

/**
 * @brief Read current cursor coordinates
 * @param pCol - pointer to destination where 1-based column number is stored
 * @param pRow - pointer to destination where 1-based row numer is stored
 * @return 0 - success, row and column parameters written, -1 - fail, row and column parameters not modified
*/
int32_t terminal_getCursorPos(int32_t *pCol, int32_t *pRow)
{
	int32_t col = 0, row = 0;
	terminal_write(TERMINAL_DSR_CURSOR_POSITION);
	int32_t idx = 0;
	// read data from terminal
	// expected sequence: ESC [ <r> ; <c> R
	// where <r> is row number and <c> is column number
	while (idx < TXT_BUF_LEN) {
		char c = '\0';
		terminal_read_internal(&c, 1);
		text_buffer[idx++] = c;
		if (c == 'R')
			break;
	}
	if (text_buffer[0] != '\x1b')
		return -1;

	bool parse_row = false, parse_column = false;
	for (int i = 0; i < idx; i++) {
		if (text_buffer[i] == '[') {
			parse_row = true;
			continue;
		}
		else if (text_buffer[i] == ';') {
			parse_row = false;
			parse_column = true;
			continue;
		}
		else if (text_buffer[i] == 'R') {
			parse_column = false;
		}
		if (text_buffer[i] >= '0' && text_buffer[i] <= '9') {
			int val = text_buffer[i] - '0';
			if (parse_row) {
				row = row * 10 + val;
			}
			if (parse_column) {
				col = col * 10 + val;
			}
		}
	}
	int colon_pos = -1;

	for (int i = 0; i < TXT_BUF_LEN; i++) {
		if (text_buffer[i] == ';') {
			colon_pos = i;
			break;
		}
	}
	if (colon_pos < 0)
		return -1;
	if (!pCol || !pRow)
		return -1;
	*pCol = col;
	*pRow = row;
	return 0;
}

/**
 * @brief Set cursor visibility
 * @param visible - true to show cursor, false to hide cursor
 * @return 0 - success, -1 - error
*/
int32_t terminal_showCursor(bool visible)
{
	if (visible)
		return terminal_write(TEMRINAL_SHOW_CURSOR);
	else
		return terminal_write(TERMINAL_HIDE_CURSOR);
}

/**
 * @brief Set foreground and background colors
 * 
 * @param fg - foreground color value
 * @param bg - background color value
 * 
 * @return 0 - success, -1 - error
*/
int32_t terminal_setColor(eTerminalColor fg, eTerminalColor bg)
{
	int colFg = 30 + (int)fg;
	int colBg = 40 + (int)bg;
	sprintf_s(text_buffer, TXT_BUF_LEN, "\x1b[%d;%dm", colFg, colBg);
	return terminal_write(text_buffer);
}

/**
* read keypress from terminal
*/
int32_t terminal_getKey()
{
	char c;
	terminal_read_internal(&c, 1);
	if (c != '\x1b')
		return c;

	return 0;
}

/**
 * @brief Reset terminal visual style.
 * @return 0 - success, -1 - failure
*/
int32_t terminal_resetStyle()
{
	return terminal_write(TERM_RESET_COLORS);
}

/**
 * @brief Send a 'clear screen' command to terminal
 * @return 0 - success, -1 - failure
*/
int32_t terminal_clearScreen()
{
	return terminal_write(TERMINAL_ERASE_SCREEN);
}

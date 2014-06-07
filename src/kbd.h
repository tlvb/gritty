#ifndef __KBD_H__
#define __KBD_H__
#include <linux/input.h>
#include <stdbool.h>
#include <stdint.h>

#define REQ32(x) (((x)+0x1f)>>5)
#define MAP_MODSTATE_CNT 2
#define MAP_KEY_CNT 59

#define MKS_LEFTSHIFT 1
#define MKS_RIGHTSHIFT 2
#define MKM_SHIFT 3
#define MS_SHIFT 1

// kind of a simplistic and incomplete mapping
// but expandable should I need to support more keys
// and/or modstates...
#define DVORAK_WITH_ESC { /*{{{*/ \
	{0, 0},				/* RESERVED		0 */ \
	{'\x1b', '\x1b'},	/* ESC			1 */ \
	{'1', '!'},			/* 1			2 */ \
	{'2', '@'},			/* 2			3 */ \
	{'3', '#'},			/* 3			4 */ \
	{'4', '$'},			/* 4			5 */ \
	{'5', '%'},			/* 5			6 */ \
	{'6', '^'},			/* 6			7 */ \
	{'7', '&'},			/* 7			8 */ \
	{'8', '*'},			/* 8			9 */ \
	{'9', '('},			/* 9			10 */ \
	{'0', ')'},			/* 0			11 */ \
	{'[', '{'},			/* MINUS		12 */ \
	{']', '}'},			/* EQUAL		13 */ \
	{'\x08', '\x08'},	/* BACKSPACE		14 */ \
	{'\x09', '\x09'},	/* TAB			15 */ \
	{'\'', '"'},		/* Q			16 */ \
	{',', '<'},			/* W			17 */ \
	{'.', '>'},			/* E			18 */ \
	{'p', 'P'},			/* R			19 */ \
	{'y', 'Y'},			/* T			20 */ \
	{'f', 'F'},			/* Y			21 */ \
	{'g', 'G'},			/* U			22 */ \
	{'c', 'C'},			/* I			23 */ \
	{'r', 'R'},			/* O			24 */ \
	{'l', 'L'},			/* P			25 */ \
	{'/', '?'},			/* LEFTBRACE		26 */ \
	{'=', '+'},			/* RIGHTBRACE		27 */ \
	{'\x0a', '\x0a'},	/* ENTER		28 */ \
	{0, 0},				/* LEFTCTRL		29 */ \
	{'a', 'A'},			/* A			30 */ \
	{'o', 'O'},			/* S			31 */ \
	{'e', 'E'},			/* D			32 */ \
	{'u', 'U'},			/* F			33 */ \
	{'i', 'I'},			/* G			34 */ \
	{'d', 'D'},			/* H			35 */ \
	{'h', 'H'},			/* J			36 */ \
	{'t', 'T'},			/* K			37 */ \
	{'n', 'N'},			/* L			38 */ \
	{'s', 'S'},			/* SEMICOLON		39 */ \
	{'-', '_'},			/* APOSTROPHE		40 */ \
	{'`', '~'},			/* GRAVE		41 */ \
	{0, 0},				/* LEFTSHIFT		42 */ \
	{'\\', '|'},		/* BACKSLASH		43 */ \
	{';', ':'},			/* Z			44 */ \
	{'q', 'q'},			/* X			45 */ \
	{'j', 'j'},			/* C			46 */ \
	{'k', 'k'},			/* V			47 */ \
	{'x', 'x'},			/* B			48 */ \
	{'b', 'b'},			/* N			49 */ \
	{'m', 'm'},			/* M			50 */ \
	{'w', 'w'},			/* COMMA		51 */ \
	{'v', 'v'},			/* DOT			52 */ \
	{'z', 'z'},			/* SLASH		53 */ \
	{0, 0},				/* RIGHTSHIFT		54 */ \
	{'*', '*'},			/* KPASTERISK		55 */ \
	{0, 0},				/* LEFTALT		56 */ \
	{' ', ' '},			/* SPACE		57 */ \
	{'\x1b', '\x1b'}	/* CAPSLOCK		58 */ \
} /*}}}*/
#define QWERTY { /*{{{*/ \
	{0, 0},				/* RESERVED		0 */ \
	{'\x1b', '\x1b'},	/* ESC			1 */ \
	{'1', '!'},			/* 1			2 */ \
	{'2', '@'},			/* 2			3 */ \
	{'3', '#'},			/* 3			4 */ \
	{'4', '$'},			/* 4			5 */ \
	{'5', '%'},			/* 5			6 */ \
	{'6', '^'},			/* 6			7 */ \
	{'7', '&'},			/* 7			8 */ \
	{'8', '*'},			/* 8			9 */ \
	{'9', '('},			/* 9			10 */ \
	{'0', ')'},			/* 0			11 */ \
	{'-', '_'},			/* MINUS		12 */ \
	{'=', '+'},			/* EQUAL		13 */ \
	{'\x08', '\x08'},	/* BACKSPACE		14 */ \
	{'\x09', '\x09'},	/* TAB			15 */ \
	{'q', 'Q'},			/* Q			16 */ \
	{'w', 'W'},			/* W			17 */ \
	{'e', 'E'},			/* E			18 */ \
	{'r', 'R'},			/* R			19 */ \
	{'t', 'T'},			/* T			20 */ \
	{'y', 'Y'},			/* Y			21 */ \
	{'u', 'U'},			/* U			22 */ \
	{'i', 'I'},			/* I			23 */ \
	{'o', 'O'},			/* O			24 */ \
	{'p', 'P'},			/* P			25 */ \
	{'[', '{'},			/* LEFTBRACE		26 */ \
	{']', '}'},			/* RIGHTBRACE		27 */ \
	{'\x0a', '\x0a'},	/* ENTER		28 */ \
	{0, 0},				/* LEFTCTRL		29 */ \
	{'a', 'A'},			/* A			30 */ \
	{'s', 'S'},			/* S			31 */ \
	{'d', 'D'},			/* D			32 */ \
	{'f', 'F'},			/* F			33 */ \
	{'g', 'G'},			/* G			34 */ \
	{'h', 'H'},			/* H			35 */ \
	{'j', 'J'},			/* J			36 */ \
	{'k', 'K'},			/* K			37 */ \
	{'l', 'L'},			/* L			38 */ \
	{';', ':'},			/* SEMICOLON		39 */ \
	{'\'', '"'},		/* APOSTROPHE		40 */ \
	{'`', '~'},			/* GRAVE		41 */ \
	{0, 0},				/* LEFTSHIFT		42 */ \
	{'\\', '|'},		/* BACKSLASH		43 */ \
	{'z', 'Z'},			/* Z			44 */ \
	{'x', 'X'},			/* X			45 */ \
	{'c', 'C'},			/* C			46 */ \
	{'v', 'V'},			/* V			47 */ \
	{'b', 'B'},			/* B			48 */ \
	{'n', 'N'},			/* N			49 */ \
	{'m', 'M'},			/* M			50 */ \
	{',', '<'},			/* COMMA		51 */ \
	{'.', '>'},			/* DOT			52 */ \
	{'/', '?'},			/* SLASH		53 */ \
	{0, 0},				/* RIGHTSHIFT		54 */ \
	{'*', '*'},			/* KPASTERISK		55 */ \
	{0, 0},				/* LEFTALT		56 */ \
	{' ', ' '},			/* SPACE		57 */ \
	{0, 0}				/* CAPSLOCK		58 */ \
} /*}}}*/

typedef struct {
	int fd;
	uint16_t modkeystate;
	uint8_t modstate;
	uint8_t keymap[MAP_KEY_CNT][MAP_MODSTATE_CNT];
} kbd_t;

char *find_keyboard(void);
int read_and_translate(uint8_t *seq, int n, kbd_t *k);



#endif

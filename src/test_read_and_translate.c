#include "kbd.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

int main(void) {
	char *devpath = find_keyboard();
	if (devpath != NULL) {
		printf("keyboard located at '%s'\n", devpath);
	}
	else {
		fprintf(stderr, "no keyboard found\n");
		return 1;
	}
	int fd = open(devpath, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "could not open devfile\n");
		return 1;
	}

	kbd_t kbd = { .fd = fd, .keymap = DVORAK_WITH_ESC };
	while (true) {
		read_and_translate(NULL, 0, &kbd);
	}
}


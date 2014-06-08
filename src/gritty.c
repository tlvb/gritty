#include "kbd.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *const *argv) {
	int ci = -1;
	for (int i=1; i<argc; ++i) {
		if (strcmp("--", argv[i]) == 0) {
			ci = i+1;
			break;
		}
	}
	if (ci == -1) {
		printf("ERROR: NO PROGRAM TO CALL\n");
		for(;;);
	}


	char *devpath = NULL;
	for (;;) {
		devpath = find_keyboard();
		if (devpath != NULL) {
			printf("keyboard located at '%s'\n", devpath);
			break;
		}
		else {
			fprintf(stderr, "no keyboard found, trying again in ten seconds...\n");
			sleep(10);
		}
	}
	int fd = open(devpath, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "could not open devfile\n");
		return 1;
	}

	kbd_t kbd = { .fd = fd, .keymap = DVORAK_WITH_ESC };

	int p[2];
	pipe(p);

	int f = fork();
	if (f > 0) {
		// child
		close(0); // close stdin
		dup2(p[0], 0); // stdin is now one end of the pipe
		close(p[0]);

		execvp(argv[ci], argv+ci); // run program
	}
	else if (f == 0) {
		// parent
		for (;;) {
			int kv = read_and_translate(NULL, 0, &kbd);
			write(p[1], &kv, sizeof(int));
		}
	}
	else {
		printf("ERROR: FORK FAILED\n");
	}

}


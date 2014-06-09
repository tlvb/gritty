#define _XOPEN_SOURCE 700
#include "kbd.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>
#include <sys/wait.h>

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

	// look for keyboard device
	/*{{{*/
	char *devpath = NULL;
	fputs("searching for keyboard...", stderr);
	for (;;) {
		devpath = find_keyboard();
		if (devpath != NULL) {
			fprintf(stderr, "\nkeyboard located at '%s'\n", devpath);
			break;
		}
		else {
			sleep(2);
			fputc('.', stderr);
		}
	}
	int fd = open(devpath, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "could not open devfile\n");
		return 1;
	}
	/*}}}*/

	kbd_t kbd = { .fd = fd, .keymap = DVORAK_WITH_ESC };

	int ret = -1;
	int ptm = posix_openpt(O_RDWR);
	ret = grantpt(ptm);
	ret = unlockpt(ptm);
	int pts = open(ptsname(ptm), O_RDWR);
	char buf[128];

	pid_t pid = fork();
	if (pid == 0) { /*{{{*/
		// parent
		fd_set readset;
		int nfds = -1;
		if (nfds <= kbd.fd) { nfds = kbd.fd+1; }
		if (nfds <= ptm) { nfds = ptm+1; }

		for (;;) {
			FD_ZERO(&readset);
			FD_SET(kbd.fd, &readset);
			FD_SET(ptm, &readset);
			select(nfds, &readset, NULL, NULL, NULL);
			if (FD_ISSET(kbd.fd, &readset)) {
				char kv = (char)read_and_translate(NULL, 0, &kbd);
				if (kv != 0) {
					int n = write(ptm, &kv, sizeof(char));
					if (n <= 0) {
						exit(20);
					}
				}
			}
			if (FD_ISSET(ptm, &readset)) {
				ssize_t n = read(ptm, buf, sizeof(buf));
				if (n < 0) {
					//oh shit!
					exit(30);
				}
				write(2, buf, n);
			}
		}
	} /*}}}*/
	else if (pid > 0) { /*{{{*/
		close(0);
		close(1);
		close(2);
		dup(pts);
		dup(pts);
		dup(pts);
		setsid();
		ioctl(0, TIOCSCTTY, 1);

		for (;;) {
			pid_t pid2 = fork();
			if (pid2 == 0) {
				// child
				printf("\n\nSPAWNING\n");
				execvp(argv[ci], argv+ci); // run program
				printf("ERROR: SPAWNING OF '%s' FAILED\n", argv[ci]);
				perror(NULL);
				sleep(9);
			}
			else if (pid2 > 0) {
				waitpid(pid2, NULL, 0);
			}
			else {
				printf("ERROR: SECOND FORK FAILED\n");
				exit(40);
			}
			sleep(1);
		}
	} /*}}}*/
	else {
		printf("ERROR: FIRST FORK FAILED\n");
	}

}


#include <dirent.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "kbd.h"

#include <stdio.h>

uint32_t test_bit32(uint32_t *seq, unsigned bit) { /*{{{*/
	return seq[bit>>5] & (1<<(bit & 0x1f));
} /*}}}*/
bool check_if_keyboard(const char *fn) { /*{{{*/
	int fd = open(fn, O_RDONLY);
	if (fd < 0) {
		goto nocigar;
	}

	uint32_t evbits[REQ32(EV_MAX)];
	if (ioctl(fd, EVIOCGBIT(0, sizeof(evbits)), evbits) < 0) {
		goto nocigar;
	}

	if (!test_bit32(evbits, EV_KEY)) {
		goto nocigar;
	}

	uint32_t keybits[REQ32(KEY_MAX)];
	if (ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keybits)), keybits) < 0) {
		goto nocigar;
	}

	for (unsigned k=KEY_ESC; k<=KEY_SPACE; ++k) {
		if (!test_bit32(keybits, k)) {
			goto nocigar;
		}
	}

	close(fd);
	return true;

nocigar:
	// but
	close(fd);
	return false;
} /*}}}*/
char *find_keyboard(void) { /*{{{*/
	DIR *dh = opendir("/dev/input");
	struct dirent *de;
	static char efn[64];
	while ((de = readdir(dh)) != NULL) {
		if (strncmp(de->d_name, "event", 5) == 0) {
			snprintf(efn, 64, "/dev/input/%s", de->d_name);
			if (check_if_keyboard(efn)) {
				closedir(dh);
				return efn;
			}
		}
	}
	closedir(dh);
	return NULL;
} /*}}}*/
int read_and_translate(uint8_t *seq, int n, kbd_t *kbd) { /*{{{*/
	struct input_event ie;
	read(kbd->fd, &ie, sizeof(ie));

	if (ie.type == EV_KEY) {
		if (ie.code < MAP_KEY_CNT) {
			uint8_t kv = kbd->keymap[ie.code][kbd->modstate];
			if (kv != 0) {
				switch (ie.value) {
					case 0: // release
						break;
					case 1: // press
						return kv;
						break;
					case 2: // autorepeat
						// possibly add some kind of runtime option
						// to turn on/off autorepeat?
						return kv;
						break;
				}
			}
			else {
				switch (ie.code) {
					case KEY_LEFTSHIFT:
						switch (ie.value) {
							case 0: // release
								kbd->modkeystate &= ~MKS_LEFTSHIFT;
								if (!(kbd->modkeystate & MKM_SHIFT)) {
									kbd->modstate &= ~MS_SHIFT;
								}
								break;
							case 1: // press
								kbd->modkeystate |= MKS_LEFTSHIFT;
								kbd->modstate |= MS_SHIFT;
						}
						break;
					case KEY_RIGHTSHIFT:
						switch (ie.value) {
							case 0: // release
								kbd->modkeystate &= ~MKS_RIGHTSHIFT;
								if (!(kbd->modkeystate & MKM_SHIFT)) {
									kbd->modstate &= ~MS_SHIFT;
								}
								break;
							case 1: // press
								kbd->modkeystate |= MKS_RIGHTSHIFT;
								kbd->modstate |= MS_SHIFT;
						}
						break;
				}
			}
		}
	}
	else {
		//printf("type %d is not key\n", ie.type);
	}
	return 0;
} /*}}}*/

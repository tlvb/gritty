#include <linux/input.h>
#include <dirent.h>
#include <stdint.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

#define REQ32(x) (((x)+0x1f)>>5)

uint32_t test_bit32(uint32_t *seq, unsigned bit) {
	return seq[bit>>5] & (1<<(bit & 0x1f));
}

bool check_if_keyboard(const char *fn) {
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
}

char *find_keyboard(void) {
	DIR *dh = opendir("/dev/input");
	struct dirent *de;
	static char efn[64];
	while ((de = readdir(dh)) != NULL) {
		if (strncmp(de->d_name, "event", 5) == 0) {
			snprintf(efn, 64, "/dev/input/%s", de->d_name);
			printf("checking '%s'\n", efn);
			if (check_if_keyboard(efn)) {
				closedir(dh);
				return efn;
			}
		}
	}
}

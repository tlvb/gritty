#include "findkbd.h"
#include <stdio.h>
#include <stdlib.h>
int main(void) {
	char *devpath = find_keyboard();
	if (devpath != NULL) {
		printf("keyboard located at '%s'\n", devpath);
	}
	else {
		printf("no keyboard found\n");
	}
}

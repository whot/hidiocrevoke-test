/*  SPDX-License-Identifier: MIT
 *
 * Test program for the HIDIOCREVOKE ioctl
 * Usage:
 *
 * $ ./revoke /dev/hidraw1
 *
 */
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
//#include <linux/hidraw.h>
#include "hidraw.h"
#include <sys/ioctl.h>

int main(int argc, char **argv) {
	const char *path = argc > 1 ? argv[1] : "/dev/hidraw0";

	setbuf(stdout, NULL);

	printf("Device is %s\n", path);
	printf("Opening  ... ");

	int fd = open(path, O_RDWR|O_NONBLOCK);
	if (fd < 0) {
		perror("");
		return 1;
	}

	printf("success\n");

	/* Reading from the device should work fine */

	printf("Reading  ... ");

	char buf[64] = {0};
	int len = read(fd, buf, sizeof(buf));
	if (len == -1) {
		perror("error");
		if (errno != EAGAIN)
			return -1;
	} else {
		printf("success. %d bytes received\n", len > 0 ? len : 0);
	}
	printf("Revoking ... ");

	int status = ioctl(fd, HIDIOCREVOKE, NULL);
	if (status < 0) {
		perror("error");
		return 1;
	}

	printf("success\n");

	printf("Reading  ... ");
	int rc = read(fd, NULL, 0);
	if (rc < 0) {
		perror("error");
		if (errno == EINVAL) {
			fprintf(stderr, "ERROR: this kernel does not support HIDIOCREVOKE\n");
		} else if (errno != ENODEV) {
			fprintf(stderr, "ERROR: expected ENODEV (%s) after revoke\n", strerror(ENODEV));
			return 1;
		}
	}

	printf("Closing  ... ");
	close(fd);
	printf("success\n");

	return 0;
}

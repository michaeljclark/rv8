#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	struct stat statbuf;
	char buf[1024];
	size_t bytes_read = 0;
	ssize_t rdlen;
	int fd;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <filename>\n", argv[0]);
		exit(9);
	}

	if (stat(argv[1], &statbuf) < 0) {
		fprintf(stderr, "error: stat: %s\n", strerror(errno));
		exit(9);
	}
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		fprintf(stderr, "error: open: %s\n", strerror(errno));
		exit(9);
	}

	do {
		if ((rdlen = read(fd, buf, sizeof(buf))) < 0) {
			fprintf(stderr, "error: read: %s\n", strerror(errno));
			exit(9);
		}
		bytes_read += rdlen;
	} while (rdlen > 0);

	if (statbuf.st_size != bytes_read) {
		fprintf(stderr, "error: stat length and file length differ\n");
		exit(9);
	}

	if (close(fd) < 0) {
		fprintf(stderr, "error: close: %s\n", strerror(errno));
		exit(9);
	}

	printf("read %zu bytes\n", bytes_read);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>

#include <mach-o/loader.h>
#include <mach-o/swap.h>
#include <mach-o/fat.h>


/* fixes x86-64 mach-o files with invalid zeropage virtual addresses */

#define PAGEZERO_SEGMENT_NAME "__PAGEZERO"

static void* read_data(FILE *file, size_t offset, size_t size)
{
	void *buf = malloc(size);
	fseek(file, offset, SEEK_SET);
	fread(buf, size, 1, file);
	return buf;
}

int main(int argc, char *argv[])
{
	FILE *file;
	uint32_t magic;
	struct mach_header_64 *header;
	struct load_command *cmd;
	struct segment_command_64 *segment;
	size_t offset, ncmds;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <macho-file>\n", argv[0]);
		exit(1);
	}

	if ((file = fopen(argv[1], "r+")) == NULL) {
		fprintf(stderr, "can't open file: %s: %s\n", argv[1], strerror(errno));
		exit(1);
	}

	if (fread(&magic, sizeof(uint32_t), 1, file) == 0 || magic != MH_MAGIC_64) {
		fprintf(stderr, "invalid magic: can only operate on x86_64 mach-o executables\n");
		exit(1);
	}

	header = read_data(file, 0, sizeof(struct mach_header_64));
	if (header->cputype != CPU_TYPE_X86_64) {
		fprintf(stderr, "invalid cputype: can only operate on x86_64 mach-o executables\n");
		exit(1);
	}
	ncmds = header->ncmds;
	free(header);

	offset = sizeof(struct mach_header_64);
	for (size_t  i = 0; i < ncmds; i++) {
		cmd = read_data(file, offset, sizeof(struct load_command));
		if (cmd->cmd == LC_SEGMENT_64) {
			segment = read_data(file, offset, sizeof(struct segment_command_64));
			if (strcmp(segment->segname, PAGEZERO_SEGMENT_NAME) == 0) {
				if (segment->vmaddr != 0) {
					printf("%s segment has invalid vmaddr 0x%llx, setting to 0x0\n",
						segment->segname, segment->vmaddr);
					segment->vmaddr = 0;
					fseek(file, offset, SEEK_SET);
					fwrite(segment, sizeof(struct segment_command_64), 1, file);
				} else {
					printf("%s segment has valid vmaddr 0x%llx\n",
						segment->segname, segment->vmaddr);
				}
			}
			free(segment);
		}
		offset += cmd->cmdsize;
		free(cmd);
	}

	fclose(file);
	return 0;
}

#include <cstdio>
#include <cstring>
#include <cerrno>
#include <cstdlib>
#include <cstdint>

#include <vector>

#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>

#if defined (__APPLE__)
typedef char mincore_char_t;
#else
typedef unsigned char mincore_char_t;
#endif

static size_t page_size = 0;

typedef std::pair<uintptr_t,uintptr_t> mem_range_t;

void print_map(std::vector<mem_range_t> &map)
{
	for (auto &ent : map) {
		printf("0x%016lx - 0x%016lx\n", ent.first, ent.second);
	}
}

void add_page(std::vector<mem_range_t> &map, uintptr_t addr)
{
	if (map.size() == 0) {
		map.push_back(std::make_pair(addr, addr + page_size));
	} else if (map.back().second == addr) {
		map.back().second += page_size;
	} else {
		map.push_back(std::make_pair(addr, addr + page_size));
	}
}

void scan_memory(std::vector<mem_range_t> &map, uintptr_t start, size_t length)
{
	size_t bufsize = (length + page_size - 1) / page_size;

#if defined(__APPLE__)
	mincore_char_t *buf = new mincore_char_t[bufsize];
	if (!buf) {
		printf("malloc failed: %s\n", strerror(errno));
		exit(9);
	}
	if (mincore((void*)0, length, buf) != 0) {
		printf("mincore failed: %s\n", strerror(errno));
		exit(9);
	}
	for (size_t i = 0; i < bufsize; i++) {
		if (buf[i]) {
			uintptr_t addr = start + i * page_size;
			add_page(map, addr);
		}
	}
	free(buf);
#else
	for (size_t i = 0; i < bufsize; i++) {
		uintptr_t addr = start + i * page_size;
		if (msync((void*)addr, 4096, MS_ASYNC) == 0) {
			add_page(map, addr);
		}
	}
#endif
}

int main(int argc, char **argv)
{
	std::vector<mem_range_t> map;
	page_size = getpagesize();
	scan_memory(map, 0, 0x100000000UL);
	print_map(map);
	return 0;
}
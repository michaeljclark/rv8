//
//  riscv-color.h
//

#include <cstring>
#include <vector>
#include <string>

#include "riscv-util.h"
#include "riscv-color.h"

const char* ansi_color_names[] = {
	"black",
	"red",
	"green",
	"yellow",
	"blue",
	"magenta",
	"cyan",
	"white",
	nullptr
};

size_t riscv_color_to_ansi_index(std::string color)
{
	const char** p = ansi_color_names;
	size_t i = 0;
	while (*p) {
		if (color == *p) return i;
		p++;
		i++;
	}
	panic("color: %s not found", color.c_str());
	return 0;
}

std::string riscv_colors_to_ansi_escape_sequence(std::string fg_color, std::string bg_color, ansi_color_spec spec)
{
	char buf[32];
	size_t fg_color_num = riscv_color_to_ansi_index(fg_color);
	size_t bg_color_num = riscv_color_to_ansi_index(bg_color);
	switch (spec) {
		case ansi_color_keep:
			break;
		case ansi_color_normal:
			if (bg_color_num != 0 /* black */) {
				fg_color_num = bg_color_num;
				bg_color_num = 0;
			}
			break;
		case ansi_color_reverse:
			if (bg_color_num != 7 /* white */) {
				bg_color_num = fg_color_num;
				fg_color_num = 7;
			}
			break;
	}
	snprintf(buf, sizeof(buf), "\x1B[%lu;%lum", 30 + fg_color_num, 40 + bg_color_num);
	return buf;
}

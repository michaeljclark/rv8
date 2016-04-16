//
//  riscv-color.h
//

#ifndef riscv_color_h
#define riscv_color_h

#define S_COLOR      "\x1B["
#define S_NORMAL     "0;"
#define S_BOLD       "1;"
#define S_UNDERSCORE "4;"
#define S_REVERSE    "7;"
#define F_BLACK      "30;"
#define F_RED        "31;"
#define F_GREEN      "32;"
#define F_YELLOW     "33;"
#define F_BLUE       "34;"
#define F_MAGENTA    "35;"
#define F_CYAN       "36;"
#define F_WHITE      "37;"
#define B_BLACK      "40m"
#define B_RED        "41m"
#define B_GREEN      "42m"
#define B_YELLOW     "43m"
#define B_BLUE       "44m"
#define B_MAGENTA    "45m"
#define B_CYAN       "46m"
#define B_WHITE      "47m"
#define T_RESET      "\x1B[m"

#define OPCODE_BEGIN S_COLOR S_UNDERSCORE F_YELLOW B_BLACK
#define BITS_BEGIN   S_COLOR S_REVERSE F_GREEN B_BLACK
#define FORMAT_BEGIN S_COLOR S_BOLD F_RED B_BLACK
#define LEGEND_BEGIN S_COLOR F_WHITE B_BLACK
#define EXT_BEGIN    S_COLOR F_RED B_BLACK

extern const char* ansi_color_names[];

enum ansi_color_spec {
	ansi_color_keep,
	ansi_color_normal,
	ansi_color_reverse
};

size_t riscv_color_to_ansi_index(std::string color);
std::string riscv_colors_to_ansi_escape_sequence(std::string fg_color, std::string bg_color,
	ansi_color_spec spec = ansi_color_keep);

#endif

//
//  color.h
//

#ifndef rv_color_h
#define rv_color_h

#define _COLOR_BEGIN       "\x1B["
#define _COLOR_SEP         ";"
#define _COLOR_END         "m"
#define _COLOR_RESET       "\x1B[m"
#define _COLOR_NORMAL      "0"
#define _COLOR_BOLD        "1"
#define _COLOR_UNDERSCORE  "4"
#define _COLOR_REVERSE     "7"
#define _COLOR_FG_BLACK    "30"
#define _COLOR_FG_RED      "31"
#define _COLOR_FG_GREEN    "32"
#define _COLOR_FG_YELLOW   "33"
#define _COLOR_FG_BLUE     "34"
#define _COLOR_FG_MAGENTA  "35"
#define _COLOR_FG_CYAN     "36"
#define _COLOR_FG_WHITE    "37"
#define _COLOR_BG_BLACK    "40"
#define _COLOR_BG_RED      "41"
#define _COLOR_BG_GREEN    "42"
#define _COLOR_BG_YELLOW   "43"
#define _COLOR_BG_BLUE     "44"
#define _COLOR_BG_MAGENTA  "45"
#define _COLOR_BG_CYAN     "46"
#define _COLOR_BG_WHITE    "47"

extern const char* ansi_color_names[];

enum ansi_color_spec {
	ansi_color_keep,
	ansi_color_normal,
	ansi_color_reverse
};

extern ssize_t rv_color_to_ansi_index(std::string color);
extern std::string rv_colors_to_ansi_escape_sequence(std::string fg_color, std::string bg_color,
	ansi_color_spec spec = ansi_color_keep);

#endif

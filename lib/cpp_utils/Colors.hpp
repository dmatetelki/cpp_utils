#ifndef COLORS_HPP
#define COLRS_HPP


// http://www.linux.gr/cgi-bin/man2html?console_codes+4


/// Formatters:

/// 1 set bold
#define F_BOLD "1"

/// 2 set half-bright (simulated with color on a color display)
#define F_HALF_BRIGHT "2"

/// 4 set underscore (simulated with color on a color display)
/// (the colors used to simulate dim or underline are set using ESC ] ...)
#define F_UNDERSCORE "4"

/// 5 set blink
#define F_BLINK "5"

/// 7 set reverse video
#define F_REVERSE_COLORS "7"

/// 10  reset selected mapping, display control flag, and toggle meta flag
/// (ECMA-48 says "primary font").
#define F_PRIMARY_FONT "10"

/// 11  select null mapping, set display control flag, reset toggle meta flag
/// (ECMA-48 says "first alternate font").
#define F_FIRST_ALTERNATE_FONT "11"

/// 12  select null mapping, set display control flag, set toggle meta flag
/// (ECMA-48 says "second alternate font").
/// The toggle meta flag causes the high bit of a byte to be toggled
/// before the mapping table translation is done.
#define F_SECOND_ALTERNATE_FONT "12"

/// 21  set normal intensity (ECMA-48 says "doubly underlined")
#define F_DOUBLY_UNDERLINED "21"

/// 22  set normal intensity
#define F_NORMAL_INTENSITY "22"

/// 24  underline off
#define F_UNDERLINE_OFF "24"

/// 25  blink off
#define F_BLINK_OFF "25"

/// 27  reverse video off
#define F_REVERSE_VIDEO_OFF "27"



/// Foregrounds:

/// 30  set black foreground
#define FG_BLACK "30"

/// 31  set red foreground
#define FG_RED "31"

/// 32  set green foreground
#define FG_GREEN "32"

/// 33  set brown foreground
#define FG_BROWN "33"

/// 34  set blue foreground
#define FG_BLUE "34"

/// 35  set magenta foreground
#define FG_MAGENTA "35"

/// 36  set cyan foreground
#define FG_CYAN "36"

/// 37  set white foreground
#define FG_WHITE "37"

/// 38  set underscore on, set default foreground color
#define FG_UNDERSCORE_ON "38"

/// 39  set underscore off, set default foreground color
#define FG_UNDERSCORE_OFF "39"



/// Backgronds:

/// 40  set black background
#define BG_BLACK "40"

/// 41  set red background
#define BG_RED "41"

/// 42  set green background
#define BG_GREEN "42"

/// 43  set brown background
#define BG_BROWN "43"

/// 44  set blue background
#define BG_BLUE "44"

/// 45  set magenta background
#define BG_MAGENTA "45"

/// 46  set cyan background
#define BG_CYNA "46"

/// 47  set white background
#define BG_WHITE "47"

/// 49  set default background color
#define BG_DEFAULT "49"


#ifndef NOCOLOR
  #define COLOR(x)             COLOR_FG(x)
  #define COLOR_FG(x)          "\33[0;" x "m"
  #define COLOR_BG(x)          "\33[0;" FG_UNDERSCORE_OFF ";" x "m"
  #define COLOR_FG_BG(x,y)     "\33[0;" x ";" y "m"
  #define COLOR_F(x)           "\33[" x ";" FG_UNDERSCORE_OFF "m"
  #define COLOR_F_FG(x,y)      "\33[" x ";" y ";" BG_DEFAULT "m"
  #define COLOR_F_FG_BG(x,y,z) "\33[" x ";" y ";" z "m"
  #define COLOR_RESET          "\33[0m"
#else
  #define COLOR(x)             ""
  #define COLOR_FG(x)          ""
  #define COLOR_BG(x)          ""
  #define COLOR_FG_BG(x,y)     ""
  #define COLOR_F(x)           ""
  #define COLOR_F_FG(x,y)      ""
  #define COLOR_F_FG_BG(x,y,z) ""
  #define COLOR_RESET          ""
#endif

// #define REMOVE_PREV_LINE "\b"


#endif // COLORS_HPP

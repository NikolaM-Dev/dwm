// -------------------------------- Appearance ---------------------------------

// border pixel of windows
static const unsigned int borderpx = 1;
// gaps between windows
static const unsigned int gappx = 0;
// snap pixel
static const unsigned int snap = 32;
// 0: sloppy systray follows selected monitor, >0: pin systray to monitor X
static const unsigned int systraypinning = 0;
// systray spacing
static const unsigned int systrayspacing = 2;
// 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor
static const int systraypinningfailfirst = 1;
// 0 means no systray
static const int showsystray = 1;
// 0 means no bar
static const int showbar = 1;
// 0 means bottom bar
static const int topbar = 1;

// ---------------------------------- Fonts ------------------------------------

static const char *fonts[] = {"UbuntuMono Nerd Font:size=14:weight=extrabold:antialias=true:autohint:true"};
static const char dmenufont[] = "UbuntuMono Nerd Font:size=12:antialias=true:autohint=true";

// ---------------------------------- Colors -----------------------------------

struct Theme
{
	char *inactive;
	char *active;
	char *bg;
	char *focus;
};

static const struct Theme material = {
	.inactive = "#4c566a",
	.active = "#ffffff",
	.bg = "#0f101a",
	.focus = "#a151d3"};

static const struct Theme onedark = {
	.inactive = "#4c566a",
	.active = "#ffffff",
	.bg = "#1e2127",
	.focus = "#E06C75"};

static const struct Theme nord = {
	.inactive = "#4c566a",
	.active = "#ffffff",
	.bg = "#2e3440",
	.focus = "#81a1c1"};

static const struct Theme monokai_pro = {
	.inactive = "#727072",
	.active = "#2d2a2e",
	.bg = "#2d2a2e",
	.focus = "#a9dc76"};

static const struct Theme dracula = {
	.inactive = "#727072",
	.active = "#f8f8f2",
	.bg = "#0f101a",
	.focus = "#6272a4"};

static const struct Theme free_code_camp = {
	.inactive = "#6e6b6e",
	.active = "#f8f8f2",
	.bg = "#0a0a23",
	.focus = "#454581"};

static const char window_border[] = "#000000";

static const char *colors[][3] = {
	// fg                 bg             border

	{free_code_camp.inactive, free_code_camp.bg, window_border},
	{free_code_camp.active, free_code_camp.focus, free_code_camp.focus},

	{dracula.inactive, dracula.bg, window_border},
	{dracula.active, dracula.focus, dracula.focus},

	{material.inactive, material.bg, window_border},
	{material.active, material.focus, material.focus},

	{onedark.inactive, onedark.bg, window_border},
	{onedark.active, onedark.focus, onedark.focus},

	{nord.inactive, nord.bg, window_border},
	{nord.active, nord.focus, nord.focus},

	{monokai_pro.inactive, monokai_pro.bg, window_border},
	{monokai_pro.active, monokai_pro.focus, monokai_pro.focus},
};

// -------------------------------- Workspaces ---------------------------------

static const char *tags[] = {"??? ", "??? ", "??? ", "??? ", "??? ", "???  ", "??? ", "??? ", "??? "};

static const Rule rules[] = {
	/* xprop(1):
	 * WM_CLASS(STRING) = instance, class
	 * WM_NAME(STRING) = title
	 */

	// class      instance    title       tags mask     isfloating   monitor
	{"Gimp", NULL, NULL, 0, 1, -1},
	{"Firefox", NULL, NULL, 1 << 8, 0, -1},
};

// ---------------------------------- Layouts ----------------------------------

static const float mfact = 0.50;  // factor of master area size [0.05..0.95]
static const int nmaster = 1;	  // number of clients in master area
static const int resizehints = 1; // 1 means respect size hints in tiled resizals
// #define FORCE_VSPLIT 1			  // nrowgrid layout: force two clients to always split vertically

#include "layouts.c"
static const Layout layouts[] = {
	// symbol   arrange function
	{"[]", tile},	   // Default: Master on left, slaves on right
	{"[TTT]", bstack}, // Master on top, slaves on bottom

	{"[@]", spiral},   // Fibonacci spiral
	{"[\\]", dwindle}, // Decreasing in size right and leftward

	{"[DD]", doubledeck}, // Master on left, slaves in monocle-like mode on right
	{"[M]", monocle},	  // All windows on top of eachother

	{"|M|", centeredmaster},		 // Master in middle, slaves on sides
	{">M>", centeredfloatingmaster}, // Same but master floats

	// {"[HHH]", grid} //
	// {"><>", NULL}, // no layout function means floating behavior
	{NULL, NULL},
};

// -------------------------------- Keybindings --------------------------------

#define MODKEY Mod4Mask

#define TAGKEYS(KEY, TAG)                                          \
	{MODKEY, KEY, view, {.ui = 1 << TAG}},                         \
		{MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}}, \
		{MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},          \
		{MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

// Helper for spawning shell commands in the pre dwm-5.0 fashion
#define SHCMD(cmd)                                           \
	{                                                        \
		.v = (const char *[]) { "/bin/sh", "-c", cmd, NULL } \
	}

// Commands
static char dmenumon[2] = "0";
static const char *termcmd[] = {"alacritty", NULL};
static const char *dmenucmd[] = {
	"dmenu_run", "-m", dmenumon,
	"-fn", dmenufont,
	NULL};

#include <X11/XF86keysym.h>

static Key keys[] = {
	// ------------------ Windows ------------------

	// Switch between windows
	{MODKEY, XK_j, focusstack, {.i = +1}},
	{MODKEY, XK_k, focusstack, {.i = -1}},

	// Change window sizes
	{MODKEY, XK_o, incnmaster, {.i = +1}},
	{MODKEY | ShiftMask, XK_o, incnmaster, {.i = -1}},
	{MODKEY | ShiftMask, XK_h, setmfact, {.f = -0.05}},
	{MODKEY | ShiftMask, XK_l, setmfact, {.f = +0.05}},

	// Toggle floating
	{MODKEY | ShiftMask, XK_f, togglefloating, {0}},

	// Toggle bar
	{MODKEY, XK_b, togglebar, {0}},

	// Move windows in current stack
	{MODKEY | ShiftMask, XK_j, rotatestack, {.i = +1}},
	{MODKEY | ShiftMask, XK_k, rotatestack, {.i = -1}},

	// Cycle layouts
	{MODKEY, XK_Tab, cyclelayout, {.i = +1}},
	{MODKEY | ShiftMask, XK_Tab, cyclelayout, {.i = -1}},

	{MODKEY, XK_t, setlayout, {.v = &layouts[0]}},			   // tile
	{MODKEY | ShiftMask, XK_t, setlayout, {.v = &layouts[1]}}, // bstack
	{MODKEY, XK_y, setlayout, {.v = &layouts[2]}},			   // spiral
	{MODKEY | ShiftMask, XK_y, setlayout, {.v = &layouts[3]}}, // dwindle
	{MODKEY, XK_u, setlayout, {.v = &layouts[4]}},			   // deck
	{MODKEY | ShiftMask, XK_u, setlayout, {.v = &layouts[5]}}, // monocle
	{MODKEY, XK_i, setlayout, {.v = &layouts[6]}},			   // centeredmaster
	{MODKEY | ShiftMask, XK_i, setlayout, {.v = &layouts[7]}}, // centeredfloatingmaster

	// Increase - decrease gaps
	{MODKEY, XK_g, setgaps, {.i = -1}},
	{MODKEY | ShiftMask, XK_g, setgaps, {.i = +1}},

	// Focus next - prev monitor
	{MODKEY, XK_period, focusmon, {.i = +1}},
	{MODKEY, XK_comma, focusmon, {.i = -1}},

	// Send window to next - prev monitor
	{MODKEY | ShiftMask, XK_period, tagmon, {.i = +1}},
	{MODKEY | ShiftMask, XK_comma, tagmon, {.i = -1}},

	// Change color scheme
	{MODKEY, XK_Left, schemeToggle, {0}},
	{MODKEY, XK_Right, schemeCycle, {0}},

	// Kill window
	{MODKEY, XK_q, killclient, {0}},

	// Full Screen
	{MODKEY | ShiftMask, XK_space, togglefullscr, {0}},

	// Focus Screnn
	{MODKEY, XK_space, zoom, {0}},

	// Restart dwm
	{MODKEY | ControlMask, XK_r, quit, {1}},

	// Quit dwm
	{MODKEY | ControlMask, XK_q, quit, {0}},

	// ---------------- Workspaces -----------------

	TAGKEYS(XK_1, 0)
		TAGKEYS(XK_2, 1)
			TAGKEYS(XK_3, 2)
				TAGKEYS(XK_4, 3)
					TAGKEYS(XK_5, 4)
						TAGKEYS(XK_6, 5)
							TAGKEYS(XK_7, 6)
								TAGKEYS(XK_8, 7)
									TAGKEYS(XK_9, 8)

	// ------------------- Apps --------------------

	// dmenu
	{MODKEY | ShiftMask, XK_Return, spawn, {.v = dmenucmd}},

	// rofi
	{MODKEY, XK_m, spawn, SHCMD("rofi -show drun")},

	// Window nav (rofi)
	{MODKEY | ShiftMask, XK_m, spawn, SHCMD("rofi -show")},

	// Terminal
	{MODKEY, XK_Return, spawn, SHCMD("alacritty")},

	// File explorer
	{MODKEY, XK_e, spawn, SHCMD("pcmanfm")},

	// Emoji
	{MODKEY | ShiftMask, XK_e, spawn, SHCMD("rofi -show emoji -modi emoji")},

	// Discord
	{MODKEY, XK_d, spawn, SHCMD("discord")},

	// Editor
	{MODKEY, XK_c, spawn, SHCMD("code")},

	// Browser
	{MODKEY, XK_w, spawn, SHCMD("google-chrome-stable")},

	// Redshift
	{MODKEY, XK_r, spawn, SHCMD("redshift -O 6000")},
	{MODKEY | ShiftMask, XK_r, spawn, SHCMD("redshift -x")},

	// Screenshot
	{MODKEY, XK_s, spawn, SHCMD("flameshot gui")},
	{MODKEY | ShiftMask, XK_s, spawn, SHCMD("scrot -d 3")},

	// ----------------- Hardware ------------------

	// Volume
	{0, XF86XK_AudioLowerVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -2%")},
	{0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +2%")},
	{0, XF86XK_AudioMute, spawn, SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle")},

	// Brightness
	{0, XF86XK_MonBrightnessUp, spawn, SHCMD("brightnessctl set +2%")},
	{0, XF86XK_MonBrightnessDown, spawn, SHCMD("brightnessctl set 2%-")},

};

// click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin
static Button buttons[] = {
	// click            event mask    button       function         argument
	{ClkLtSymbol, 0, Button1, setlayout, {0}},
	{ClkLtSymbol, 0, Button3, setlayout, {.v = &layouts[2]}},
	{ClkWinTitle, 0, Button2, zoom, {0}},
	{ClkStatusText, 0, Button2, spawn, {.v = termcmd}},
	{ClkClientWin, MODKEY, Button1, movemouse, {0}},
	{ClkClientWin, MODKEY, Button2, togglefloating, {0}},
	{ClkClientWin, MODKEY, Button3, resizemouse, {0}},
	{ClkTagBar, 0, Button1, view, {0}},
	{ClkTagBar, 0, Button3, toggleview, {0}},
	{ClkTagBar, MODKEY, Button1, tag, {0}},
	{ClkTagBar, MODKEY, Button3, toggletag, {0}},
};

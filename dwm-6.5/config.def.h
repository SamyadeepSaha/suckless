/* See LICENSE file for copyright and license details. */

/* include volume key */
#include<X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "FiraCode:pixelsize=16" };
static const char dmenufont[]       = "FiraCode:pixelsize=16";
static const char col_gray1[]       = "#000000";
static const char col_gray2[]       = "#7f7f7f";
static const char col_gray3[]       = "#e5e5e5";
static const char col_gray4[]       = "#ffffff";
static const char col_cyan[]        = "#cd0000";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { "#cccccc", "#000000", "#000000" },
	[SchemeSel]  = { "#ffffff", "#764d30",  "#764d30"  },
};

static const char *const autostart[] = {
	"slstatus", NULL,
	"clipmenud", NULL,
	//"transmission-daemon", NULL,
	"touchpad", NULL,
	"unclutter", NULL,
	NULL /* terminate */
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "80x24", NULL };
const char *spcmd2[] = {"st", "-n", "Calculator", "-g", "80x24", "-e", "bc", "-l", NULL };
const char *spcmd3[] = {"st", "-n", "Music", "-g", "80x24", "-e", "ytfzf", "-m", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"Calculator",    spcmd2},
	{"Music",   spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",	  NULL,			NULL,		0,				1,			 -1 },
	{ "Firefox",  NULL,			NULL,		1 << 8,			0,			 -1 },
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1,			 -1 },
	{ NULL,		  "Calculator",		NULL,		SPTAG(1),		1,			 -1 },
	{ NULL,		  "Music",	NULL,		SPTAG(2),		1,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.52; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", NULL };
static const char *termcmd[]  = { "st", NULL };
/* volume commands */
static const char *upvol[]   = { "pamixer", "--increase", "1", NULL };
static const char *downvol[] = { "pamixer", "--decrease", "1", NULL };
static const char *mutevol[] = { "pamixer", "--toggle-mute", NULL };
/* media commands */
static const char *mplaypause[] = { "playerctl", "--all-players", "play-pause", NULL };
static const char *mnext[] = { "playerctl", "--all-players", "next", NULL };
static const char *mprev[] = { "playerctl", "--all-players", "previous", NULL };
/* brightness commands */
static const char *brightnessup[] = { "xbacklight", "-inc", "5", NULL };
static const char *brightnessdown[] = { "xbacklight", "-dec", "5", NULL };
/* other commands */
static const char *print[]  = { "screenshot", NULL };
static const char *powermenu[]  = { "powermenu", NULL };
static const char *bookmark[]  = { "bookmark", NULL };
static const char *touchpad[]  = { "touchpad", NULL };
static const char *yankmenu[]  = { "clipmenu", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,            			XK_a,  	   togglescratch,  {.ui = 0 } },
	{ MODKEY,            			XK_c,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,            			XK_r,	   togglescratch,  {.ui = 2 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          {.v = powermenu} },
    /* volume key bind */
    { 0,                            XF86XK_AudioLowerVolume,  spawn,          {.v = downvol } },
    { 0,                            XF86XK_AudioMute,         spawn,          {.v = mutevol } },
    { 0,                            XF86XK_AudioRaiseVolume,  spawn,          {.v = upvol   } },
    /* media control key */
    { 0,                            XF86XK_AudioPrev,  spawn,          {.v = mprev } },
    { 0,                            XF86XK_AudioPlay,         spawn,          {.v = mplaypause } },
    { 0,                            XF86XK_AudioNext,  spawn,          {.v = mnext   } },
    /* screen brightness key bind */
    { 0,                            XF86XK_MonBrightnessUp,   spawn,          {.v = brightnessup   } },
    { 0,                            XF86XK_MonBrightnessDown, spawn,          {.v = brightnessdown   } },
    /* screenshot keybind */
    { 0,                            XK_Print, spawn,          {.v = print   } },
    /* Others */
	{ MODKEY,                       XK_w,      spawn,          {.v = bookmark } },
	{ MODKEY,                       XK_s,      spawn,          {.v = touchpad } },
	{ MODKEY,                       XK_y,      spawn,          {.v = yankmenu } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button1,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


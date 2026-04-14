#ifndef GLOBALS_H
	#define GLOBALS_H
	#include <X11/Xlib.h>

	#define MAX(a, b) ((a) > (b) ? (a) : (b))
	extern Display* dpy;
	extern XWindowAttributes attr;
	extern XButtonEvent start;
	extern XEvent ev;
    extern Window focused_win;
#endif

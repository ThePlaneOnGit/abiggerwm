#ifndef GLOBALS_H
	#define GLOBALS_H
	#include <X11/Xlib.h>

	#define MAX(a, b) ((a) > (b) ? (a) : (b))
	Display* dpy;
	XWindowAttributes attr;
	XButtonEvent start;
	XEvent ev;
#endif

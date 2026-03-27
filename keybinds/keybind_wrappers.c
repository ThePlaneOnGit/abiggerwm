#include "../include/shared.h"
#include "../include/globals.h"
/* Keybind Wrappers */

/* Define Any Wrappers For Functions Here, Make Sure The Return void and have no args
 * Keep In Mind The Main Loop Checks For Dead Processes 
 * Convention For Function Names Is The Key Preceded By K With Masks First In Camel Case
 * Eg: KAltF1, KCtrlAltT, KAltMouse1
 * */

void KAltF1(void){
	XRaiseWindow(dpy, ev.xkey.subwindow);
}

void KAltMouse1(void){
	if (start.subwindow == None) return;
	int xdiff = ev.xbutton.x_root - start.x_root;
	int ydiff = ev.xbutton.y_root - start.y_root;
	XMoveResizeWindow(dpy, start.subwindow,
		attr.x + xdiff,
		attr.y + ydiff,
		attr.width,
		attr.height);
}

void KAltMouse3(void){
	if (start.subwindow == None) return;
	int xdiff = ev.xbutton.x_root - start.x_root;
	int ydiff = ev.xbutton.y_root - start.y_root;
	XMoveResizeWindow(dpy, start.subwindow,
		attr.x,
		attr.y,
		MAX(1, attr.width  + xdiff),
		MAX(1, attr.height + ydiff));
}

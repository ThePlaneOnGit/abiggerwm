/* 
ABiggerWM: A Window Manager That Adds A Few Cool Things
Copyright (C) 2026 ANNONYMUS PERSON

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include <X11/Xlib.h>
#include "../keybinds/keybinds.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SANITIZED(masks) (masks & (Mod1Mask | Mod4Mask | Mod5Mask | ControlMask | ShiftMask))
Display* dpy;
XWindowAttributes attr;
XButtonEvent start;
XEvent ev;

void setup(void){
	if (!(dpy = XOpenDisplay(0x0))) exit(1);

	for (int i = 0; keybinds[i].key != NULL; i++)
		XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym(keybinds[i].key)), SANITIZED(keybinds[i].masks), DefaultRootWindow(dpy),
				True, GrabModeAsync, GrabModeAsync);

	for (int i = 0; mouse_events[i].button != (long long) NULL; i++)
		XGrabButton(dpy, mouse_events[i].button, SANITIZED(mouse_events[i].masks), DefaultRootWindow(dpy),
				True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

	start.subwindow = None;
}

void mouse(XButtonEvent* start, XEvent* ev){
	if (ev->type == ButtonRelease || start->subwindow == None){
		start->subwindow = None;
		return;
	}

	for (int i = 0; mouse_events[i].button != (long long) NULL; i++){
		if ((start->button == mouse_events[i].button) &&
				((SANITIZED(ev->xkey.state) == mouse_events[i].masks))){
			(*mouse_events[i].function)();
			return;
		}
	}
	return;

}

void keybd(XWindowAttributes* attr, XButtonEvent* start, XEvent* ev){
	if (ev->xkey.subwindow == None) return;
	if (ev->type == ButtonPress && ev->xbutton.subwindow != None) {
		XGetWindowAttributes(dpy, ev->xbutton.subwindow, attr);
		*start = ev->xbutton;
		return;
	}

	if (ev->type != KeyPress ) return;
	for (int i = 0; keybinds[i].key != NULL; i++){
		if ((ev->xkey.keycode == XStringToKeysym(keybinds[i].key)) &&
				((SANITIZED(ev->xkey.state) == keybinds[i].masks))){
			(*keybinds[i].function)();
			return;
		}
	};
	return;
}

int main(void) {
	setup();
	for (;;) {
		XNextEvent(dpy, &ev);
		waitpid(-1, NULL, WNOHANG);
		if (ev.type == MotionNotify || ev.type == ButtonRelease)
			mouse(&start, &ev);
		else if (ev.type == ButtonPress || ev.type == KeyPress)
			keybd(&attr, &start, &ev);
	}
}

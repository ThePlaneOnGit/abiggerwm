/* vim: set tabstop=6 noexpandtab shiftwidth=6 softtabstop=0 colorcolumn=100 ft=c syntax=c:
syntax on:
*/
// -*- mode: c indent-tabs-mode: t tab-width: 6 fill-column: 100 -*-
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
#include "../include/window.h"
#include "../include/log.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SANITIZED(masks) (masks & (Mod1Mask | Mod4Mask | Mod5Mask | ControlMask | ShiftMask))

Display* dpy;
XWindowAttributes attr;
XButtonEvent start;
XEvent ev;
Windows windows;

#define CMP_MASK(byte, mask) ((byte) & mask ? #mask : "")
#define GET_MASKS(mask_byte) \
	CMP_MASK(mask_byte, Mod1Mask) ,\
	CMP_MASK(mask_byte, Mod2Mask) ,\
	CMP_MASK(mask_byte, Mod3Mask) ,\
	CMP_MASK(mask_byte, Mod4Mask) ,\
	CMP_MASK(mask_byte, Mod5Mask) ,\
	CMP_MASK(mask_byte, ControlMask) ,\
	CMP_MASK(mask_byte, ShiftMask)
#define MASK_PATTERN "%s%s%s%s%s%s%s"

void mouse(Display* dpy, XButtonEvent* start, XEvent* ev){
	if (start->subwindow == None) return;
	focus_window(dpy, start->subwindow, &windows);

	for (int i = 0; mouse_events[i].button != (long long) NULL; i++){
		if ((start->button == mouse_events[i].button) &&
				((SANITIZED(ev->xkey.state) == mouse_events[i].masks))){
			LOG("Got Mouse Event Of Button %d with masks "MASK_PATTERN, 
					mouse_events[i].button,
					GET_MASKS(SANITIZED(mouse_events[i].masks)));
			(*mouse_events[i].function)();
			return;
		}
	}
	return;

}

void keybd(Display* dpy, XWindowAttributes* attr, XButtonEvent* start, XEvent* ev){
	if (ev->xkey.subwindow == None) return;
	if (ev->type == ButtonPress && ev->xbutton.subwindow != None) {
		XGetWindowAttributes(dpy, ev->xbutton.subwindow, attr);
		*start = ev->xbutton;

		focus_window(dpy, ev->xbutton.subwindow, &windows);
		LOG("Got ButtonPress, Focused Window, Returning");
		return;
	}

	if (ev->type != KeyPress ) return;
	for (int i = 0; keybinds[i].key != NULL; i++){
		KeyCode keycode = XKeysymToKeycode(dpy, XStringToKeysym(keybinds[i].key));
		if (keycode && (ev->xkey.keycode == keycode) &&
				((SANITIZED(ev->xkey.state) == keybinds[i].masks))){
			LOG("Added Keybind Of Key %s with masks "MASK_PATTERN, keybinds[i].key,
				GET_MASKS(SANITIZED(keybinds[i].masks)));
			(*keybinds[i].function)();
			return;
		}
	}
	return;
}

void setup(Display** dpy, XButtonEvent* start, Window* focused_win){
	BEGIN_LOG();
	if (!(*dpy = XOpenDisplay(0x0))) FALIURE("Failed To Open Display");
	LOG("Selected Inputs SubstructureRedirectMask | SubstructureNotifyMask");
	XSelectInput(*dpy, DefaultRootWindow(*dpy),
			SubstructureRedirectMask | SubstructureNotifyMask);

	for (int i = 0; keybinds[i].key != NULL; i++){
		LOG("Added Keybind Of Key %s with masks "MASK_PATTERN, keybinds[i].key,
				GET_MASKS(SANITIZED(keybinds[i].masks)));
		XGrabKey(*dpy, XKeysymToKeycode(*dpy, XStringToKeysym(keybinds[i].key)),
			SANITIZED(keybinds[i].masks), DefaultRootWindow(*dpy),
			True, GrabModeAsync, GrabModeAsync);
	}

	for (int i = 0; mouse_events[i].button != (long long) NULL; i++){
		LOG("Added Mouse Event Of Button %d with masks "MASK_PATTERN, mouse_events[i].button,
				GET_MASKS(SANITIZED(mouse_events[i].masks)));
		XGrabButton(*dpy, mouse_events[i].button,
				SANITIZED(mouse_events[i].masks), DefaultRootWindow(*dpy),
				True, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
				GrabModeAsync, GrabModeAsync, None, None);
	}
	XSync(*dpy, False);
	*focused_win     = None;
	start->subwindow = None;
	return;
}

int main(void) {
	setup(&dpy, &start, &focused_win);
	for (;;) {
		XNextEvent(dpy, &ev);
		waitpid(-1, NULL, WNOHANG);
		switch (ev.type){
			case MotionNotify:
				mouse(dpy, &start, &ev);
				break;
			case ButtonRelease:
				mouse(dpy, &start, &ev);
				break;
			case ButtonPress:
				keybd(dpy, &attr, &start, &ev);
				break;
			case KeyPress:
				keybd(dpy, &attr, &start, &ev);
				break;
			case MapRequest:
				add_window(dpy, &windows, ev.xmaprequest.window);
				break;
			case DestroyNotify:
				remove_window(dpy, &windows, ev.xdestroywindow.window);
				break;
			case UnmapNotify:
				unmap_window(dpy, &windows, ev.xunmap.window);
				break;
		}
	}
}

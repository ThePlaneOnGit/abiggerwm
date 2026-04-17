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

#include "../include/window.h"
Window focused_win = None;

ssize_t get_win_index(Window win, Windows* wins){
	// Returns index of window if it exists in the `wins` object, returns -1 instead
	for (size_t i = 0; i < wins->used; i++)
		if (wins->data[i] == win)
			return i;
	return -1;
}

//Basic Wrapper Functions

void unmap_window(Display* dpy, Windows* windows, Window window){
	ssize_t index = get_win_index(window, windows);
	if (index == -1) return;
	windows->unmapped[index] = 1;
}

_Bool is_unmapped(Window window, Windows* windows){
	ssize_t index = get_win_index(window, windows);
	if (index == -1) return 1;
	return windows->unmapped[index];
}


void focus_window(Display* dpy, Window win) {
	// Sets given window to `focused_win` and edits properties about it and the previous window
	if (win == None) {
		focused_win = None;
		return;
	}

	if (focused_win != None && focused_win != win) {
		XSetWindowBorderWidth(dpy, focused_win, 1);
	}

	XRaiseWindow(dpy, win);
	XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
	XSetWindowBorderWidth(dpy, win, 3);

	focused_win = win;
	XFlush(dpy);
}

void add_window(Display* dpy, Windows* windows, Window window){
	// Adds (and maps) window to windows array
	if (get_win_index(window, windows) == -1) return;
	if (windows->max == windows->used){
		windows->max *= 2;
		windows->data = realloc(windows->data, windows->max);
	}
	windows->used++;
	windows->data[windows->used] = window;
	XMapWindow(dpy, window);
	windows->unmapped[windows->used] = 0;
	focus_window(dpy, window);
}

void remove_window(Display* dpy, Windows* windows, Window window){
	// Removes (and unmaps) given window from windows array
	if (!is_unmapped(window, windows)) unmap_window(dpy, windows, window);

	ssize_t index = get_win_index(window, windows);
	if (index == -1) return;
	for (size_t i = (size_t)index; i < windows->used; i++)
		windows->data[i] = windows->data[i + 1];
}

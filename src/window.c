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
#include "../include/log.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <errno.h>

#include <X11/Xlib.h>

Window focused_win = None;

typedef struct Windows {
	size_t max;
	size_t used;
	Window* data;
	_Bool* unmapped;
} Windows;

int size_overflows(size_t n){ // Thanks, musl
	if (n >= SIZE_MAX / 2 - (1 << 12)){
		errno = ENOMEM;
		return true;
	}
	return false;
}

void win_double(Windows* windows){
	// Doubles the size of the windows array so it can hold more windows
	LOG("Doubling Size Of Windows");
	if (size_overflows(windows->max * 2 * sizeof(Window)))
		FALIURE("Too many windows to be stored");

	LOG("Allocating New Memory");
	Window* new_wins = (Window*)malloc(windows->max * 2 * sizeof(Window));
	_Bool* new_unmap = (_Bool* )malloc(windows->max * 2 * sizeof(_Bool));

	LOG("Copying Memory Over");
	memcpy(new_wins, windows->data, windows->max * sizeof(Window));
	memcpy(new_unmap, windows->unmapped, windows->max * sizeof(_Bool));

	LOG("Freeing Old Memory");
	free(windows->data);
	free(windows->unmapped);

	LOG("Setting Values To New Memory");
	windows->data = new_wins;
	windows->unmapped = new_unmap;
}
ssize_t get_win_index(Window win, Windows* wins){
	// Returns index of window if it exists in the `wins` object, returns -1 instead
	LOG("Getting Index Of Window %ld", win);
	for (size_t i = 0; i < wins->used; i++)
		if (wins->data[i] == win){
			LOG("Found Window %ld at index %jd", win, i);
			return i;
		}
	return -1;
}

//Basic Wrapper Functions

void unmap_window(Display* dpy, Windows* windows, Window window){
	LOG("Unmapped Window");
	ssize_t index = get_win_index(window, windows);
	if (index == -1) return;
	windows->unmapped[index] = true;
}

_Bool is_unmapped(Window window, Windows* windows){
	ssize_t index = get_win_index(window, windows);
	if (index == -1) return 1;
	return windows->unmapped[index];
}


void add_window(Display* dpy, Windows* windows, Window window){
	LOG("Adding Window %ld And Mapping It", window);
	if (get_win_index(window, windows) != -1){
		LOG("Tried to add existing window %ld", window);
		return;
	}
	// Adds (and maps) window to windows array
	if (windows->max == windows->used){
		LOG("Windows Struct To Double");
		win_double(windows);
		LOG("Doubled Windows Size");
	}
	windows->data[windows->used] = window;
	windows->unmapped[windows->used] = false;
	windows->used++;
	XMapWindow(dpy, window);
//	focus_window(dpy, window, windows);
	LOG("Successfully Added And Mapped Window");
}

Windows* create_wins(void){
	Windows* end = malloc(sizeof(Windows));

	end->data     = malloc(2 * sizeof(*end->data));
	end->unmapped = malloc(2);
	end->max      = 2;
	end->used     = 0;
	return end;
}

void remove_window(Display* dpy, Windows* windows, Window window){
	// Removes (and unmaps) given window from windows array
	LOG("Removing And Unmapping Window");
	if (!is_unmapped(window, windows)) unmap_window(dpy, windows, window);

	ssize_t index = get_win_index(window, windows);
	if (index == -1) return;
	windows->used--;
	for (size_t i = (size_t)index; i < windows->used; i++){
		windows->data[i] = windows->data[i + 1];
		windows->unmapped[i] = windows->unmapped[i + 1];
	}
	LOG("Sucessfully Removed And Unmapped Window");
}

void focus_window(Display* dpy, Window win, Windows* wins) {
	// Sets given window to `focused_win` and edits properties about it and the previous window
	LOG("Focusing Window");

	if (win == None) {
		focused_win = None;
		return;
	}

	if (is_unmapped(focused_win, wins)){
		LOG("Old Focused Win Unmapped");
		focused_win = None;
	}

	if (get_win_index(win, wins) == -1){
		LOG("Window Tried To Get Focused While Not In Windows");
		add_window(dpy, wins, win);
	}

	if (focused_win != None && focused_win != win) {
		XSetWindowBorderWidth(dpy, focused_win, 1);
	}

	XRaiseWindow(dpy, win);
	XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
	XSetWindowBorderWidth(dpy, win, 3);

	focused_win = win;
	XFlush(dpy);
	LOG("Focused Window And Flushed Successfully");
}

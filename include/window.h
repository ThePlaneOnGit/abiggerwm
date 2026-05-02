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
#ifndef WINDOW_H
#define WINDOW_H

#include <X11/Xlib.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>

struct _Windows {
	size_t max;
	size_t used;
	Window* data;
	_Bool* unmapped;
};
typedef struct _Windows Windows;

extern Window focused_win;

ssize_t get_win_index(Window win, Windows* wins);

void unmap_window(Display* dpy, Windows* windows, Window window);

_Bool is_unmapped(Window window, Windows* windows);

void focus_window(Display* dpy, Window win, Windows* wins);

void add_window(Display* dpy, Windows* windows, Window window);

void remove_window(Display* dpy, Windows* windows, Window window);
#endif

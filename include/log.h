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

#ifndef LOG_H
	#define LOG_H
	#ifndef LOG_DIR
		#define LOG_DIR "~/.abiggerwmlog"
	#endif
	#include <stdio.h>
	#include <stdlib.h>
	extern FILE* log_file;
	#define BEGIN_LOG() log_file = (log_file != NULL ? log_file : fopen(LOG_DIR, "a"))
	#define END_LOG() fclose(log_file)
	#define LOG(MESSAGE, ...)\
		do {\
			fprintf((log_file != NULL ? log_file : stdout),\
				"%s at %d: "MESSAGE"\n",\
				__FILE__, __LINE__, ##__VA_ARGS__);\
			fflush((log_file != NULL ? log_file : stdout));\
		} while (0)
	#define SUCESS(MESSAGE, ...) LOG("Sucess in "MESSAGE, ##__VA_ARGS__)
	#define FALIURE(MESSAGE, ...) do {LOG("Faliure in "MESSAGE, ##__VA_ARGS__);\
		END_LOG(); exit(1);} while (0)
#endif

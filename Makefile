# vim: set tabstop=6 noexpandtab shiftwidth=6 softtabstop=0 colorcolumn=100 ft=make: syntax on:
# -*- mode: c indent-tabs-mode: t tab-width: 6 fill-column: 100 -*-
TARGET = abiggerwm
SRC = $(wildcard src/*.c)
INCLUDES = -I./keybinds -I./include -lX11
WARN_FLAGS ?= -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
COMPILATION_FLAGS ?= -ggdb -ggdb3
CC ?= cc
DEBUG ?=

all: $(TARGET)

$(TARGET): $(SRC)
	@test -f "keybinds/keybinds.h" ||\
		cat keybinds/keybinds.h.example > keybinds/keybinds.h
	@test -f "keybinds/keybind_wrappers.c" ||\
	       	cat keybinds/keybind_wrappers.c.example > keybinds/keybind_wrappers.c
	$(CC) -o $(TARGET) $(SRC) $(WARN_FLAGS) $(INCLUDES) $(COMPILATION_FLAGS) $(DEBUG)

clean:
	rm -f $(TARGET)

.PHONY: all clean

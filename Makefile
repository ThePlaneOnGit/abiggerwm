# vim: set tabstop=6 noexpandtab shiftwidth=6 softtabstop=0 colorcolumn=100 ft=make: syntax on:
# -*- mode: c indent-tabs-mode: t tab-width: 6 fill-column: 100 -*-
TARGET = abiggerwm
SRC = $(wildcard src/*.c)
export KEY_SRC = keybinds/keybinds.h keybind_wrappers.c
C_INCLUDES = -I./keybinds -I./include -lX11
WARN_FLAGS ?= -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
CC ?= cc
DEBUG ?= -ggdb -ggdb3
CFLAGS ?= $(WARN_FLAGS) -O3 $(DEBUG)

all: $(TARGET)

$(TARGET): $(SRC)
	$(shell test -f "keybinds/keybinds.h" ||\
		cat keybinds/keybinds.h.example > keybinds/keybinds.h )
	$(shell test -f "keybinds/keybind_wrappers.c" ||\
		cat keybinds/keybind_wrappers.c.example > keybinds/keybind_wrappers.c )
	$(CC) -o $@ $^ $(C_INCLUDES) $(CFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

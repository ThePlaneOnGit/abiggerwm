# vim: set tabstop=6 noexpandtab shiftwidth=6 softtabstop=0 colorcolumn=100 ft=make: syntax on:
# -*- mode: c indent-tabs-mode: t tab-width: 6 fill-column: 100 -*-
TARGET = abiggerwm
SRC = $(wildcard src/*.c)
export KEY_SRC = keybinds/keybinds.h keybind_wrappers.c
C_INCLUDES = -I./config -I./include -lX11
WARN_FLAGS ?= -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
CC ?= cc
DEBUG ?= -ggdb -ggdb3
CFLAGS ?= $(WARN_FLAGS) -O3 $(DEBUG)

all: $(TARGET)

$(TARGET): $(SRC)
	$(shell test -f "config/keybinds.h" ||\
		cat config/keybinds.h.example > config/keybinds.h )
	$(shell test -f "config/keybind_wrappers.c" ||\
		cat config/keybind_wrappers.c.example > config/keybind_wrappers.c )
	$(shell test -f "config/config.h" ||\
		cat config/config.h.example > config/config.h )
	$(CC) -o $@ $^ $(C_INCLUDES) $(CFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

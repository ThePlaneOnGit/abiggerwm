#!/usr/bin/env sh

build(){
	test -f ./keybinds/keybinds.h || cp ./keybinds/keybinds.h.example ./keybinds/keybinds.h
	test -f ./keybinds/keybind_wrappers.c || cp ./keybinds/keybind_wrappers.c.example ./keybinds/keybind_wrappers.c
	cc -o abiggerwm src/main.c -Wall -Wextra -Wpedantic -Werror -lX11
	return 0
}

clean(){
	return 0
}

main(){
	test -z "$1" || test "$1" = "build" && build
	test "$1" = "clean"  && clean
	return 1
}

main
exit $?

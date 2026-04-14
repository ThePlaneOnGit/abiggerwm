TARGET = abiggerwm
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	gcc -Wall -Wextra -I./keybinds -I./include -g -O2 -o $(TARGET) $(SRC) -lX11

clean:
	rm -f $(TARGET)

.PHONY: all clean

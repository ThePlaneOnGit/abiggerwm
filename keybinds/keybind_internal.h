struct _Keybind {
	char* key;
	unsigned int masks;
	void (*function)(void);
};

typedef struct _Keybind keybind;

struct _Mouse {
	unsigned int button;
	unsigned int masks;
	void (*function)(void);
};

typedef struct _Mouse mouse_event;

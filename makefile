all:
	gcc stax.c -o stax $$(pkg-config --cflags --libs sdl2)	
debug:
	gcc stax.c -o stax $$(pkg-config --cflags --libs sdl2) -g
clean:
	rm stax

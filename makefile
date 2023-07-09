CC := gcc
CFLAGS := -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -Wall

stax: stax.o SDL_ext.o piece.o
	${CC} ${CFLAGS} $^ -o $@
stax.o: stax.c
	$(CC) $(CFLAGS) -c $<
SDL_ext.o: SDL_ext.c
	$(CC) $(CFLAGS) -c $<
piece.o: piece.c
	$(CC) $(CFLAGS) -c $<
clean:
	rm stax stax.o SDL_ext.o

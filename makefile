CC := gcc
CFLAGS := -I/usr/include/SDL2 -D_REENTRANT -lSDL2 -Wall

stax: stax.o SDL_ext.o piece.o board.o
	${CC} ${CFLAGS} $^ -o $@
stax.o: stax.c
	$(CC) $(CFLAGS) -c $<
SDL_ext.o: SDL_ext.c
	$(CC) $(CFLAGS) -c $<
piece.o: piece.c
	$(CC) $(CFLAGS) -c $<
board.o: board.c
	$(CC) $(CFLAGS) -c $<
debug: CFLAGS += -g
debug: stax
clean:
	rm stax stax.o SDL_ext.o board.o piece.o

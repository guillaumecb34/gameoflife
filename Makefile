LIBS=-Llib -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer

all:
	gcc src/main.c -o app -Iinclude ${LIBS}
	./app
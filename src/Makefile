CC=gcc
CFLAGS=-Wall -Werror -Wextra
EXTRAFLAGS=-lncurses
SOURCE=game_of_life.c
EXENAME=game_of_life

all: compile

compile:
	$(CC) $(CFLAGS) $(SOURCE) -o $(EXENAME) $(EXTRAFLAGS)

clean:
	-rm -rf game_of_life

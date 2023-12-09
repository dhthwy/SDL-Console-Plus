cc=g++
cflags=-Wall -g -ggdb -std=c++20 $(shell pkg-config freetype2 --cflags)
ldflags=-lSDL2 -lGL -lm $(shell pkg-config freetype2 --libs)

all: example
example:
	$(cc) -o example example.cpp SDL_console.cpp $(cflags) $(ldflags) 
lib:
	$(cc) -fPIC -shared -o libSDL_console.so SDL_console.cpp $(cflags) $(ldflags)

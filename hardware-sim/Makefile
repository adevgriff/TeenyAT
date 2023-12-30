CC := gcc
CCPP := g++
CFLAGS := -c -I.. -I./include -Os
CPPFLAGS := $(CFLAGS) --std=c++23
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lgdi32 -luser32 -lkernel32 -lmingw32 -lopengl32 -static-libgcc -static-libstdc++
    RMCMD := rm 
    MAKE := mingw32-make
    MKDIR := mkdir
else
    LDFLAGS := -framework OpenGL -framework Cocoa
    RMCMD := rm -f
    MAKE := make
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS := -s -lGLU -lGL -lX11
    endif
    MKDIR := mkdir -p
endif

all: ./bin/system

./bin/system: tigr.o teenyat.o main.o
	$(CCPP) tigr.o teenyat.o main.o -o bin/system $(LDFLAGS)

tigr.o: tigr.c
	$(CC) $(CFLAGS) -o $@ $^

teenyat.o: ../teenyat.c
	$(CC) $(CFLAGS) -o $@ $^

main.o: ./main.cpp
	$(CCPP) $(CPPFLAGS) -o $@ $^


clean:
	$(RMCMD) ./bin/system *.o
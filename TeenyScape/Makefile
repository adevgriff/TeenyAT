CFLAGS += -I.. -I./include -Os 
WFLAGS = -Wall -Wextra -Wpedantic
CPPFLAGS += $(CFLAGS) --std=c++17
ifeq ($(OS),Windows_NT)
    LDFLAGS += -s -lopengl32 -lgdi32
    RMCMD = del 
else
    RMCMD = rm -f
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        LDFLAGS += -framework OpenGL -framework Cocoa
    else ifeq ($(UNAME_S),Linux)
        LDFLAGS += -s -lGLU -lGL -lX11
    endif
endif

all:
	# Compile all source files
	gcc -c tigr.c -o tigr.o $(CFLAGS)
	gcc -c ../teenyat.c -o teenyat.o $(CFLAGS) $(WFLAGS)
	g++ -c TigrUtils/util.cpp -o util.o $(CPPFLAGS) $(WFLAGS)
	g++ -c maploader.cpp -o maploader.o $(CPPFLAGS) $(WFLAGS)
	g++ -c field.cpp -o field.o $(CPPFLAGS) $(WFLAGS)
	g++ -c TigrUtils/image.cpp -o image.o $(CPPFLAGS) $(WFLAGS)
	g++ -c bot.cpp -o bot.o $(CPPFLAGS) $(WFLAGS)
	g++ -c main.cpp -o main.o $(CPPFLAGS) $(WFLAGS)
	g++ -c stb_image.c -o stb_image.o $(CPPFLAGS) $(WFLAGS)
	g++ tigr.o teenyat.o main.o util.o bot.o maploader.o field.o image.o stb_image.o -o tscape $(LDFLAGS)

clean:
	# Remove all generated files
	$(RMCMD) tscape *.o

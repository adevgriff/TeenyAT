CC := gcc
CXX := g++
CFLAGS := -c -I.. -I./include -Os
CPPFLAGS := -c $(CFLAGS) --std=c++17 
LDFLAGS :=

ifeq ($(OS),Windows_NT)
    CFLAGS += -IC:\dev\include
	LDFLAGS += -s -lopengl32 -lgdi32 
	RMCMD := rm # Using rm for compatibility with Unix-like environments in PowerShell
else
	RMCMD := rm -f
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		LDFLAGS += -framework OpenGL -framework Cocoa
	else ifeq ($(UNAME_S),Linux)
		LDFLAGS += -s -lGLU -lGL -lX11
	endif
endif

OBJ_DIR := bin
VPATH := src:../
OBJS := $(OBJ_DIR)/teenyat.o $(OBJ_DIR)/test.o
TARGET := $(OBJ_DIR)/test

all: $(TARGET)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c| $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.cpp| $(OBJ_DIR)
	$(CXX) $(CPPFLAGS) -o $@ $<

$(TARGET): $(OBJS) | $(OBJ_DIR)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: clean
clean:
	$(RMCMD) $(OBJ_DIR)/*.o $(TARGET)

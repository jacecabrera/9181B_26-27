CC = gcc
CXX = g++

CFLAGS = -std=c99 -Wall -O2 $(shell pkg-config --cflags sdl2) -I../include -I.
CXXFLAGS = -std=c++17 -Wall -O2 $(shell pkg-config --cflags sdl2) -I../include -I.
LIBS = $(shell pkg-config --libs sdl2) -lm

# Automatically find all C source files recursively inside the lvgl directory
C_SRCS = $(shell find lvgl -name "*.c")

CXX_SRCS = \
    main.cpp \
    ../src/b_template/auton/selector.cpp

# Generate object file paths
OBJS = $(C_SRCS:.c=.o) $(CXX_SRCS:.cpp=.o)

all: sim_runner

sim_runner: $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f sim_runner $(OBJS)

run: all
	./sim_runner
# Makefile for building the Wayland wrapper and your game.
#
# Commands:
#   make          Build the demo binary (default).
#   make clean    Remove binary, object files, and generated protocol files.

# Tools
CXX      := g++
CC       := gcc
SCANNER  := wayland-scanner

# Flags
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -I. -pthread
CFLAGS   := -O2 -I.
LDFLAGS  := -pthread -lwayland-client -lxkbcommon

# XDG-shell protocol XML from the "wayland-protocols" package
XDG_XML  := /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml

# Generated files (produced from XML by wayland-scanner)
XDG_HDR  := xdg-shell-client-protocol.h
XDG_SRC  := xdg-shell-protocol.c
XDG_OBJ  := xdg-shell-protocol.o

# Your C++ sources
CXX_SRCS := collision.cpp \
			cooldown.cpp \
			entity.cpp \
			font.cpp \
			game.cpp \
			geometry.cpp \
			obstacles.cpp \
			player.cpp \
			text.cpp \
			summoner.cpp \
			Window.cpp

CXX_OBJS := $(CXX_SRCS:.cpp=.o)

TARGET   := game

.PHONY: all clean

all: $(TARGET)

# Final link: C++ object files + the generated C protocol object.
$(TARGET): $(XDG_OBJ) $(CXX_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

# Every C++ source needs the generated header to exist first.
$(CXX_OBJS): $(XDG_HDR)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generated protocol stub is C, not C++.
$(XDG_OBJ): $(XDG_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Run wayland-scanner: XML -> header.
$(XDG_HDR): $(XDG_XML)
	$(SCANNER) client-header $< $@

# Run wayland-scanner: XML -> C source.
$(XDG_SRC): $(XDG_XML)
	$(SCANNER) private-code $< $@

clean:
	rm -f $(TARGET) $(CXX_OBJS) $(XDG_OBJ) $(XDG_HDR) $(XDG_SRC)

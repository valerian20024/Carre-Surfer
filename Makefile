# Makefile for building the Wayland wrapper and the game.
#
# Layout:
#   include/   headers (.hpp)
#   src/       sources (.cpp)
#   build/     object files and generated Wayland protocol files
#
# Commands:
#   make          Build the game binary (default).
#   make run      Build and run the game.
#   make clean    Remove the binary and the build directory.

# Tools
CXX      := g++
CC       := gcc
SCANNER  := wayland-scanner

# Directories
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build

# Flags (-MMD -MP generate header dependency files, so editing a .hpp
# rebuilds every .cpp that includes it)
CPPFLAGS := -I$(INC_DIR) -I$(BUILD_DIR) -MMD -MP
CXXFLAGS := -std=c++20 -Wall -Wextra -O2 -pthread
CFLAGS   := -O2
LDFLAGS  := -pthread -lwayland-client -lxkbcommon

# XDG-shell protocol XML from the "wayland-protocols" package
XDG_XML  := /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml

# Generated files (produced from XML by wayland-scanner)
XDG_HDR  := $(BUILD_DIR)/xdg-shell-client-protocol.h
XDG_SRC  := $(BUILD_DIR)/xdg-shell-protocol.c
XDG_OBJ  := $(BUILD_DIR)/xdg-shell-protocol.o

# C++ sources: every .cpp in src/ is picked up automatically
CXX_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
CXX_OBJS := $(CXX_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
DEPS     := $(CXX_OBJS:.o=.d)

TARGET   := game

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Final link: C++ object files + the generated C protocol object.
$(TARGET): $(XDG_OBJ) $(CXX_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

# Every C++ source needs the generated header to exist first.
$(CXX_OBJS): $(XDG_HDR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

# Generated protocol stub is C, not C++.
$(XDG_OBJ): $(XDG_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Run wayland-scanner: XML -> header.
$(XDG_HDR): $(XDG_XML) | $(BUILD_DIR)
	$(SCANNER) client-header $< $@

# Run wayland-scanner: XML -> C source.
$(XDG_SRC): $(XDG_XML) | $(BUILD_DIR)
	$(SCANNER) private-code $< $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(TARGET) $(BUILD_DIR)

-include $(DEPS)

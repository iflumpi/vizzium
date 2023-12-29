CC=g++
CC_FLAGS=-Wall -std=c++11
DEBUG_FLAGS=-g
RELEASE_FLAGS=-O2

SRC_ROOT=src
SRC_CONSOLE=$(SRC_ROOT)/console
SRC_HELPER=$(SRC_ROOT)/helper

EMUZETA_ROOT=lib/emuzeta80
EMUZETA_INCLUDE=$(EMUZETA_ROOT)/src
EMUZETA_LIB=$(EMUZETA_ROOT)/build

SRC=$(SRC_CONSOLE)/consolez80.cpp $(SRC_CONSOLE)/consoleui.cpp $(SRC_HELPER)/stringhelper.cpp $(SRC_ROOT)/vizzium.cpp
TARGET=build/vizzium

INCLUDE_FLAGS=-I$(SRC_CONSOLE) -I$(SRC_HELPER) -I$(EMUZETA_INCLUDE)
LINK_FLAGS=-L$(EMUZETA_LIB) -lemuzeta80 -lncurses -lpanel

.PHONY: all

all: release

debug: CC_FLAGS+=$(DEBUG_FLAGS)
debug: $(TARGET)

release: CC_FLAGS+=$(RELEASE_FLAGS)
release: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p build
	g++ $(SRC) \
		$(CC_FLAGS) \
		$(INCLUDE_FLAGS) \
		$(LINK_FLAGS) \
		-o $(TARGET)

clean:
	rm -Rf build
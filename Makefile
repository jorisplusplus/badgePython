# Select the board to build for:
ifdef BOARD_DIR
# Custom board path - remove trailing slash and get the final component of
# the path as the board name.
BOARD ?= $(notdir $(BOARD_DIR:/=))
else
# If not given on the command line, then default to GENERIC.
BOARD ?= GENERIC
BOARD_DIR ?= boards/$(BOARD)
endif

ifeq ($(wildcard $(BOARD_DIR)/.),)
$(error Invalid BOARD specified: $(BOARD_DIR))
endif

# If the build directory is not given, make it reflect the board name.
BUILD ?= build-$(BOARD)
BAUD ?= 460800
PORT ?= /dev/ttyUSB0
IDF_PATH ?= $(shell pwd)/esp-idf
IDF_EXPORT_QUIET ?= 0
SHELL := /usr/bin/env bash

IDFPY_FLAGS += -D MICROPY_BOARD=$(BOARD) -D MICROPY_BOARD_DIR=$(abspath $(BOARD_DIR)) -B $(BUILD) $(CMAKE_ARGS)

.PHONY: prepare clean build flash erase monitor menuconfig image qemu install prepare-mch2022 prepare-cz19 mch2022

all: prepare build

prepare:
	cd components/micropython/micropython/mpy-cross; make
	cp $(BOARD)/sdkconfig sdkconfig
	cp $(BOARD)/partitions.csv partitions.csv

clean:
	idf.py $(IDFPY_FLAGS) fullclean

flash:
	idf.py $(IDFPY_FLAGS) -p $(PORT) -b $(BAUD) flash

erase:
	idf.py $(IDFPY_FLAGS) -p $(PORT) -b $(BAUD) erase_flash

monitor:
	idf.py $(IDFPY_FLAGS) -p $(PORT) flash monitor
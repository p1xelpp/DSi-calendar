# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Antonio Niño Díaz, 2024

BLOCKSDS        ?= /opt/blocksds/core

# User config
NAME            := dsicalendar
GAME_TITLE      := DSi calendar
GAME_SUBTITLE   := a calendar for the Nintendo DSi!
GAME_AUTHOR	:= p1xelpp/WetOut
GAME_ICON 	:= data/icon.png


ARCHFLAGS       := -I$(CURDIR)/build/$(NAME) -I$(CURDIR)/build

include $(BLOCKSDS)/sys/default_makefiles/rom_arm9/Makefile

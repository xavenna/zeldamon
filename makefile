# ----------------------------
# Makefile Options
# ----------------------------

NAME = zeldamon
ICON = icon.png
DESCRIPTION = "A game about catching creatures"
COMPRESSED = NO
ARCHIVED = NO
HAS_PRINTF := NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz

# ----------------------------
phony : level zm

zm : bin/zeldamon.8xp

include $(shell cedev-config --makefile)

# this was added by xavenna to facilitate building levels
level : zmlvl.bin
	convbin -n ZML1 -j bin -k 8xv -i zmlvl.bin -o zmlvl.8xv

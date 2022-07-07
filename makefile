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

include $(shell cedev-config --makefile)

# ----------------------------
# Makefile Options
# ----------------------------

NAME = POWDERTI
ICON = icon.png
DESCRIPTION = "A powder simulation game on your calculator."
COMPRESSED = NO
ARCHIVED = NO

CFLAGS = -Wall -Wextra -Oz
CXXFLAGS = -Wall -Wextra -Oz -isystem "include" -std=c++17

# ----------------------------

include $(shell cedev-config --makefile)

.PHONY: test
test: all
	cemu64 --send-ram $(abspath $(BINDIR)\$(TARGET8XP)) --launch $(NAME) --no-reset
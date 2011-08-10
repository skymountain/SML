CC=g++
CFLAGS_CHK=-Wall -Wextra -Winit-self -fsyntax-only
INCLUDES=.

check-syntax:
	$(CC) $(CFLAGS_CHK) -I$(INCLUDES) -S ${CHK_SOURCES}

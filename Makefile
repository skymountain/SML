CC=g++
CFLAGS_CHK=-Wall -Wextra -Winit-self -fsyntax-only
INCLUDES=./sml/

check-syntax:
	$(CC) $(CFLAGS_CHK) $(INCLUDES) -S ${CHK_SOURCES}

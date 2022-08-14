CC=g++
CFLAGS=-Wall -Wpedantic -Wextra -Werror -std=c++17
SRCS=lupc.cpp tokenizer.cpp
HEADERS=lupc.h

.FORCE :

lupc : $(SRCS) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $@ $(SRCS)
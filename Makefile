CC=g++
CFLAGS=-Wall -Wpedantic -Wextra -Werror -std=c++17
SRCS=lupc.cpp tokenizer/tokenizer.cpp parser/parser.cpp parser/utils.cpp
HEADERS=lupc.hpp tokenizer/tokenizer.hpp parser/parser.hpp

.FORCE :

lupc : $(SRCS) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $@ $(SRCS)
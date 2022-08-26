CC=g++
CFLAGS=-Wall -Wpedantic -Wextra -Werror -std=c++17
SRCS=lupc.cpp tokenizer/tokenizer.cpp parser/parser.cpp parser/utils.cpp generator/generator.cpp
HEADERS=lupc.hpp tokenizer/tokenizer.hpp parser/parser.hpp generator/generator.hpp

.FORCE :

lupc : $(SRCS) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $@ $(SRCS)
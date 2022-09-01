CC=g++
CFLAGS=-Wall -Wpedantic -Wextra -Werror -std=c++17
SRCS=l4tc.cpp tokenizer/tokenizer.cpp parser/parser.cpp parser/utils.cpp generator/generator.cpp
HEADERS=l4tc.hpp tokenizer/tokenizer.hpp parser/parser.hpp generator/generator.hpp

.FORCE :

l4tc : $(SRCS) $(HEADERS) Makefile
	$(CC) $(CFLAGS) -o $@ $(SRCS)
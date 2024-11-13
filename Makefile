# Names: Danny Miranda, Charlie Penner, Steven Roddenberry
# Professor: Andy Wang, PhD
# Class: COP 4610
# Project: 1
# Description: This is the Project 1 makefile.

BINS = shell
C = gcc
CFLAGS = -std=gnu99 -Wall -pedantic -g

all: $(BINS)

shell: shell.o help_funcs.o builtins.o jobs.o path_search.o parser.o
	$(C) $(CFLAGS) shell.o help_funcs.o builtins.o jobs.o path_search.o parser.o -o shell

shell.o: shell.c parser.o builtins.o help_funcs.o jobs.o
	$(C) $(CFLAGS) -c shell.c -o shell.o

help_funcs.o: help_funcs.h help_funcs.c path_search.o
	$(C) $(CFLAGS) -c help_funcs.c -o help_funcs.o

builtins.o: builtins.h builtins.c parser.o
	$(C) $(CFLAGS) -c builtins.c -o builtins.o

jobs.o: jobs.h jobs.c parser.o
	$(C) $(CFLAGS) -c jobs.c -o jobs.o

path_search.o: path_search.h path_search.c
	$(C) $(CFLAGS) -c path_search.c -o path_search.o

parser.o: parser.h parser.c
	$(C) $(CFLAGS) -c parser.c -o parser.o


clean:
	rm $(BINS) *.o

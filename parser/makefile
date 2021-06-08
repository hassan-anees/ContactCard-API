CC=gcc
CFLAGS=-std=c11 -pedantic -Wall -c -g -Iinclude
ROOT = ./../

#UNAME Shell Variable
UNAME_S := $(shell uname -s)

# OS X Dependant Variables
ifeq ($(UNAME_S), Darwin)
    LIB := sharedLib.dylib
# GNU/Linux Depedant Variables
else ifeq ($(UNAME_S), Linux)
    LIB := sharedLib.so
endif


$(LIB): LinkedListAPI.o VCardUtilities.o VCardParser.o
	$(CC) -shared -o $(ROOT)$(LIB) LinkedListAPI.o VCardUtilities.o VCardParser.o

VCardUtilities.o: src/VCardUtilities.c include/LinkedListAPI.h include/VCardParser.h include/VCardUtilities.h 
	$(CC) $(CFLAGS) -c -fpic src/VCardUtilities.c -o VCardUtilities.o

VCardParser.o: src/VCardParser.c include/LinkedListAPI.h include/VCardParser.h include/VCardUtilities.h 
	$(CC) $(CFLAGS) -c -fpic src/VCardParser.c -o VCardParser.o

LinkedListAPI.o: src/LinkedListAPI.c include/LinkedListAPI.h 
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c -o LinkedListAPI.o

clean:
	rm $(ROOT)$(LIB) *.o

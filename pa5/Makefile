#File: Makefile
# By: Andy Sayler <www.andysayler.com>
# Adopted from work by: Chris Wailes <chris.wailes@gmail.com>
# Project: CSCI 3753 Programming Assignment 5
# Creation Date: 2010/04/06
# Modififed Date: 2012/04/12
# Description:
#	This is the Makefile for PA5.


CC           = gcc

CFLAGSFUSE   = `pkg-config fuse --cflags`
LLIBSFUSE    = `pkg-config fuse --libs`
LLIBSOPENSSL = -lcrypto

CFLAGS = -c -g -Wall -Wextra
LFLAGS = -g -Wall -Wextra 

.PHONY: all clean

all: Candelaresi_Andrew_pa5


Candelaresi_Andrew_pa5: Candelaresi_Andrew_pa5.o aes-crypt.o
	$(CC) $(LFLAGS)  $^ -o $@ $(LLIBSFUSE) $(LLIBSOPENSSL)

Candelaresi_Andrew_pa5.o: Candelaresi_Andrew_pa5.c
	$(CC) $(CFLAGS) $(CFLAGSFUSE)  $<

aes-crypt.o: aes-crypt.c aes-crypt.h
	$(CC) $(CFLAGS) $<  


clean:
	rm -f Candelaresi_Andrew_pa5
	rm -f *.o
	rm -f *~




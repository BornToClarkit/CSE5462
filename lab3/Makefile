# Makefile for client and server

CC = gcc
OBJCLI = ftpc.o CapitalFunctions.o
OBJSRV = ftps.o	CapitalFunctions.o
CFLAGS = -Wall
DEPS = CapitalFunctions.h
# setup for system
LIBS =

all: ftpc ftps 

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

ftpc:	$(OBJCLI)
	$(CC) $(CFLAGS) -o $@ $^

ftps:	$(OBJSRV)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm ftpc ftps


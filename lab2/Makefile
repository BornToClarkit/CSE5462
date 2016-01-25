# Makefile for client and server

CC = gcc
OBJCLI = ftpc.c 
OBJSRV = ftps.c
CFLAGS = 
# setup for system
LIBS =

all: cli srv 

cli:	$(OBJCLI)
	$(CC) $(CFLAGS) -o $@ $(OBJCLI) $(LIBS)

srv:	$(OBJSRV)
	$(CC) $(CFLAGS) -o $@ $(OBJSRV) $(LIBS)

clean:
	rm cli srv


# Makefile for timer

CC = gcc
OBJ_TIMER = timer-process.o
OBJ_TIMER_TEST = timerTester.o
CFLAGS =
# setup for system
LIBS =

all: timer-process timerTester

timer-process: $(OBJ_TIMER)
	$(CC) $(CFLAGS) -o $@ $^

timerTester: $(OBJ_TIMER_TEST)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm timer-process.o timerTester.o timer-process timerTester

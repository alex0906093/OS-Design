default: all

CC=gcc
CFLAGS=-g -lm -lrt

all: analyzecache

analyzecache: analyzecache.c analyzecache.h
	$(CC) $(CFLAGS) analyzecache.c -o analyzecache

clean:
	@echo "Cleaning for analyzecache..."
	@rm -f analyzecache

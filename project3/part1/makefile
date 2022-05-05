CC = gcc
CFLAGS = -g -Wall

OBJS = indexer.o crawler.o

all: webSearch

webSearch: webSearch.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $< -o $@
%.o : %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm *.o *~ webSearch
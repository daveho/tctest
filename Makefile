CC = gcc
CFLAGS = -g -Wall

COMMON_OBJS = demofn.o tctest.o
DEMOPROGS = demo

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c

all : $(DEMOPROGS)

demo : demo.o $(COMMON_OBJS)
	$(CC) -o $@ demo.o $(COMMON_OBJS)

demo.o : demo.c

demofn.o : demofn.c demofn.h

tctest.o : tctest.c tctest.h

.PHONY : clean

clean :
	rm -f *.o $(DEMOPROGS)

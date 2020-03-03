C_SRCS = demo.c demofn.c tctest.c
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu11

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

depend :
	$(CC) -M $(C_SRCS) > depend.mak

depend.mak :
	touch $@

include depend.mak

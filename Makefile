C_SRCS = demo.c demofn.c tctest.c
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=gnu11

CXX_SRCS = demo_cplusplus.cpp demofn_cplusplus.cpp
CXX = g++
CXXFLAGS = -g -Wall -Wextra -pedantic -std=c++14

COMMON_OBJS = tctest.o
DEMOPROGS = demo demo_cplusplus

%.o : %.c
	$(CC) $(CFLAGS) -c $*.c

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $*.cpp

all : $(DEMOPROGS)

demo : demo.o demofn.o $(COMMON_OBJS)
	$(CC) -o $@ demo.o demofn.o $(COMMON_OBJS)

demo_cplusplus : demo_cplusplus.o demofn_cplusplus.o $(COMMON_OBJS)
	$(CXX) -o $@ demo_cplusplus.o demofn_cplusplus.o $(COMMON_OBJS)

.PHONY : clean

clean :
	rm -f *.o $(DEMOPROGS) depend.mak

depend :
	$(CC) -M $(C_SRCS) > depend.mak
	$(CXX) -M $(CXX_SRCS) >> depend.mak

depend.mak :
	touch $@

include depend.mak

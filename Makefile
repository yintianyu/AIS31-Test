# 20190521

SRCS=AIS31-Test.cpp Tester.cpp QRBG-lib/QRBG.cpp
PROG=AIS31-Test.exe

PARAM=-std=c++11
CC=g++
OBJS=$(SRCS:%.cpp=%.o)
HEADS=$(SRCS:%.cpp=%.h)

default: $(PROG)
$(PROG): $(OBJS)
	$(CC) $(PARAM) -o $(PROG) $(OBJS)

%.o: %.c $(HEADS)
	$(CC) $(PARAM) -c $< -o $@

.PHONY: clean
clean: 
	rm -f $(OBJS) $(PROG)
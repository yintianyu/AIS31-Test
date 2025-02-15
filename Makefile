# 20190521

SRCS=AIS31-Test.cpp Tester.cpp QRBG-lib/QRBG.cpp
PROG=AIS31-Test.exe
CXXFLAGS+= -std=c++11
LDFLAGS+= -lpthread
ifeq ($(win32),true)
	LDLIBRARY=-lWSock32
else
	LDLIBRARY=
endif
CC=g++
OBJS=$(SRCS:%.cpp=%.o)
HEADS=$(SRCS:%.cpp=%.h)

default: $(PROG)
$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDLIBRARY) -g

%.o: %.c $(HEADS)
	$(CC) -c $< -o $@ -g

.PHONY: clean
clean: 
ifeq ($(win32),true)
	del "*.o"
	del $(PROG)
	del QRBG-lib\QRBG.o
else
	rm -f $(OBJS) $(PROG)
endif
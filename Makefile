CC = g++
CFLAGS = -c
XLIBS = -L/usr/X11R6/lib -lX11 -lXtst
OBJS = XFakeKey.o

all: MarioAi

MarioAi: $(OBJS)
	$(CC) $(OBJS) $(XLIBS) -o MarioAi

XFakeKey.o: XFakeKey.cpp
	$(CC) $(CFLAGS) $(XLIBS) XFakeKey.cpp

clean:
	rm *.o MarioAi

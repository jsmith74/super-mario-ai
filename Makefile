CC = g++
CFLAGS = -c -std=c++11 -Ofast
XLIBS = -L/usr/X11R6/lib -lX11 -lXtst
OBJS = SnesController.o main.o

all: MarioAi

MarioAi: $(OBJS)
	$(CC) $(OBJS) $(XLIBS) -o MarioAi

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

SnesController.o: SnesController.cpp
	$(CC) $(CFLAGS) $(XLIBS) SnesController.cpp

clean:
	rm *.o MarioAi *.dat

CC = g++
CFLAGS = -c
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
	rm *.o MarioAi

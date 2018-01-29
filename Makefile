CC = g++
CFLAGS = -c -std=c++11 -Ofast
LFLAGS = -std=c++11 -Ofast
XLIBS = -L/usr/X11R6/lib -lX11 -lXtst
OBJS = SnesController.o main.o Eyes.o NeuralNetwork.o
OMPFLAGS = -fopenmp


all: MarioAi

MarioAi: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(OMPFLAGS) $(XLIBS) -o MarioAi

main.o: main.cpp
	$(CC) $(CFLAGS) $(OMPFLAGS) main.cpp

SnesController.o: SnesController.cpp
	$(CC) $(CFLAGS) $(XLIBS) SnesController.cpp

Eyes.o: Eyes.cpp
	$(CC) $(CFLAGS) $(XLIBS) Eyes.cpp

NeuralNetwork.o: NeuralNetwork.cpp
	$(CC) $(CFLAGS) $(OMPFLAGS) NeuralNetwork.cpp

clean:
	rm *.o MarioAi *.txt

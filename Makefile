CC = icpc
CFLAGS = -c -std=c++11 -O3 -xavx -funroll-loops -inline-forceinline
LFLAGS = -std=c++11 -O3 -xavx -funroll-loops -inline-forceinline
OBJS = main.o NeuralNetwork.o
OMPFLAGS = -openmp


all: MIP

MIP: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) $(OMPFLAGS) $(XLIBS) -o MIP

main.o: main.cpp
	$(CC) $(CFLAGS) $(OMPFLAGS) main.cpp

NeuralNetwork.o: NeuralNetwork.cpp
	$(CC) $(CFLAGS) $(OMPFLAGS) NeuralNetwork.cpp

clean:
	rm *.o MIP *.txt

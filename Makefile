CC = g++
LIBS = -lfftw3 -lm

main: main.cpp
	$(CC) main.cpp $(LIBS) -o main -std=c++11
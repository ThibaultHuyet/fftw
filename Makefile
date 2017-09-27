CC = g++
SRC = $(wildcard src/*.cpp)
LIBS = -lfftw3f -lm -lmosquitto

main: $(SRC)
	$(CC) $(SRC) $(LIBS) -o main -std=c++11
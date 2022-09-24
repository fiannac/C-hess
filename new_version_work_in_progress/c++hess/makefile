#makefile

CC = g++

all: main

main: main.o moves_generator.o chess_types.o
	$(CC) -o main main.o moves_generator.o chess_types.o

main.o: main.cpp moves_generator.h chess_types.h
	$(CC) -c main.cpp

moves_generator.o: moves_generator.cpp moves_generator.h chess_types.h
	$(CC) -c moves_generator.cpp

chess_types.o: chess_types.cpp chess_types.h
	$(CC) -c chess_types.cpp

clean:
	rm -f *.o main
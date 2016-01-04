
CC     = g++
CFLAGS = -std=c++11

all: simpletron

simpletron: main.o cpu.o fileparser.o ram.o string_f.o
	$(CC) main.o cpu.o fileparser.o ram.o string_f.o -o simpletron


main.o: main.cpp cpu.h ram.h fileparser.h
	$(CC) -c main.cpp -o main.o $(CFLAGS)



fileparser.o: fileparser.cpp fileparser.h
	$(CC) -c fileparser.cpp -o fileparser.o $(CFLAGS)


ram.o: ram.cpp ram.h
	$(CC) -c ram.cpp -o ram.o $(CFLAGS)


string_f.o: string_f.cpp string_f.h
	$(CC) -c string_f.cpp -o string_f.o $(CFLAGS)

FLAGS =-Wall -g
CC = gcc #if we want to change compiler

all: graph 

graph:main.o libclass.a
	$(CC) $(FLAGS) -o graph main.o algo.o Digraph.o edge.o

main.o: main.c algo.h Digraph.h edge.h
	$(CC) $(FLAGS) -c main.c 

libclass.a: algo.o Digraph.o edge.o
	ar -rcs libclass.a algo.o Digraph.o edge.o

edge.o: edge.c edge.h 
	$(CC) $(FLAGS) -c edge.c 

Digraph.o: Digraph.c Digraph.h 
	$(CC) $(FLAGS) -c Digraph.c 

algo.o: algo.c algo.h
	$(CC) $(FLAGS) -c algo.c 


.PHONY: clean
clean:
	rm -f *.o *.a *.so graph
run:
	./graph

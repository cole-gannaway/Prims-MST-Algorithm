OBJS = integer.o real.o string.o sll.o dll.o queue.o bst.o avl.o scanner.o binomial.o prim.o vertex.o edge.o 
OOPTS = -std=c99 -Wall -Wextra -g -c
LOPTS = -std=c99 -Wall -Wextra -g

all : prim

prim : prim.o scanner.o binomial.o bst.o avl.o queue.o sll.o integer.o real.o string.o dll.o vertex.o edge.o 
	gcc $(LOPTS) prim.o scanner.o binomial.o bst.o avl.o queue.o sll.o integer.o real.o string.o dll.o vertex.o edge.o -lm -o prim

prim.o : prim.c
	gcc $(OOPTS) prim.c

binomial.o: binomial.h binomial.c
	gcc $(OOPTS) binomial.c

queue.o : queue.c queue.h
	gcc $(OOPTS) queue.c

sll.o : sll.c sll.h
	gcc $(OOPTS) sll.c

dll.o : dll.c dll.h
	gcc $(OOPTS) dll.c

bst.o : bst.c bst.h
	gcc $(OOPTS) bst.c

avl.o : avl.c avl.h
	gcc $(OOPTS) avl.c

integer.o : integer.c integer.h
	gcc $(OOPTS) integer.c

real.o : real.c real.h
	gcc $(OOPTS) real.c

string.o : string.c string.h
	gcc $(OOPTS) string.c

scanner.o : scanner.c scanner.h
	gcc $(OOPTS) scanner.c

edge.o :edge.c edge.h
	gcc $(OOPTS) edge.c

vertex.o : vertex.c vertex.h
	gcc $(OOPTS) vertex.c

valgrind  : all
	valgrind ./prim prim.data

test : all
	./prim prim.data

clean    :
	rm -f $(OBJS) prim

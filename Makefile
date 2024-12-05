CC = gcc
CFLAGS = -Wall -Werror

all: project3

project3: project3.o product.o cart.o
	$(CC) $(CFLAGS) -o project3 project3.o product.o cart.o

project3.o: project3.c project3.h product.h cart.h
	$(CC) $(CFLAGS) -c project3.c

product.o: product.c product.h
	$(CC) $(CFLAGS) -c product.c

cart.o: cart.c cart.h product.h
	$(CC) $(CFLAGS) -c cart.c

clean:
	rm -f *.o project3



# List C source files here. (C dependencies are automatically generated.)
SRC = \
bootload.c \
main.c \
../common/protocol.c \
../common/uart_linux.c

CFLAGS = -I../common/


all: gangprog

gangprog: dep $(SRC)
	gcc $(SRC) $(CFLAGS) -o gangprog

clean:
	rm -f *.o gangprog .deps

dep:
	echo >.deps
	for i in $(SRC); do gcc -MM $$i >>.deps; done

-include .deps




# List C source files here. (C dependencies are automatically generated.)
SRC = \
main.c \
../common/protocol.c \
../common/uart_linux.c \
../common/hal_linux.c

CFLAGS = -I../common


all: phc_cmd

phc_cmd: dep $(SRC)
	gcc $(SRC) $(CFLAGS) -o phc_cmd

clean:
	rm -f *.o phc_cmd .deps

dep:
	echo >.deps
	for i in $(SRC); do gcc -MM $$i >>.deps; done

-include .deps

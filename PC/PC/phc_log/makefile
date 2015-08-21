


# List C source files here. (C dependencies are automatically generated.)
SRC = \
common.c \
decoder.c \
main.c \
../common/uart_linux.c \
../common/hal_linux.c

CFLAGS = -I../common/


all: phc_log

phc_log: dep $(SRC)
	gcc $(SRC) $(CFLAGS) -o phc_log

clean:
	rm -f *.o phc_log .deps

dep:
	echo >.deps
	for i in $(SRC); do gcc -MM $$i >>.deps; done

-include .deps

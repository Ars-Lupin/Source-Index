CC=gcc
CFLAGS= -g -Wall -lm
DEPS= $(wildcard *.h)
OBJ= $(patsubst %.c,%.o,$(wildcard *.c))

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

principal: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clear:
	rm -rf *.o principal

run: principal
	@valgrind --leak-check=full ./principal
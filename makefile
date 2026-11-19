.PHONY: compile run clean

compile: main.o shell.o
	@gcc -o shell main.o shell.o

main.o: main.c shell.h
	@gcc -Wall -c main.c

shell.o: shell.c shell.h
	@gcc -Wall -c shell.c

run: shell
	@./shell

clean:
	rm -f *.o shell

all:pro

pro:main.o 
	gcc main.o -o pro -lncurses
	
main.o: main.c wprocess.h
	gcc -ansi -pedantic -g -c -o main.o main.c

clean:
	rm -rf *.o pro


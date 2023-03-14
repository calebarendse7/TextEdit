all:editor

editor:main.o readFile.o
	gcc main.o readFile.o -o editor -lncurses
	
main.o: main.c textedit.h
	gcc -ansi -pedantic -g -c -o main.o main.c

readFile.o: readFile.c textedit.h
	gcc -ansi -pedantic -g -c -o readFile.o readFile.c

clean:
	rm -rf *.o editor


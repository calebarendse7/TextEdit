#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <ctype.h>


void copyprevline(int col, int y, int x);
void saveFile(FILE *file);
void editFile(FILE **f, char *text);

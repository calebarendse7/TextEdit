#include "textedit.h"

int main(int argv, char *argc[]){

	FILE *file;
	WINDOW *text;

	int value;
	int k = 0; 
	char *buff;

	/*the size of the whole pad*/
	int padCols, padLines, maxCols;

	/*the cols and lines of the pad to be shown*/
	int scrCols = 0, scrLines = 0;

	/*the current displayed lines and cols*/
	int dispCols = 0, dispLines = 0;

	int lineLength = 0;
	
	/*cursor position*/
	int x = 0, y = 0;

	/*index variable*/
	int i, n, pos; 

	file = fopen("t", "r+");


	/*Initialize Window*/
	initscr();
	noecho();
	raw();

	/*Generate pad for text*/
	text = newpad(LINES, COLS);

	/*Enable keyboard keywords*/
	keypad(text, TRUE);

	padCols = COLS;
	maxCols = COLS;
	padLines = LINES;	

	buff = (char *) malloc((COLS + 1) * sizeof(char));

	if(buff == NULL){
		printf("ERROR: Buffer not allocated\n");
	}

	readFile(text, file, buff);
	getmaxyx(text, padLines, padCols);

	wmove(text, y = 0, x = 0);
	prefresh(text, scrLines, scrCols, 0, 0, LINES - 1, COLS - 1);

	while ((value = wgetch(text)) != 'q'){
		switch (value){
			case KEY_UP:
			if(y - 1 >= 0){
				y--;
				for(i = 0; i < padCols; i++){
					wmove(text, y, i);
					if(winch(text) & A_BOLD) break;
				}
				if(x >= i){
					x = i;
				}	
				wmove(text, y, x);
			}
			break;
			case KEY_DOWN:
				/*check if not last line*/
				if(y + 1 < padLines){
						/*go to next line*/
						y++;
						/*check where last pos bold*/
						for(i = 0; i < padCols; i++){
							wmove(text, y, i);
							if(winch(text) & A_BOLD) break;
						}
						if(i <= x){
							x = i;
						}
						/*move the cursor*/
						wmove(text, y, x);
				}
			break;
			case KEY_RIGHT:
			if(x + 1 < padCols){
					x++;
					/*not off current displayed columns*/
					if(x < (scrCols + (COLS))){
						/*check if bold or reached the end of lines*/
						if (winch(text) & A_BOLD){
							if(y != padLines){
								x = 0;
								scrCols = 0;
								/*if(y + 1 < padLines){}*/
									y++;
							}else{
								x--;
							}	
						}	
					}
					wmove(text, y, x);
			}
			break;
			case KEY_LEFT:
			
				x--;
				if(x < scrCols){
					/*only needs to */
					if(scrCols == 0){
						if(y != 0){
							y--;
							/*find last bumper on prev line*/
							for(x = 0; x < padCols; x++){
								wmove(text, y, x);
								if(winch(text) & A_BOLD) break;
							}
						}
					}
				}
				wmove(text, y, x);		
			
			break;

			case KEY_BACKSPACE:
			case '\b':
			case 127:
				if(x != 0){
						mvwdelch(text, y, --x);
				}else{
					for(k = 0; k < padCols; k++){
							wmove(text, y, k);
							if(winch(text) & A_BOLD) break;
					}

					if(k > COLS){
						buff = (char *)realloc(buff, sizeof(char) * (k + 1));
						
						if(buff == NULL){
							printf("ERROR: Buffer not allocated\n");
						}
					}

					wmove(text, y, x);
					winnstr(text, buff, k);

					wmove(text, --y, i);

					/*find last positon on prev line*/
					for(i = 0; i < padCols; i++){
						wmove(text, y, i);
						if(winch(text) & A_BOLD) break;
					}
						
					wdelch(text);
					n = pos = i;

					wmove(text, y, n);
					waddnstr(text, buff, k);
						
					wmove(text, ++y, 0);

					wdeleteln(text);

					wmove(text, --y, k+i);
              		wchgat(text, 1, A_BOLD, 0, NULL);

					wmove(text, y, x = pos);

					padLines--;
					wresize(text, padLines, padCols);

					/*if on last page*/
					if(scrLines + LINES > padLines){
						wclear(stdscr);
						wnoutrefresh(stdscr);
					}
				}
				
			break;

			case KEY_ENTER:
			case 10:
			/*make pad one line bigger*/
					padLines++;
					wresize(text, padLines, padCols);
					y++;	
						if(x == 0){
							winsertln(text);
							wchgat(text, 1, A_BOLD, 0, NULL); 
						}else{
							wmove(text, y, x);
							winsertln(text);	
							n = x;

							for(i = 0; i < padCols; i += lineLength){
								wmove(text, --y, n);
								winnstr(text, buff, COLS);

								lineLength = strlen(buff);

								wmove(text, ++y, i);
								waddnstr(text, buff, lineLength);

								n += lineLength;
							}

							wmove(text, --y, x);
							wclrtoeol(text);
							
							wchgat(text, 1, A_BOLD, 0, NULL);
							wmove(text, ++y, x = 0);
						}
				wmove(text, y, x);	

			break;
			default:
			/*needs to check if extending past max pad size if typing anywhere*/
			x++;
			if(x > padCols){
				padCols++;
				wresize(text, padLines, padCols);
			}
			winsch(text, value);
			wmove(text, y, x);
			break;
		}
		
		if(x < dispCols){
			scrCols = x - (x % COLS);
		}else if(x > scrCols + (COLS - 1)){
			scrCols = x;
		}

		if(y < dispLines){
			scrLines = y - (y % LINES);
		}else if (y > (scrLines + (LINES - 1))){
			scrLines = y;	
		}

		/*check if screen moved*/
		if(scrLines != dispLines || scrCols != dispCols){
			wclear(stdscr);
			wnoutrefresh(stdscr);

			dispLines = scrLines;
			dispCols = scrCols;
		}
		prefresh(text, scrLines, scrCols, 0, 0, LINES - 1, COLS - 1);
	}
	
	endwin();

	if(buff != NULL) free(buff);
	fclose(file);
	return 0;
}

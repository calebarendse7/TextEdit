#include "wprocess.h"

int main(int argv, char *argc[])
{

	FILE *file;
	WINDOW *text;

	int value;

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

	while (fgets(buff, COLS + 1, file)){
		
		lineLength = strlen(buff);
		x += lineLength;

		if(x > padCols){
				padCols += lineLength;
				wresize(text, padLines, padCols);
		}

		wmove(text, y, x - lineLength);
		waddstr(text, buff);

		if(*(buff + (lineLength - 1)) == '\n'){
			wmove(text, y, x - 1);
			wchgat(text, 1, A_BOLD, 0, NULL);
			y++;

			if(y > padLines - 1){
				padLines += 1;
                wresize(text, padLines, padCols);
			}	

			wmove(text, y, x = 0);
		}
	
	}
	wmove(text, y = 0, x = 0);

	prefresh(text, scrLines, scrCols, 0, 0, LINES - 1, COLS - 1);

	while ((value = wgetch(text)) != 'q')
	{
		switch (value){

		case KEY_UP:
				/*if prev line off screen update displayed part of pad*/
					y--;
					if(y < dispLines){
						scrLines = y - (y % LINES);
					}
					for(i = 0; i < padCols; i++){
						wmove(text, y, i);
						if(winch(text) & A_BOLD) break;
					}
					/*check if past the bumper*/
					if(x >= i){
						/*go to correct column was -1*/
						x = i;
						/*check if end of past line is off screen*/
						if(x < scrCols){
							/*move screen appropriately was i - cols before*/
							scrCols = x - (x % COLS);
						}
					}
				
			wmove(text, y, x);
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

					/*check if past the bumper*/
					if(i <= x){
						/*go to correct column*/
						x = i;

						/*check if end of past line is off screen*/
						if(x < scrCols){
							/*move screen appropriately was i - cols before*/
							scrCols = x - (x % COLS);
						}
					}

					/*check if on farther down line*/
					if(y > (scrLines + (LINES - 1))){
						/*Clear screen for new part of pad*/
						scrLines = y;
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
							/*move back to start*/
							x = 0;
							/*move screen back to start*/
							scrCols = 0;
						
							/*check if not hit max
							if(y + 1 < padLines){}*/
								y++;
							
								/*check if on next line not displayed*/
							if(y > (scrLines + (LINES - 1))){
								/*move dispayed lines*/
								scrLines = y;	
							}
							
						}else{
							x--;
						}	
					}	
				}else{
					/*move displayed columns*/
					scrCols = x;
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
							if(x > scrCols + COLS){
								scrCols = x - (x % COLS);
							}
							if(y < scrLines){
								scrLines = y - (y % LINES);
							}
							
						}
					}else{
						scrCols = x - (x % COLS);
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
					wmove(text, --y, x);

					/*find last pos on last line*/
					for(i = 0; i < padCols; i++){
						wmove(text, y, i);
						if(winch(text) & A_BOLD) break;
					}

					wdelch(text);
					n = i ;
					pos = n;
					
					/*copy characters*/
					for(i = 0; i < padCols; i++){
						/*move down*/
						wmove(text, ++y, x);
						value = winch(text);
						/*move up*/
						wmove(text, --y, n);
						waddch(text, value);
						++x;
						++n;
					}
					
					wmove(text, ++y, 0);
					wdeleteln(text);
					wmove(text, --y, x = pos);
					if(x > scrCols + COLS){
						scrCols = x - (x % COLS);
					}
					if(y < scrLines){
						scrLines = y - (y % LINES);
					}
			}
			padLines--;
			wresize(text, padLines, padCols);
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

						for(i = 0; i < padCols; i++){
								wmove(text, --y, n);
						
								value = winch(text);
						
								wmove(text, ++y, i);
						
								winsch(text, value);
								n++;
						}

						wmove(text, --y, x);
						wclrtoeol(text);
						
						wchgat(text, 1, A_BOLD, 0, NULL);
						wmove(text, ++y, x = 0);
					}
					
					if(x < scrCols){
						scrCols = x - (x % COLS);
					}
					/*check if on farther down line*/
					if(y > (scrLines + (LINES - 1))){
						/*Clear screen for new part of pad*/
						scrLines = y;
					}	

			wmove(text, y, x);	
					
		break;
		default:
			winsch(text, value);
			wmove(text, y, ++x);
		break;

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

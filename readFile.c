#include "textedit.h"

void readFile(WINDOW *text, FILE *file, char *buffer){
    int x = 0, y = 0;
    int lineLength;

    int padCols = COLS;
    int padLines = LINES;

    while (fgets(buffer, COLS + 1, file)){
            
            lineLength = strlen(buffer);
            x += lineLength;

            if(x > padCols){
                    padCols += lineLength;
                    wresize(text, padLines, padCols);
            }

            wmove(text, y, x - lineLength);
            waddstr(text, buffer);

            if(*(buffer + (lineLength - 1)) == '\n'){
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
}

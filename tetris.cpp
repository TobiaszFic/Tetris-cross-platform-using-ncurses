#include "tetris.h"


void clock::tick(){
    usleep(75000);
    value = (value + 1) mod 10;
}

//screen class functions
void screen::drawClearConsole(){
    for(int i = 1; i<=width; i++){
        mvaddch(1,i,'=');
        mvaddch(22,i,'=');
    }
    for(int i = 2; i<height-2; i++){
        mvaddch(i, 1,'|');
        mvaddch(i, 38,'|');
    }
    for(int i = 2; i<height-2; i++)
        for(int j = 2; j<width; j++)
            mvaddch(i,j,' ');
        
}

void screen::drawLocalBorders(){
    for(int i = 2; i<height-2; i++){
        mvaddch(i, 14,'|');
        mvaddch(i, 25,'|');
    }
}

void screen::drawLogo(){
    mvprintw(6,8,"TTT EEE TTT RR  I  SS");
    mvprintw(7,8," T  EEE  T  RR  I  S ");
    mvprintw(8,8," T  EEE  T  R R I SS");
    
    mvprintw(15,9,"Press enter to play");
    mvprintw(17,4,"(Press esc to close at any time)");
}

void screen::drawPlayArea(char buffer[20][10]){
    int x_offset = 15, y_offset = 2;
    for(int i = 0; i <20 ; i++)
        for(int j = 0; j<10 ; j++)
            mvaddch(y_offset+i,x_offset+j, buffer[i][j]);
}


void screen::drawRandomStuff(){
    int a;
    init_pair (1, COLOR_BLACK, COLOR_WHITE);
    attrset(COLOR_PAIR(1));
        for(int i = 2; i<height-2; i++)
            for(int j = 2; j<width; j++) {
                a = (rand() mod 10);
                if(a<=2)
                    mvaddch(i,j,' ');
            }
    attrset(A_NORMAL);
}

std::string rotateTetromino(std::string tetromino, int rotation){
    std::string rotatedTetromino;
    switch (rotation mod 4){
        case 0://no rotation
            for(int i = 0; i<4;i++)
                for(int j = 0; j<4;j++)
                    rotatedTetromino[4*i+j] = tetromino[4*i + j];
            break;
        case 1://90 degrees
            for(int i = 0; i<4;i++)
                for(int j = 0; j<4;j++)
                    rotatedTetromino[4*i+j] = tetromino[12 + i - 4*j];
            break;
        case 2://180 degrees
            for(int i = 0; i<4;i++)
                for(int j = 0; j<4;j++)
                    rotatedTetromino[4*i+j] = tetromino[15 - 4*i - j];
            break;
        case 3://270 degrees
            for(int i = 0; i<4;i++)
                for(int j = 0; j<4;j++)
                    rotatedTetromino[4*i+j] = tetromino[3 - i + 4*j];
            break;
    }
    return rotatedTetromino;
}

void drawTetromino(char buffer[20][10], std::string tetromino, int rotation, int x, int y){
    std::string rotatedTetromino = rotateTetromino(tetromino, rotation);

    for(int i = 0; i<4;i++)
        for(int j = 0; j<4;j++)
            if(rotatedTetromino[4*i + j] != '.')
                buffer[i+y][j+x] = 'X';
}

void eraseTetromino(char buffer[20][10], std::string tetromino, int rotation, int x, int y){
    std::string rotatedTetromino = rotateTetromino(tetromino, rotation);

    for(int i = 0; i<4;i++)
        for(int j = 0; j<4;j++)
            if(rotatedTetromino[4*i + j] != '.')
                buffer[i+y][j+x] = ' ';
}

bool checkLegality(char buffer[20][10], std::string tetromino, int rotation, int x, int y, int mode){//modes:1-left,2-right,3-rotation,4-down, 5-gameover
    
    int x_t = x, y_t = y;
    std::string tetromino_t;
    tetromino_t = rotateTetromino(tetromino, rotation);
    
    switch (mode) {
        case 1:
            x_t = x - 1;
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++)
                    if(tetromino_t[4*i + j] == 'X' && (x_t + j < 0 || buffer[y_t + i][x_t+j] == 'X'))
                        return FALSE;
            break;
        case 2:
            x_t = x + 1;
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++)
                    if(tetromino_t[4*i + j] == 'X' && (x_t + j > 9 || buffer[y_t + i][x_t+j] == 'X'))
                        return FALSE;
            break;
        case 3:
            tetromino_t = rotateTetromino(tetromino_t, 1);
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++)
                    if(tetromino_t[4*i + j] == 'X' && (x_t + j > 9 || x_t + j < 0 || y_t + i > 19 || buffer[y_t + i][x_t+j] == 'X'))
                        return FALSE;
            break;
        case 4:
            y_t = y + 1;
            for(int i = 0; i < 4; i++)
                for(int j = 0; j < 4; j++)
                    if(tetromino_t[4*i + j] == 'X' && (y_t + i > 19 || buffer[y_t + i][x_t+j] == 'X'))
                        return FALSE;
            break;
    }
    return TRUE;
}

void new_tetromino (std::string tetromino[7], int &current_tetromino, int &current_rotation){
    static int queued_tetromino = rand() mod 7;
    static int queued_rotation = rand() mod 4;
    
    current_tetromino = queued_tetromino;
    current_rotation = queued_rotation;
    
    queued_tetromino = rand() mod 7;
    queued_rotation = rand() mod 4;
    
    
    std::string queued = rotateTetromino(tetromino[queued_tetromino], queued_rotation);
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++){
            if(queued[4*i+j] != '.')
                mvaddch(4+i, 4+j, 'X');
            else{
                mvaddch(4+i, 4+j, ' ');
            }
        }
    
    
    
}

bool checkForGameOver(char buffer[20][10]) {
    for (int i = 0; i<10; i++)
        if (buffer[0][i] == 'X')
            return TRUE;
     return FALSE;
}

bool checkForLine(char buffer[20][10]) {
    for(int k = 0; k<=19;k++){
        int i = 0;
        for (; i<10; i++)
            if (buffer[k][i] == ' ')
                break;
        if (buffer[k][i] != ' ') {
            for(int i = k; i>0 ;i--)
                for(int j = 0; j<10; j++)
                    buffer[i][j] = buffer[i-1][j];
            for(int i = 0; i<10;i++)
                buffer[1][i] = ' ';
        }
    }
    return TRUE;
}


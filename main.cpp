#include "tetris.h"

int main (void){
    std::string tetromino[7];

    tetromino[0] += "..X.";
    tetromino[0] += "..X.";//I
    tetromino[0] += "..X.";
    tetromino[0] += "..X.";

    tetromino[1] += "..X.";
    tetromino[1] += ".XX.";//T
    tetromino[1] += "..X.";
    tetromino[1] += "....";

    tetromino[2] += "....";
    tetromino[2] += ".XX.";//C
    tetromino[2] += ".XX.";
    tetromino[2] += "....";

    tetromino[3] += "..X.";
    tetromino[3] += ".XX.";//Z
    tetromino[3] += ".X..";
    tetromino[3] += "....";

    tetromino[4] += ".X..";
    tetromino[4] += ".XX.";//S
    tetromino[4] += "..X.";
    tetromino[4] += "....";

    tetromino[5] += ".X..";
    tetromino[5] += ".X..";//L
    tetromino[5] += ".XX.";
    tetromino[5] += "....";

    tetromino[6] += "..X.";
    tetromino[6] += "..X.";//J
    tetromino[6] += ".XX.";
    tetromino[6] += "....";

    initscr ();
    noecho();
    cbreak();
    start_color();
    nodelay(stdscr,TRUE);
    screen screen;
    class clock clock;

    while(1){//program loop
        fflush(stdin);
        bool gameOver = FALSE;
        char keyPressed = 0;
        while(1){//menu loop
            screen.drawClearConsole();
            screen.drawRandomStuff();
            screen.drawLogo();
            refresh();
            usleep(750000);
            keyPressed = getch();
            if (keyPressed == 27){
                endwin();
                exit(1);
            }
            if (keyPressed == '\n')
                break;
        }
        screen.drawClearConsole();
        screen.drawLocalBorders();

        char consoleBuffer[20][10];
        for(int i = 0; i<20;i++)
            for(int j = 0; j<10;j++)
            consoleBuffer[i][j] = ' ';
        static int tet_rot_curr = 0;
        static int t_x = 3;
        static int t_y = 0;
        static int tet_type_curr = 0;
        new_tetromino(tetromino, tet_type_curr, tet_rot_curr);
        while(1){//game loop
            clock.tick();
            eraseTetromino(consoleBuffer, tetromino[tet_type_curr], tet_rot_curr, t_x, t_y);
            screen.drawPlayArea(consoleBuffer);
            keyPressed = getch();
            if (clock.value == 9)
                keyPressed = 's';
            switch (keyPressed){
                case 27:
                    endwin();
                    exit(1);
                case 'a':
                    if(checkLegality(consoleBuffer, tetromino[tet_type_curr], tet_rot_curr, t_x, t_y, 1) == TRUE)
                        t_x--;
                    break;
                case 'd':
                    if(checkLegality(consoleBuffer, tetromino[tet_type_curr], tet_rot_curr, t_x, t_y, 2) == TRUE)
                        t_x++;
                    break;
                case 'w':
                    if(checkLegality(consoleBuffer, tetromino[tet_type_curr], tet_rot_curr, t_x, t_y, 3) == TRUE)
                        tet_rot_curr++;
                    break;
                case 's':
                    if(checkLegality(consoleBuffer, tetromino[tet_type_curr], tet_rot_curr, t_x, t_y, 4) == TRUE)
                        t_y++;
                    else{
                        drawTetromino(consoleBuffer, tetromino[tet_type_curr], tet_rot_curr, t_x, t_y);
                        checkForLine(consoleBuffer);
                        gameOver = checkForGameOver(consoleBuffer);
                        new_tetromino(tetromino, tet_type_curr, tet_rot_curr);
                        t_x = 3, t_y = 0;
                    }
                    break;
            }
            drawTetromino(consoleBuffer, tetromino[tet_type_curr], tet_rot_curr, t_x, t_y);
            screen.drawPlayArea(consoleBuffer);
            refresh();

            if (gameOver == TRUE)
                break;
        }
        while(1){//game over loop
            usleep(750000);
            mvprintw(10,14,"GAME OVER");
            mvprintw(12,13,"Press ENTER");
            refresh();
            keyPressed = getch();
            if (keyPressed == 27){
                endwin();
                exit(1);
            }
            if (keyPressed == '\n')
                break;
        }
    }
    endwin();
    return 0;
}

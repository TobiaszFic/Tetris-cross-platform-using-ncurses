#pragma once
#include <ncurses.h>
#include <unistd.h>
#include <math.h>
#include <string>
#define mod %

class screen {
    //border h = 22
    //border w = 12+2 (border in-game)
    //height will remain the same, however the width must be adjusted for score display
    //w = 12+12+12 = 36
    //play area size: 10x20
    
private:
    unsigned short width = 38;
    unsigned short height = 24;//
public:
    void drawClearConsole();
    void drawLocalBorders();
    void drawLogo();
    void drawPlayArea(char buffer[20][10]);
    void updateScore(int score);
    void drawGameOver();
    void drawRandomStuff();
};

class clock{
public:
    int value;
    void tick();
};
void drawTetromino(char buffer[20][10], std::string tetromino, int rotation, int x, int y);
void eraseTetromino(char buffer[20][10], std::string tetromino, int rotation, int x, int y);
bool checkLegality(char buffer[20][10], std::string tetromino, int rotation, int x, int y, int mode);
void new_tetromino (std::string tetromino[7], int &current_tetromino, int &current_rotation);
bool checkForGameOver(char buffer[20][10]);
bool checkForLine(char buffer[20][10]);


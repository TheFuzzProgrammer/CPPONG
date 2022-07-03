#pragma once
#include <iostream>
#include <windows.h>
#include <vector>

/// <summary>
/// Writted by Fuzz.. /TheFuzzProgrammer
/// </summary>

struct ShortElement {
    int x, y;
    int lives;
};

enum consoleColors{
    BLACK = 0,
    DARKBLUE = FOREGROUND_BLUE,
    DARKGREEN = FOREGROUND_GREEN,
    DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKRED = FOREGROUND_RED,
    DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
    MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
    DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GRAY = FOREGROUND_INTENSITY,
    BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
};

enum Difficulty {
    EASY = 1,
    MEDIUM = 3,
    HARD = 5,
    IMPOSSIBLE = 10,
    NIGHTMARE = 50,
};

struct Row {
    unsigned int length;
    unsigned int zeroDistance, widthDistance, heightDistance;
    std::vector<ShortElement> *rowElements;

    Row(int _elementsLife, int _numberOfElements, int _screenWidth, int _heightDistance) {
        this->rowElements = new std::vector<ShortElement>;
        this->length = _numberOfElements;
        this->zeroDistance = widthDistance = (_screenWidth - length) / 2;
        this->heightDistance = _heightDistance;
        for (int i = 0; i < this->length; i++) {
            ShortElement temp_Se = ShortElement();
            temp_Se.y = heightDistance;
            temp_Se.x = zeroDistance + i;
            temp_Se.lives = _elementsLife;
            this->rowElements->push_back(temp_Se);
        }
    }
    ~Row() {
        delete rowElements;
    }
    std::vector<ShortElement>* GetElements() {
        return this->rowElements;
    }
    ShortElement GetElementAt(int _index) {
        return this->rowElements->at(_index);
    }

};

class Level {
    std::vector<Row*> *rows;
public:
    Level(int _numberOfLevel, Difficulty _difficulty, int _screenWidth) {
        this->rows = new std::vector<Row*>;
        Row testingRow1 = Row(_difficulty, _screenWidth - 4, _screenWidth, 5);
        Row testingRow2 = Row(_difficulty, _screenWidth - 8, _screenWidth, 6);
        Row testingRow3 = Row(_difficulty, _screenWidth - 12, _screenWidth, 7);
        switch (_numberOfLevel) {
            case 1: {
                this->rows->push_back(new Row(_difficulty, _screenWidth - 4, _screenWidth, 5));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 8, _screenWidth, 6));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 12, _screenWidth, 7));
                break;    
            } 
            
            case 2: {
                this->rows->push_back(new Row(_difficulty, _screenWidth - 4, _screenWidth, 5));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 8, _screenWidth, 6));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 12, _screenWidth, 7));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 16, _screenWidth, 8));
                break;    
            }             
            case 3: {     
                this->rows->push_back(new Row(_difficulty, _screenWidth - 12, _screenWidth, 5));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 8, _screenWidth, 6));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 4, _screenWidth, 7));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 8, _screenWidth, 8));
                this->rows->push_back(new Row(_difficulty, _screenWidth - 12, _screenWidth, 8));
                break;
            }
            
        }

    }
    ~Level() {
        for (Row* pointer : *rows) {
            delete pointer;
        }
        delete rows;
    }

    std::vector<Row*>* GetRows() {
        return this->rows;
    }
    Row* GetRowAt(int _index) {
        return this->rows->at(_index);
    }
};

class Paddle
{
    std::vector <ShortElement> *elements;
public:
    Paddle(unsigned short *quantity) {
        this->elements = new std::vector<ShortElement>;
        for (unsigned short x = 1; x <= *quantity; x++) {
            elements->push_back(ShortElement({ 0,0 }));
        }
    }
    void Move(unsigned short code) {
        for (unsigned int i = 0; i < this->elements->size(); i++) {
            switch (code) {
            case 1: {
                this->elements->at(i).x ++;
                break;
            }
            case 2: {
                this->elements->at(i).x --;
            }
            default:break;
            }
        }
    }

    std::vector<ShortElement>* GetBody() {
        return this->elements;
    }

    ~Paddle() {
        delete elements;
    }
};

class Ball {
    ShortElement* body;
    short xDirection, yDirection;
public:
    Ball(unsigned int x, unsigned int y) {
        this->body = new ShortElement;
        this->body->x = x;
        this->body->y = y;
        this->xDirection = 1;
        this->yDirection = 1;
    }
    ~Ball() {
        delete body;
    }

    ShortElement* GetBody() {
        return this->body;
    }
    void Move() {
        this->body->x += this->xDirection;
        this->body->y += this->yDirection;
    }
private:
    void SetXD(int _xDirection) {
        this->xDirection *= -_xDirection;
    }
    void SetYD(int _yDirection){
        this->yDirection *= -_yDirection;
    }
public:
    int ImpactsWall(int *_marginX,int *_marginY, int xMultiplier = 1, int yMultiplier = 1) {
        if (this->body->x == *_marginX - 1 || this->body->x == 0) {
            this->SetXD(xMultiplier);
            return 1;
        }
        if (this->body->y == *_marginY || this->body->y == 3) {
            this->SetYD(yMultiplier);
            return 2;
        }
        return 0;
    }
    bool ImpactsElementAt(int *_x, int *_y) {
        if (this->body->x == *_x && this->body->y == *_y) {
            this->ImpactsWall(_x,_y);
            return true;
        }
        return false;
    }
    int GetY() {
        return this->body->y;
    }
};
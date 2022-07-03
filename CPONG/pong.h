#include "elements.h"
#include <iostream>
#include <string>

/// <summary>
/// Writted by Fuzz.. /TheFuzzProgrammer
/// </summary>


int screenWidth = 120, screenHeight = 70;
int score = 0;
int delay = 40;
unsigned short paddleLong = 3;
bool playerScored = false;

int playerDefPosX = screenWidth / 2, playerDefPosY = screenHeight / 2;
int playerLives = 3;
bool isPlayerDead = false;
bool playerExit = false;
int livesAddedCount = 1;
bool mainMenu = true;
int levelNumber = 1;
Difficulty levelDifficulty;


void ClearScreen(wchar_t* screenBuffer) {
    for (int i = 0; i < (screenWidth * screenHeight); i++) { screenBuffer[i] = L' '; };
    Sleep(delay);
    return;
}


int pong() {
#pragma region BUFFER & CONSOLE INIT 
    SMALL_RECT cbffrect; COORD cbffcoord;
    cbffcoord.X = screenWidth; // WIDTH
    cbffcoord.Y = screenHeight; //HEIGTH
    cbffrect.Top = 0; cbffrect.Left = 0; cbffrect.Bottom = cbffcoord.Y - 1; cbffrect.Right = cbffcoord.X - 1;
    HANDLE consoleHandler = GetStdHandle(STD_OUTPUT_HANDLE); //Getting console handler
    SetConsoleScreenBufferSize(consoleHandler, cbffcoord); //Reserving buffer
    SetConsoleWindowInfo(consoleHandler, TRUE, &cbffrect);
    SetConsoleTextAttribute(consoleHandler, consoleColors::DARKYELLOW | consoleColors::BLACK);
    wchar_t* screen = new wchar_t[screenWidth * screenHeight];
    for (int i = 0; i < (screenWidth * screenHeight); i++) { screen[i] = L' '; };
    consoleHandler = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, NULL, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(consoleHandler); DWORD bytesWritten = 0;
    Paddle* paddle = new Paddle(&paddleLong);
    Ball *ball = new Ball(screenWidth/2, screenHeight/2);
    std::vector <ShortElement> *obstacles = 0;
    Level* currentLevel;
    int totalObstacles;
    paddle->GetBody()->at(0).x = screenWidth / 2 - paddle->GetBody()->size() / 2;
    //setDefaultPaddlePosition
    for (int i = 1; i < paddle->GetBody()->size(); i++) {
        paddle->GetBody()->at(i).x = paddle->GetBody()->at(i - 1).x + 1;
    }
    while (mainMenu)
    {

        if (GetAsyncKeyState((unsigned short)'1') & 0x8000) {
            levelDifficulty = Difficulty::EASY;
            mainMenu = false;
        }
        else if (GetAsyncKeyState((unsigned short)'2') & 0x8000) {
            levelDifficulty = Difficulty::MEDIUM;
            mainMenu = false;
        }
        else if (GetAsyncKeyState((unsigned short)'3') & 0x8000) {
            levelDifficulty = Difficulty::HARD;
            mainMenu = false;
        }
        else if (GetAsyncKeyState((unsigned short)'4') & 0x8000) {
            levelDifficulty = Difficulty::IMPOSSIBLE;
            mainMenu = false;
        }
        else if (GetAsyncKeyState((unsigned short)'5') & 0x8000) {
            levelDifficulty = Difficulty::NIGHTMARE;
            mainMenu = false;
        }
        for (int i = 0; i < screenWidth; i++) {
            screen[i] = '~';
            screen[i + screenWidth * 2] = '~';
        }
        wsprintf(&screen[3 * (screenWidth / 2)], L"Pong!");
        wsprintf(&screen[31 * (screenWidth / 2)], L"1 EASY!");
        wsprintf(&screen[35 * (screenWidth / 2)], L"2 MEDIUM!");
        wsprintf(&screen[39 * (screenWidth / 2)], L"3 HARD!");
        wsprintf(&screen[43 * (screenWidth / 2)], L"4 IMPOSSIBLE!");
        wsprintf(&screen[47 * (screenWidth / 2)], L"5 NIGHTMARE!");
        WriteConsoleOutputCharacterW(consoleHandler, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
    }
    currentLevel = new Level(levelNumber, levelDifficulty, screenWidth);
    totalObstacles = currentLevel->GetRows()->size();

    while (!playerExit) {

        if (!isPlayerDead) {
            if (GetAsyncKeyState((unsigned short)'A') & 0x8000) {
                if (paddle->GetBody()->at(0).x >= 1) {
                    paddle->Move(2);
                }
            }
            else if (GetAsyncKeyState((unsigned short)'D') & 0x8000) {
                if (paddle->GetBody()->at(paddle->GetBody()->size() - 1).x < screenWidth - 1) {
                    paddle->Move(1);
                }
            }

            ClearScreen(screen);
            for (ShortElement segment : *paddle->GetBody()) {
                segment.y = screenHeight - 2;
                if (ball->ImpactsElementAt(&segment.x, &segment.y)) {
                    score++;
                    playerScored = true;
                }
                screen[(screenWidth * (screenHeight - 1)) + segment.x] = '@';
            }
            //TESTING DRAW LEVEL

            for (Row* row : *currentLevel->GetRows()) {
                for (ShortElement &element : *row->GetElements()) {
                    if (element.y < screenWidth && element.lives > 0 && element.x < screenWidth) {
                        if (ball->ImpactsElementAt(&element.x, &element.y)) {
                            element.lives--;
                        }
                        if (element.lives == 1) {
                            screen[screenWidth * element.y + element.x] = '=';
                        }
                        else if (1 < element.lives <= 3) {
                            screen[screenWidth * element.y + element.x] = '#';
                        }
                        else if (3 < element.lives <= 5) {
                            screen[screenWidth * element.y + element.x] = '%';
                        }
                        else {
                            screen[screenWidth * element.y + element.x] = '&';
                        }
                        if (element.lives == 0) {
                            totalObstacles--;
                        }
                    }
                }
            }

            //Draw tittle
            for (int i = 0; i < screenWidth; i++) {
                screen[i] = '~';
                screen[i + screenWidth * 2] = '~';
            }
            wsprintf(&screen[3 * (screenWidth / 2)], L"Pong!");
            wsprintf(&screen[3 * (screenWidth / 2) + screenWidth / 3], L"Score: ");
            wsprintf(&screen[2 * (screenWidth / 2) + screenWidth / 20], L"Lives: ");

            //Draw score
            for (int i = 0; i < std::to_string(score).length(); i++) {
                screen[3 * (screenWidth / 2) + screenWidth / 3 + 7 + i] = std::to_string(score).c_str()[i];
            }
            //Draw Lives
            for (int i = 0; i < std::to_string(playerLives).length(); i++) {
                screen[2 * (screenWidth / 2) + screenWidth / 20 + 7 + i] = std::to_string(playerLives).c_str()[i];
            }

            if (ball->ImpactsWall(&screenWidth, &screenHeight) == 2 && ball->GetBody()->y > 5) {
                if (!playerScored) {
                    playerLives --;
                }
                if (playerLives < 1) {
                    isPlayerDead = true;
                }
            };

            if (score / livesAddedCount == 5) {
                playerLives++;
                livesAddedCount++;
            }

            playerScored = false;
            ball->Move();
            screen[(screenWidth * ball->GetBody()->y) + ball->GetBody()->x] = '0';
            WriteConsoleOutputCharacterW(consoleHandler, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
        }
        else {
            ClearScreen(screen);
            wsprintfW(&screen[3 * (screenWidth / 2)], L"YOU LOSE");
            wsprintfW(&screen[screenHeight / 2 * screenWidth + (screenWidth / 2)], L"Continue? Y/N");
            WriteConsoleOutputCharacterW(consoleHandler, screen, screenWidth * screenHeight, { 0,0 }, &bytesWritten);
            if (GetAsyncKeyState((unsigned short)'Y') & 0x8000) {
                playerLives++;
                isPlayerDead = false;
            }
            else if (GetAsyncKeyState((unsigned short)'N') & 0x8000) {
                playerExit = true;
            }
        }
    }

    return 0;
}


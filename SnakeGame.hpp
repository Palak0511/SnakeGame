#pragma once
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

enum mode
{
    WINDOW,
    BUTTON,
    GAME,
    GAMEOVER

};

enum motion
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum bound
{
    MINIMUM = 120,
    MAXIMUM = 860
};

enum buttonsOnPlayMode
{
    SCORE,
    LIFES,
    MENU,
    PAUSE,
    EXIT,
    TOTAL
};

class SnakeGame
{

public:
    SnakeGame();
    void start();
    ~SnakeGame();

private:
    static uint16_t score;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    SDL_Event event;
    std::string title = "SNAKE GAME";
    const uint16_t SCREEN_WIDTH = 1000, SCREEN_HEIGHT = 1000;
    SDL_Surface *backgroundImageSurface = nullptr;
    SDL_Texture *backgroundImageTexture = nullptr;
    std::string backgroundImagePath = "images/snake1.webp";
    SDL_Rect StartOrEnd = {100, 800, 800, 50};
    TTF_Font *Font = nullptr;
    SDL_Surface *StartOrEndFontSurface = nullptr;
    SDL_Texture *StartOrEndFontTexture = nullptr;
    uint8_t whereAreYouNow = WINDOW;

    SDL_Surface *tempSurface;
    SDL_Texture *ButtonsTexture[3];
    SDL_Rect ButtonRect[3];

    SDL_Rect SnakeHead;
    SDL_Rect SnakeFood;

    bool isGameRunning;
    bool SnakeIsColloidedWithFood = false;
    bool SnakeIsColloidedWithWall = false;
    bool pause = true;
    uint8_t direction;

    SDL_Rect walls[4];
    SDL_Rect gameButtons[TOTAL];
    SDL_Texture *gameButtonsTexture[TOTAL];

    SDL_Texture *gameOverTexture;
    SDL_Texture *finalScore;
    SDL_Rect finalScoreRect = {100, 200, 800, 50};

    std::vector<SDL_Rect> snakeBody;
    uint16_t lifes;

private:
    void createWindow();
    void createRenderer();
    void show();
    void update();
    void handle();
    void loadImage();
    void createStartOrEnd();
    void createWalls();
    void createButtons();
    void createButtonsRect();
    void createGameButtons();
    void keyPressed();
    void mainWindow();
    void button();
    void play();
    void boundary();
    void collisionWithFood();
    void collisionWithWalls();
    void collisionWithBody();
    void snakeEats();
    void gameKeyPressed();
    void displayWalls();
    void showGameButtons();
    void updateScoreTexture();
    void updateLifes();
    void gameOver();
    void showSnakeBody();
    void resetGame();
};
#include "SnakeGame.hpp"
#include <time.h>

uint16_t SnakeGame::score = 0;

SnakeGame::SnakeGame()
{

    SDL_Init(SDL_INIT_EVERYTHING);
    createWindow();
    createRenderer();
    IMG_Init(IMG_INIT_WEBP);
    loadImage();
    TTF_Init();
    Font = TTF_OpenFont("Text/8bitOperatorPlus8-Regular.ttf", 512);
    createStartOrEnd();
    createButtons();
    createButtonsRect();
    createWalls();
    lifes = 3;
    createGameButtons();
    SnakeFood.h = SnakeFood.w = SnakeHead.h = SnakeHead.w  = 15;
    srand(time(0));
    SnakeHead.x = 225;
    SnakeHead.y = 225;
    snakeBody.push_back(SnakeHead);
    SnakeFood.x = MINIMUM + SnakeFood.w*( (rand()%(MAXIMUM - MINIMUM) )/SnakeFood.w );
    SnakeFood.y = MINIMUM + SnakeFood.h*( (rand()%(MAXIMUM - MINIMUM) )/SnakeFood.h );
    isGameRunning = true;
    direction = RIGHT;
    
}

void SnakeGame::createWindow()
{
    window = SDL_CreateWindow(title.c_str(), 
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SDL_WINDOW_SHOWN);
}

void SnakeGame::createRenderer()
{
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void SnakeGame::loadImage()
{
    backgroundImageSurface = IMG_Load(backgroundImagePath.c_str());
    backgroundImageTexture = SDL_CreateTextureFromSurface(renderer, backgroundImageSurface);
}

void SnakeGame::createStartOrEnd()
{
    StartOrEndFontSurface = TTF_RenderText_Solid(Font, "Press 'P' to PLAY or 'X' to EXIT", {255, 255, 255, 255});
    StartOrEndFontTexture = SDL_CreateTextureFromSurface(renderer, StartOrEndFontSurface);
    
}

void SnakeGame::createButtons()
{
    tempSurface = TTF_RenderText_Solid(Font, "Press P to PLAY", {255, 0, 0, 255});
    ButtonsTexture[0] = SDL_CreateTextureFromSurface(renderer, tempSurface);

    tempSurface = TTF_RenderText_Solid(Font, "Press B to BACK", {255, 0, 0, 255});
    ButtonsTexture[1] = SDL_CreateTextureFromSurface(renderer, tempSurface);

    tempSurface = TTF_RenderText_Solid(Font, "Press X to EXIT", {255, 0, 0, 255});
    ButtonsTexture[2] = SDL_CreateTextureFromSurface(renderer, tempSurface);

    SDL_FreeSurface(tempSurface);
}

void SnakeGame::createButtonsRect()
{
    for (int i = 0; i < 3; i++)
    {
        ButtonRect[i] = {300, (100 + i * 100), 400, 50};
    }
}

void SnakeGame::createWalls()
{
    walls[UP] = {100, 100, 800, 20};
    walls[DOWN] = {100, 880, 800, 20};
    walls[LEFT] = {100, 120, 20, 760};
    walls[RIGHT] = {880, 120, 20, 760};
}

void SnakeGame::createGameButtons()
{
    gameButtons[SCORE] = {100, 50, 100, 50};
    tempSurface = TTF_RenderText_Solid(Font, ("Score : " + std::to_string(score)).c_str(), {0, 255, 0, 255});
    gameButtonsTexture[SCORE] = SDL_CreateTextureFromSurface(renderer, tempSurface);
    
    gameButtons[LIFES] = {700, 50, 200, 50};
    tempSurface = TTF_RenderText_Solid(Font,("Remaining Lifes : " + std::to_string(lifes)).c_str(),{0, 255, 0, 255});
    gameButtonsTexture[LIFES] = SDL_CreateTextureFromSurface(renderer,tempSurface);
     
    gameButtons[MENU]  = {0,950,240,50};
    tempSurface = TTF_RenderText_Solid(Font, "Press R to Restart", {0, 255, 0, 255});
    gameButtonsTexture[MENU] = SDL_CreateTextureFromSurface(renderer, tempSurface);
    
    gameButtons[PAUSE] = {320,950,360,50};
    tempSurface = TTF_RenderText_Solid(Font,"Press Space to Play or Pause",{0, 255, 0, 255});
    gameButtonsTexture[PAUSE] = SDL_CreateTextureFromSurface(renderer,tempSurface);
    
    gameButtons[EXIT]  = {760,950,240,50};
    tempSurface = TTF_RenderText_Solid(Font,"Press X to Exit",{0, 255, 0, 255});
    gameButtonsTexture[EXIT] = SDL_CreateTextureFromSurface(renderer,tempSurface);

    SDL_FreeSurface(tempSurface);
    
    
}

void SnakeGame::start()
{
    while (isGameRunning)
    {
        show();
        handle();
    }
}

void SnakeGame::show()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    switch (whereAreYouNow)
    {
    case WINDOW:
        mainWindow();
        break;

    case BUTTON:
        button();
        break;

    case GAME:
        play();
        break;
    
    case GAMEOVER:
        gameOver();
        break;

    default:
        break;
    }
    if(!(isGameRunning && lifes)){
        gameOver();
        SDL_RenderPresent(renderer);
        SDL_Delay(3000); 
    }
    else 
        SDL_RenderPresent(renderer); 
}

void SnakeGame::handle()
{
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            if (whereAreYouNow == GAME)
                gameKeyPressed();
            keyPressed();
            break;
        case SDL_QUIT:
            isGameRunning = false;
            
            break;
        }
    }
}

void SnakeGame::keyPressed()
{
    switch (event.key.keysym.sym)
    {
    case SDLK_p:
        if (whereAreYouNow == WINDOW)
            whereAreYouNow = BUTTON;

        else if (whereAreYouNow == BUTTON)
            whereAreYouNow = GAME;
        break;

    case SDLK_x:
        isGameRunning = false;
        break;

    case SDLK_b:
        if (whereAreYouNow == BUTTON)
            whereAreYouNow = WINDOW;
        break;

    default:
        break;
    }
}

void SnakeGame::gameKeyPressed()
{

    switch (event.key.keysym.sym)
    {
    case SDLK_w:
    case SDLK_UP:
        if(direction !=DOWN)
            direction = UP;
        break;

    case SDLK_a:
    case SDLK_LEFT:
        if(direction != RIGHT)
            direction = LEFT;
        break;

    case SDLK_s:
    case SDLK_DOWN:
        if(direction != UP)
            direction = DOWN;
        break;

    case SDLK_d:
    case SDLK_RIGHT:
        if( direction != LEFT)
            direction = RIGHT;
        break;

    case SDLK_SPACE:
        pause = !pause;
        break;
    
    case SDLK_r:
        score = 0;
        lifes = 3;
        updateLifes();
        updateScoreTexture();
        resetGame();
        whereAreYouNow = BUTTON;

    }
}

void SnakeGame::mainWindow()
{
    SDL_RenderCopy(renderer, backgroundImageTexture, nullptr, nullptr);
    SDL_RenderCopy(renderer, StartOrEndFontTexture, nullptr, &StartOrEnd);
}

void SnakeGame::button()
{

    for (int i = 0; i < 3; i++)
    {
        SDL_RenderCopy(renderer, ButtonsTexture[i], nullptr, &ButtonRect[i]);
    }
}

void SnakeGame::play()
{
    SDL_SetRenderDrawColor(renderer, 255, 250, 0, 255);
    SDL_RenderFillRect(renderer, &SnakeFood);
    SDL_RenderDrawRect(renderer, &SnakeFood);
    SDL_SetRenderDrawColor(renderer, 0, 0, 250, 255);
    displayWalls();
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    if (SnakeIsColloidedWithFood)
        snakeEats();
    if(!pause)
    {
        update();
        collisionWithFood();
        collisionWithWalls();
        collisionWithBody();
        boundary();
    }
    showGameButtons();
    showSnakeBody();
}

void SnakeGame::displayWalls()
{
    for (int i = 0; i < 4; i++)
    {
        SDL_RenderFillRect(renderer, &walls[i]);
        SDL_RenderDrawRect(renderer, &walls[i]);
    }
}

void SnakeGame::snakeEats()
{
    snakeBody.push_back(SnakeFood);
    srand(time(0));
    SnakeFood.x = MINIMUM + SnakeFood.w*( (rand()%(MAXIMUM-MINIMUM) )/SnakeFood.w );
    SnakeFood.y = MINIMUM + SnakeFood.h*( (rand()%(MAXIMUM-MINIMUM) )/SnakeFood.h );
    
}

void SnakeGame::collisionWithFood()
{
    
    SnakeIsColloidedWithFood = SDL_HasIntersection(&SnakeFood, &snakeBody[0]);
    if(SnakeIsColloidedWithFood)
    {
        score++;
        updateScoreTexture();
        
    }

}

void SnakeGame::collisionWithWalls()
{
    SDL_Point p;
    p.x = snakeBody[0].x;
    p.y = snakeBody[0].y;

    for (int i = 0; i < 4; i++)
    {
        if (SDL_PointInRect(&p, &walls[i]))
        {
            if(!(--lifes)){
                isGameRunning = false;
                break;
            }
            updateLifes();
            resetGame();
            break;
        }
    }
}

void SnakeGame::collisionWithBody()
{
    for(int i=3;i<snakeBody.size();i++)
    {
        if(SDL_HasIntersection(&snakeBody[0],&snakeBody[i]))
        {
            if(!(--lifes)){
                isGameRunning = false;
            }
            updateLifes();
            resetGame();
            break;

        }
        
    }
}

void SnakeGame::boundary()
{
    if (snakeBody[0].x <= -snakeBody[0].w)
        snakeBody[0].x = SCREEN_WIDTH;

    else if (snakeBody[0].x >= (SCREEN_WIDTH + snakeBody[0].w))
        snakeBody[0].x = 0;

    else if (snakeBody[0].y <= -snakeBody[0].h)
        snakeBody[0].y = SCREEN_HEIGHT;

    else if (snakeBody[0].y >= (SCREEN_HEIGHT + snakeBody[0].h))
        snakeBody[0].y = 0;
}

void SnakeGame::update()
{
    switch (direction)
    {
    case UP:
        SnakeHead.y -= (snakeBody[0].h);
        break;

    case DOWN:
        SnakeHead.y += (snakeBody[0].h);
        break;

    case LEFT:
        SnakeHead.x -= (snakeBody[0].w);
        break;

    case RIGHT:
        SnakeHead.x += (snakeBody[0].w);
        break;

    default:
        break;
    }

    for(int i = snakeBody.size() - 1; i>0; i--)
        snakeBody[i] = snakeBody[i-1];  

    snakeBody[0] = SnakeHead;

    uint8_t delay = (score<30) ? (100 - 10*(score/5)) : 40;
    SDL_Delay(delay);
     
}

void SnakeGame::updateScoreTexture()
{
    tempSurface = TTF_RenderText_Solid(Font, ("score : " + std::to_string(score)).c_str(), {0, 255, 0, 255});
    gameButtonsTexture[SCORE] = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
}

void SnakeGame::updateLifes()
{
    tempSurface = TTF_RenderText_Solid(Font,("Remaining Lifes : " + std::to_string(lifes)).c_str(),{0, 255, 0, 255});
    gameButtonsTexture[LIFES] = SDL_CreateTextureFromSurface(renderer,tempSurface);

}

void SnakeGame::showGameButtons()
{
    SDL_RenderCopy(renderer, gameButtonsTexture[SCORE], nullptr, &gameButtons[SCORE]);
    SDL_RenderCopy(renderer, gameButtonsTexture[LIFES], nullptr, &gameButtons[LIFES]);
    for(int i = MENU; i<TOTAL; i++)
        SDL_RenderCopy(renderer, gameButtonsTexture[i],nullptr,&gameButtons[i]);
}

void SnakeGame::showSnakeBody()
{
    for(auto part:snakeBody)
    {

        SDL_RenderFillRect(renderer, &part);
        SDL_RenderDrawRect(renderer, &part);
    }
}

void SnakeGame::gameOver()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    tempSurface = TTF_RenderText_Solid(Font, ("GAME OVER"), {200, 100, 100});
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    tempSurface = TTF_RenderText_Solid(Font, ("Your Score is " + std::to_string(score)).c_str(), {100,0,200});
    finalScore = SDL_CreateTextureFromSurface(renderer,tempSurface);
    SDL_RenderCopy(renderer,finalScore,nullptr,&finalScoreRect);
    SDL_RenderCopy(renderer, gameOverTexture, nullptr, &StartOrEnd);
    isGameRunning = false;
}

void SnakeGame::resetGame()
{
    snakeBody.resize(1);
    SnakeHead.x = SnakeHead.y = 225;
    snakeBody[0] = SnakeHead;
    pause = true;
    direction = RIGHT;
    
}

SnakeGame::~SnakeGame()
{

    TTF_CloseFont(Font);
    SDL_DestroyTexture(StartOrEndFontTexture);
    SDL_FreeSurface(StartOrEndFontSurface);
    SDL_DestroyTexture(backgroundImageTexture);
    SDL_FreeSurface(backgroundImageSurface);
    for (int i = 0; i < 3; i++)
        SDL_DestroyTexture(ButtonsTexture[i]);
    for(int i=SCORE;i<TOTAL;i++)
        SDL_DestroyTexture(gameButtonsTexture[i]);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

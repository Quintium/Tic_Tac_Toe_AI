#pragma once

// include libraries
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <chrono>
#include "AI.h"
#include "Board.h"
#include "Texture.h"

// enumerate states
enum State_IDs 
{
    PLAY,
    STOP
};

// main class for the SDL app
class App
{
    private:
        // boolean to stop the program once needed
        bool running;

        // window, screen and renderer variables
        SDL_Window* window = nullptr;
        SDL_Surface* screenSurface = nullptr;
        SDL_Renderer* renderer = nullptr;

        // board and ai variables
        Board* board = nullptr;
        AI* ai = nullptr;

        // initialize ai and state variables
        int AI_ID = O_ID;
        bool versusAI = true;
        int turn = X_ID;
        int state = PLAY;

        // event functions
        bool onInit();

        bool loadMedia();

        void onEvent(SDL_Event* event);

        void onLoop();

        void onRender();

        void onCleanup();

        // function for making a move
        void move(int x, int y);

    public:
        // constructor and main function
        App();
        int onExecute();
};

// screen dimension constants
const int S_WIDTH = 600;
const int S_HEIGHT = 600;
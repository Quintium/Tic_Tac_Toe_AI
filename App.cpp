#include "App.h"

// app constructor
App::App() 
{
    running = true;
}

// main function
int App::onExecute() 
{
    // try to initialize app: if failed - return -1
    if (!onInit()) 
    {
        return -1;
    }
    
    // create new event to check for future events
    SDL_Event event;

    while (running) 
    {
        // loop through event queue and pass them on to onEvent()
        while (SDL_PollEvent(&event)) 
        {
            onEvent(&event);
        }

        // call the main loop and render function
        onRender();
        onLoop();
    }

    // clean the app up before finishing
    onCleanup();

    return 0;
}

// initialize the app
bool App::onInit() {
    // initialize sdl, print error message if failed
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // create window, print error message if failed
    window = SDL_CreateWindow("Tic Tac Toe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, S_WIDTH, S_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) 
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    // create renderer for window, print error message if failed
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) 
    {
        printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // initialize image library, print error message if failed
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) 
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    // initialize SDL_ttf, print error message if failed
    if (TTF_Init() == -1)
    {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }

    // get window surface
    screenSurface = SDL_GetWindowSurface(window);

    // create new board and ai
    board = new Board(renderer);
    ai = new AI(AI_ID);

    // load media
    if (!loadMedia()) return false;

    // return true if initialization was successful
    return true;
}

// load media (pictures, songs...)
bool App::loadMedia() 
{
    if (!board->loadMedia()) return false;

    // return true if media loading was successful
    return true;
}

// function for handling events
void App::onEvent(SDL_Event* event) 
{  
    // check if the event type is mouse button down and the game is playing and it's not the ai's turn
    if (event->type == SDL_MOUSEBUTTONDOWN) {
        if (state == PLAY && (!versusAI || board->getTurn() != AI_ID)) 
        {
            // get x and y in grid based on mouse
            int x = event->button.x / (S_WIDTH / 3);
            int y = event->button.y / (S_HEIGHT / 3);

            // make that move
            move(x, y);
        }
    }
    // if the user presses a key after the game has ended
    else if (event->type == SDL_KEYDOWN && state == STOP) 
    {
        // check if space was pressed
        if (event->key.keysym.sym == SDLK_SPACE) 
        {
            // restart game
            board->restartGame();
            state = PLAY;
        }
    }
}

// loop for handling the game other than events and rendering
void App::onLoop()
{
    if (state == PLAY && board->getTurn() == AI_ID && versusAI)
    {
        auto start = std::chrono::system_clock::now();

        Move aiMove;
        for (int i = 0; i < 10; i++)
        {
            aiMove = ai->getBestMove(board);
        }

        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> timePassed = end - start;
        std::cout << "Move: " << aiMove.x << ", " << aiMove.y << "\n";
        std::cout << "Time needed: " << timePassed.count() << "s\n";
        std::cout << "Nodes searched: " << ai->nodes << "\n";
        std::cout << "\n";

        move(aiMove.x, aiMove.y);
    }
}

void App::onRender() 
{
    // clear screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // render board
    board->render();

    // update screen
    SDL_RenderPresent(renderer);
}

void App::onCleanup() {
    // make the board clean up
    board->cleanup();

    // destroy window and renderer 
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    window = nullptr;
    renderer = nullptr;

    // quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void App::move(int x, int y)
{
    if (!board->isTaken(x, y)) 
    {
        board->makeMove(x, y);

        if (board->checkWin(true) != EMPTY || board->isDraw()) 
        {
            state = STOP;
        }
    }
}

// main function
int main(int argc, char* argv[]) 
{
    App theApp;

    // execute app
    return theApp.onExecute();
}
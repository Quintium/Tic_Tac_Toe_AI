#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Texture.h"

// rename unsigned short to U16 for bitboard
typedef unsigned short U16;

// ids for X and O
enum IDs {
	EMPTY,
	X_ID,
	O_ID
};

// flags for winning position
enum WinFlags
{
	HORIZONTAL = 1,
	VERTICAL = 2,
	DIAGONAL = 4,
	ROW0 = 8,
	ROW1 = 16,
	ROW2 = 32
};

#include <vector>

// class for the board
class Board
{
	private:
		// board state
		std::vector<U16> bitboards;
		int turn;

		// renderer and images
		SDL_Renderer* renderer = nullptr;
		Texture xImage;
		Texture oImage;
		Texture gridImage;
		Texture verticalLineImage;
		Texture horizontalLineImage;
		Texture diagonalLineImage;

		// win situation
		int winPosition = 0;
		
	public:
		// constructor
		Board(SDL_Renderer* appRenderer);

		// replacing and checking cell on the grid
		void makeMove(int x, int y);
		void unmakeMove(int x, int y);
		bool isTaken(int x, int y);

		// checking for wins and draws
		int checkWin();
		int checkWinSave();
		bool isDraw();

		// function for resetting the grid
		void restartGame();

		// functions for loading media, rendering and cleaning up
		bool loadMedia();
		void render();
		void cleanup();

		// get functions
		int getTurn();
};
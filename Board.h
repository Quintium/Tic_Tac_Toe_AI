#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include "Texture.h"

// ids for X and O
enum IDs {
	X_ID = 1,
	O_ID = 2,
};

// flags for winning position
enum WinFlags
{
	HORIZONTAL = 1,
	VERTICAL,
	DIAGONAL
};

// class for the board
class Board
{
	private:
		// grid
		int grid[3][3];

		// renderer and images
		SDL_Renderer* renderer = nullptr;
		Texture xImage;
		Texture oImage;
		Texture gridImage;
		Texture verticalLineImage;
		Texture horizontalLineImage;
		Texture diagonalLineImage;

		// win situation
		int winRow = 0;
		int winPosition = 0;

		// helper function for checking wins
		bool equals3(int a, int b, int c);
		
	public:
		// constructor
		Board(SDL_Renderer* appRenderer);

		// replacing and checking cell on the grid
		void replace(int id, int x, int y);
		bool isTaken(int x, int y);

		// checking for wins and draws
		int checkWin(bool save);
		bool isDraw();

		// function for resetting the grid
		void resetGrid();

		// functions for loading media, rendering and cleaning up
		bool loadMedia();
		void render();
		void cleanup();
};
#include "Board.h"
#include "App.h"

// constructor with renderer
Board::Board(SDL_Renderer* appRenderer) 
{
	renderer = appRenderer;

	restartGame();
}

// make a move
void Board::makeMove(int x, int y)
{
	grid[x][y] = turn;
	turn = (turn == X_ID) ? O_ID : X_ID;
}

// unmake a move
void Board::unmakeMove(int x, int y)
{
	turn = (turn == X_ID) ? O_ID : X_ID;
	grid[x][y] = EMPTY;
}

// return if cell is taken
bool Board::isTaken(int x, int y) 
{
	return grid[x][y] != EMPTY;
}

// return if all numbers are equal and not empty
bool Board::equals3(int a, int b, int c) 
{
	return a == b && b == c && a != EMPTY;
}

// check for a win
int Board::checkWin(bool save) 
{
	// loop through 3 rows
	for (int row = 0; row < 3; row++) 
	{
		// check if vertical row is equal
		if (equals3(grid[row][0], grid[row][1], grid[row][2])) 
		{
			// save win situation if needed
			if (save) 
			{
				winRow = row;
				winPosition = VERTICAL;
			}

			// return the id of winner
			return grid[row][0];
		}

		// check if horizontal row is equal
		if (equals3(grid[0][row], grid[1][row], grid[2][row])) 
		{
			// save win situation if needed
			if (save) {
				winRow = row;
				winPosition = HORIZONTAL;
			}

			// return the id of winner
			return grid[0][row];
		}
	}

	// check if first diagonal is equal
	if (equals3(grid[0][0], grid[1][1], grid[2][2])) 
	{
		// save win situation if needed
		if (save)
		{
			winRow = 0;
			winPosition = DIAGONAL;
		}

		// return the id of winner
		return grid[0][0];
	}

	// check if second diagonal is equal
	if (equals3(grid[0][2], grid[1][1], grid[2][0])) 
	{
		// save win situation if needed
		if (save)
		{
			winRow = 1;
			winPosition = DIAGONAL;
		}

		// return the id of winner
		return grid[0][2];
	}

	// return empty if no one won
	return EMPTY;
}

// check if board is a draw
bool Board::isDraw()
{
	// loop through grid
	for (int x = 0; x < 3; x++) 
	{
		for (int y = 0; y < 3; y++)
		{
			// if cell is empty, return that it isn't a draw
			if (grid[x][y] == EMPTY) 
			{
				return false;
			}
		}
	}

	// if no cells are empty, return draw
	return true;
}

// reset the grid
void Board::restartGame()
{
	turn = X_ID;

	// set all cells to 0
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			grid[i][j] = EMPTY;
		}
	}

	// reset win situation
	winPosition = 0;
	winRow = 0;
}

// load all media
bool Board::loadMedia()
{
	// load PNG textures, return false if failed
	if (!xImage.loadFromFile("Images/X.png", renderer)) return false;
	if (!oImage.loadFromFile("Images/O.png", renderer)) return false;
	if (!gridImage.loadFromFile("Images/Grid.png", renderer)) return false;
	if (!verticalLineImage.loadFromFile("Images/VerticalLine.png", renderer)) return false;
	if (!horizontalLineImage.loadFromFile("Images/HorizontalLine.png", renderer)) return false;
	if (!diagonalLineImage.loadFromFile("Images/DiagonalLine.png", renderer)) return false;

	return true;
}

// render the board
void Board::render()
{
	// define third of the window size and small offsets
	int thirdWidth = S_WIDTH / 3;
	int thirdHeight = S_HEIGHT / 3;
	int xOffset = S_WIDTH / 40;
	int yOffset = S_HEIGHT / 40;

	// render the grid full screen
	SDL_Rect gridRect = { 0, 0, S_WIDTH, S_HEIGHT };
	gridImage.render(&gridRect);

	// loop through cells
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			// make a rect for the cell
			SDL_Rect imageRect = { thirdWidth * i, thirdHeight * j, thirdWidth, thirdHeight };

			// render the image of the cell
			if (grid[i][j] == X_ID)
			{
				xImage.render(&imageRect);
			}
			else if (grid[i][j] == O_ID)
			{
				oImage.render(&imageRect);
			}
		}
	}

	// if game is won by someone
	if (winPosition != 0)
	{
		// if the win is horizontal, render horizontal line based on row
		if (winPosition == HORIZONTAL)
		{
			SDL_Rect lineRect = { 0, thirdHeight * winRow, S_WIDTH, thirdHeight };
			horizontalLineImage.render(&lineRect);
		}
		// if the win is vertical, render vertical line based on row
		else if (winPosition == VERTICAL)
		{
			SDL_Rect lineRect = { thirdWidth * winRow, 0, thirdWidth, S_HEIGHT };
			verticalLineImage.render(&lineRect);
		}
		// if the win is diagonal, render diagonal line and rotate it if needed
		else
		{
			SDL_Rect lineRect = { 0, 0, S_WIDTH, S_HEIGHT };
			SDL_Point centerPoint = { S_WIDTH / 2, S_HEIGHT / 2 };
			diagonalLineImage.render(&lineRect, nullptr, winRow == 0 ? 0 : 90);
		}
	}
}

// clean the board up
void Board::cleanup()
{
	// free all the images
	xImage.free();
	oImage.free();
	gridImage.free();
	verticalLineImage.free();
	horizontalLineImage.free();
	diagonalLineImage.free();
}

int Board::getTurn()
{
	return turn;
}
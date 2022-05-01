#include "Board.h"
#include "App.h"

// constructor with renderer
Board::Board(SDL_Renderer* appRenderer) 
{
	renderer = appRenderer;

	restartGame();
}

// reset the grid
void Board::restartGame()
{
	turn = X_ID;

	// full bitboard for empty squares, empty bitboards for x and o squares
	bitboards.resize(3);
	bitboards[EMPTY] = (U16(1) << 9) - 1;
	bitboards[X_ID] = 0;
	bitboards[O_ID] = 0;

	// reset win situation
	winPosition = 0;
}

// make a move
void Board::makeMove(int x, int y)
{
	// change empty and player bitboards at spot based on x and y
	U16 spot = U16(1) << (x + y * 3);
	bitboards[turn] ^= spot;
	bitboards[EMPTY] ^= spot;

	turn = (turn == X_ID) ? O_ID : X_ID;
}

// unmake a move
void Board::unmakeMove(int x, int y)
{
	turn = (turn == X_ID) ? O_ID : X_ID;

    // change empty and player bitboards at spot based on x and y
	U16 spot = U16(1) << (x + y * 3);
	bitboards[turn] ^= spot;
	bitboards[EMPTY] ^= spot;
}

// return if cell is taken
bool Board::isTaken(int x, int y) 
{
	return (bitboards[EMPTY] & (U16(1) << (x + y * 3))) == 0;
}


// efficient check for a win
int Board::checkWin()
{
	// check if bitboards contain winning rows
	U16 bbX = bitboards[X_ID];
	if ((bbX & 448) == 448 || // 111000000
		(bbX & 56) == 56 || // 000111000
		(bbX & 7) == 7 || // 000000111
		(bbX & 292) == 292 || // 100100100
		(bbX & 146) == 146 || // 010010010
		(bbX & 73) == 73 || // 001001001
		(bbX & 273) == 273 || // 100010001
		(bbX & 84) == 84)  // 001010100
	{
		return X_ID;
	}

	U16 bbO = bitboards[O_ID];
	if ((bbO & 448) == 448 || // 111000000
		(bbO & 56) == 56 || // 000111000
		(bbO & 7) == 7 || // 000000111
		(bbO & 292) == 292 || // 100100100
		(bbO & 146) == 146 || // 010010010
		(bbO & 73) == 73 || // 001001001
		(bbO & 273) == 273 || // 100010001
		(bbO & 84) == 84)  // 001010100
	{
		return O_ID;
	}

	return EMPTY;
}

// check for a win and save winning row
int Board::checkWinSave() 
{
	winPosition = 0;

	// check if bitboards contain winning rows
	U16 bbX = bitboards[X_ID];
	if ((bbX & 448) == 448) winPosition = HORIZONTAL | ROW2; // 111000000
	if ((bbX & 56) == 56) winPosition = HORIZONTAL | ROW1; // 000111000
	if ((bbX & 7) == 7) winPosition = HORIZONTAL | ROW0; // 000000111
	if ((bbX & 292) == 292) winPosition = VERTICAL | ROW2; // 100100100
	if ((bbX & 146) == 146) winPosition = VERTICAL | ROW1; // 010010010
	if ((bbX & 73) == 73) winPosition = VERTICAL | ROW0; // 001001001
	if ((bbX & 273) == 273) winPosition = DIAGONAL | ROW0; // 100010001
	if ((bbX & 84) == 84) winPosition = DIAGONAL | ROW1;  // 001010100

	if (winPosition != 0)
	{
		return X_ID;
	}

	U16 bbO = bitboards[O_ID];
	if ((bbO & 448) == 448) winPosition = HORIZONTAL | ROW2; // 111000000
	if ((bbO & 56) == 56) winPosition = HORIZONTAL | ROW1; // 000111000
	if ((bbO & 7) == 7) winPosition = HORIZONTAL | ROW0; // 000000111
	if ((bbO & 292) == 292) winPosition = VERTICAL | ROW2; // 100100100
	if ((bbO & 146) == 146) winPosition = VERTICAL | ROW1; // 010010010
	if ((bbO & 73) == 73) winPosition = VERTICAL | ROW0; // 001001001
	if ((bbO & 273) == 273) winPosition = DIAGONAL | ROW0; // 100010001
	if ((bbO & 84) == 84) winPosition = DIAGONAL | ROW1;  // 001010100

	if (winPosition != 0)
	{
		return O_ID;
	}

	// return empty if no one won
	return EMPTY;
}

// check if board is a draw
bool Board::isDraw()
{
	// if no cells are empty, return draw
	return bitboards[EMPTY] == 0;
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
			if (bitboards[X_ID] & U16(1) << (i + j * 3))
			{
				xImage.render(&imageRect);
			}
			else if (bitboards[O_ID] & U16(1) << (i + j * 3))
			{
				oImage.render(&imageRect);
			}
		}
	}

	// if game is won by someone
	if (winPosition != 0)
	{
		// convert win row flags to int
		int winRow = winPosition & ROW0 ? 0 : (winPosition & ROW1 ? 1 : 2);

		// if the win is horizontal, render horizontal line based on row
		if (winPosition & HORIZONTAL)
		{
			SDL_Rect lineRect = { 0, thirdHeight * winRow, S_WIDTH, thirdHeight };
			horizontalLineImage.render(&lineRect);
		}
		// if the win is vertical, render vertical line based on row
		else if (winPosition & VERTICAL)
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
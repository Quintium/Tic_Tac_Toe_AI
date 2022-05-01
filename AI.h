#pragma once
#include "Board.h"

// enum for board scores
enum Score
{
	LOSS = -1,
	DRAW = 0,
	WIN = 1,
	UNDECIDED = 2
};

// simple move with x and y
struct Move
{
	int x;
	int y;
};

// class for the ai
class AI
{
	private:
		// id (x or o), best move
		int ID;
		Move bestMove = { 0, 0 };

		// functions for minimax
		int negaMax(Board* board, int alpha, int beta, int ply);
		int evaluate(Board* board);

	public:
		int nodes = 0;

		// constructor and best move function
		AI(int AI_ID);
		Move getBestMove(Board* board);
};
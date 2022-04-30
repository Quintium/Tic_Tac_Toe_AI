#pragma once
#include "Board.h"

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
		// id (x or o), depth of minimax, best move
		int ID;
		int depthLimit;
		Move bestMove = { 0, 0 };

		// functions for minimax
		int negaMax(Board* board, int alpha, int beta, int depth, int turn);
		int evaluate(Board* board, int eval_ID);

	public:
		int nodes = 0;

		// constructor and best move function
		AI(int AI_ID, int AI_depth);
		Move getBestMove(Board* board);
};
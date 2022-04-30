#include "Board.h"
#include "AI.h"

// ai constructor
AI::AI(int AI_ID, int AI_depth) 
{
	ID = AI_ID;
	depthLimit = AI_depth;
}

// negamax/minimax algorithm
int AI::negaMax(Board* board, int alpha, int beta, int depth, int turn) 
{
	nodes++;

	// evaluate board
	int boardScore = evaluate(board, turn);
	if (boardScore != 2)
	{
		// if the game ended return the score
		return boardScore;
	}

	// return if depth limit was reached
	if (depth == 0) 
	{
		return 0;
	}

	// save the best score (for the ai) yet
	int bestScore = alpha;

	// loop through grid positions
	for (int x = 0; x < 3; x++) 
	{
		for (int y = 0; y < 3; y++)
		{
			// check if position is free
			if (!board->isTaken(x, y)) 
			{
				// check the score if this cell was replaced
				board->replace(turn, x, y);
				int score = -negaMax(board, -beta, -bestScore, depth - 1, 3 - turn);
				board->replace(0, x, y);

				// if the score is better for the ai
				if (score > bestScore)
				{
					// save the score
					bestScore = score;

					// if this is the original called function, set the best move
					if (depth == depthLimit) 
					{
						bestMove = { x, y };
					}

					// if this move is guaranteed to not be played, stop analyzing this position
					if (bestScore >= beta)
					{
						break;
					}
				}
			}
		}
	}

	// return the best score
	return bestScore;
}

// evaluate board based on turn
int AI::evaluate(Board* board, int eval_ID) 
{
	int win = board->checkWin(false);
	if (win == 0) 
	{
		// if no one won, return 0 if it's a draw, else return 2 for proceed
		return board->isDraw() ? 0 : 2;
	} 
	else 
	{
		// if someone won, return 1 if id won, else return -1
		return win == eval_ID ? 1 : -1;
	}
}

// get the best move from board position
Move AI::getBestMove(Board* board)
{
	nodes = 0;

	// call the minimax/negamax algorithm
	negaMax(board, -2, 2, depthLimit, ID);

	// return the best move
	return bestMove;
}
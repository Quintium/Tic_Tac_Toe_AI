#include "Board.h"
#include "AI.h"

// ai constructor
AI::AI(int AI_ID) 
{
	ID = AI_ID;
}

// negamax/minimax algorithm
int AI::negaMax(Board* board, int alpha, int beta, int ply) 
{
	nodes++;

	// evaluate board
	int boardScore = evaluate(board);
	if (boardScore != UNDECIDED)
	{
		// if the game ended return the score
		return boardScore;
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
				board->makeMove(x, y);
				int score = -negaMax(board, -beta, -bestScore, ply + 1);
				board->unmakeMove(x, y);

				// if the score is better for the ai
				if (score > bestScore)
				{
					// save the score
					bestScore = score;

					// if this is the original called function, set the best move
					if (ply == 0) 
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
int AI::evaluate(Board* board) 
{
	int win = board->checkWin();
	if (win == EMPTY) 
	{
		// return draw or undecided if there's no win
		return board->isDraw() ? DRAW : UNDECIDED;
	} 
	else 
	{
		// if opponent won, return loss
		return LOSS;
	}
}

// get the best move from board position
Move AI::getBestMove(Board* board)
{
	nodes = 0;

	// call the minimax/negamax algorithm
	negaMax(board, -2, 2, 0);

	// return the best move
	return bestMove;
}
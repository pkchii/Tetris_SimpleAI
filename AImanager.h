#pragma once
#include <vector>

#include "board.h"
#include "piece.h"

class AImanager
{
    friend class GameState;
public:
    AImanager();
    ~AImanager();
    void run(Piece piece);
    int getAggregateHeight(Board* ghostBoard);
    int getCompletedline(Board* ghostBoard);
    int getHoles(Board* ghostBoard);
    int getBumpiness(Board* ghostBoard);
    float getScore(Board* ghostBoard);
    void update(Board* ghostBoard);
    void updateGhostBoard(Board* ghostBoard, Piece p);
private:
	std::vector<Board*> ghostBoards;
    Board* currentBoard;

    // optimal set of parameters:
    const float a = -0.510066;
    const float b = 0.760666;
    const float c = -0.35663;
    const float d = -0.184483;

    int optimalCol;
    int optimalRotatiton;
};


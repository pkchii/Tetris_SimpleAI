#include "AImanager.h"

AImanager::AImanager()
{
    optimalCol = 0;
    optimalRotatiton = 0;
    currentBoard = new Board;
}

AImanager::~AImanager()
{
    for (auto i : ghostBoards) {
        delete i;
    }
    delete currentBoard;
}

void AImanager::run(Piece piece)
{
    Piece tempPiece(piece);
    for (int i = 0; i < 4; i++) { //  each tetromino has 4 rotations
        tempPiece.rotation = i;
        for (int j = 0; j < config::playfield_width; j++) {
            Board* tempBoard = new Board;
            tempPiece.c = piece.c + j;
            if (tempBoard->isPositionLegal(tempPiece)) {
                updateGhostBoard(tempBoard, tempPiece);
                update(tempBoard);
                tempBoard->currentPieceRotation = i;
                tempBoard->currentPieceCol = j;
                ghostBoards.push_back(tempBoard);
            }
            else {
                break;
            }
        }
    }
    float highestScore = -9999;
    for (auto i : ghostBoards) {
        if (i->score > highestScore) {
            highestScore = i->score;
            optimalCol = i->currentPieceCol;
            optimalRotatiton = i->currentPieceRotation;
        }
    }
}

int AImanager::getAggregateHeight(Board* ghostBoard)
{
    int total = 0;
    for (int col = 0; col < config::playfield_width; col++)
    {
        for (int row = 0; row < config::playfield_height; row++)
        {
            if (ghostBoard->board_state[row][col] != block_empty) {
                total += (config::playfield_height - row);
                break;
            }
        }
    }
    return total;
}

int AImanager::getCompletedline(Board* ghostBoard)
{
    int count = 0;
    for (int row = 0; row < config::playfield_height; row++)
    {
        bool temp = true;
        for (int col = 0; col < config::playfield_width; col++)
        {
            if (ghostBoard->board_state[row][col] == block_empty) {
                temp = false;
                break;
            }
        }
        if (temp) {
            count++;
        }
    }
    return count;
}

int AImanager::getHoles(Board* ghostBoard)
{
    int count = 0;
    for (int col = 0; col < config::playfield_width; col++)
    {
        bool block = false;
        for (int row = 0; row < config::playfield_height; row++)
        {
            if (ghostBoard->board_state[row][col] != block_empty) {
                block = true;
            }
            else if (ghostBoard->board_state[row][col] == block_empty && block) {
                count++;
            }
        }
    }
    return count;
}

int AImanager::getBumpiness(Board* ghostBoard)
{
    int index[config::playfield_width] = { 0 };
    for (int col = 0; col < config::playfield_width; col++)
    {
        for (int row = 0; row < config::playfield_height; row++)
        {
            if (ghostBoard->board_state[row][col] != block_empty) {
                index[col] = config::playfield_height - row;
                break;
            }
        }
    }
    int result = 0;
    for (int i = 0; i < config::playfield_width - 1; i++) {
        result += abs(index[i] - index[i + 1]);
    }
    return result;
}

float AImanager::getScore(Board* ghostBoard)
{
    // linear function to compute to score
    ghostBoard->score = a * ghostBoard->aggregateHeight + b * ghostBoard->completedLines + c * ghostBoard->holes + d * ghostBoard->bumpiness;
    return ghostBoard->score;
}

void AImanager::update(Board* ghostBoard)
{
    ghostBoard->completedLines = getCompletedline(ghostBoard);
    ghostBoard->aggregateHeight = getAggregateHeight(ghostBoard);
    ghostBoard->holes = getHoles(ghostBoard);
    ghostBoard->bumpiness = getBumpiness(ghostBoard);
    ghostBoard->score = getScore(ghostBoard);
}

void AImanager::updateGhostBoard(Board* ghostBoard, Piece p)
{
    // create ghost board state
    for (int row = 0; row < config::playfield_height; row++)
    {
        for (int col = 0; col < config::playfield_width; col++)
        {
            ghostBoard->board_state[row][col] = currentBoard->board_state[row][col];
        }
    }

    // create ghost piece
    while (ghostBoard->isPositionLegal(p)) {
        p.r++;
    }
    p.r--;

    // store ghost piece to ghost board state
    ghostBoard->storePiece(p);
}

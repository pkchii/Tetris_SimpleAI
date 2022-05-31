#pragma once
#include <queue>
#include <string>

#include "board.h"
#include "inputmanager.h"
#include "piece.h"
#include "texture.h"
#include "AImanager.h"

class GameState
{
public:
    GameState();
    void checkState();                                 // Deletes filled lines and creates a new block.
    void createNewPiece();                             // Create a new random piece
    void drawState();                                  // Draw the board and all pieces after each frame
    void initializeState();                            // Create new board and load necessary textures
    bool isGameOver();                                 // True if the game has ended
    void movePieceDown();
private:
    Board* board;
    Piece currentPiece{ 0, 0 };
    Piece ghostPiece{ 0, 0 };
    Piece nextPiece{ 0, 0 };
    std::queue<Piece> nextPieces;                       // Holds the next 2 pieces

    Texture tetrominoSprites;
    SDL_Rect tetrominoSpriteClips[7];
    Texture playfieldFrame;
    SDL_Rect playfieldFrameClips[4];

    void drawBoard();
    void drawCurrentPiece(Piece p);
    void drawGhostPiece(Piece p);
    void drawNextPiece(Piece p);
    int getRandom(int lower_limit, int upper_limit);   // Return a random number in this range
};


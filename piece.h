#pragma once
class Piece
{
    friend class Board;
    friend class GameState;
    friend class AImanager;
public:
    Piece(int p_piece_type, int p_rotation);       // Initialize a piece with these values
    Piece(const Piece& p);                         // Copy constructor
    int getBlockType(int r_offset, int c_offset);  // Returns the block type for a certain piece, rotation, and position
private:
    int piece_type;                                 // Type of tetromino
    int rotation;                                   // One of four possible rotations
    int r;                                          // Row index
    int c;                                          // Column index
};


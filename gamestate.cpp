#include "gamestate.h"

#include <cstdlib>
#include <ctime>

#include <iostream>

#include <SDL.h>

#include "utilities.h"


extern SDL_Renderer* gRenderer; // Global renderer that handles all the drawing

/*
 * ====================================
 * Public methods start here
 * ====================================
 */

GameState::GameState()
{
    board = new Board;
}

// Stores current block, deletes filled lines and creates a new block.
void GameState::checkState()
{
    board->storePiece(currentPiece);
    board->clearFullLines();
    if (!board->isGameOver())
    {
        createNewPiece();
    }
}

void GameState::createNewPiece()
{
    currentPiece.piece_type = nextPiece.piece_type;
    currentPiece.rotation = nextPiece.rotation;
    currentPiece.r = 0;
    currentPiece.c = 0;
    AImanager* bot = new AImanager;

    // copy current board state
    for (int row = 0; row < config::playfield_height; row++)
    {
        for (int col = 0; col < config::playfield_width; col++)
        {
            bot->currentBoard->board_state[row][col] = board->board_state[row][col];
        }
    }
    bot->run(currentPiece);
    currentPiece.c = bot->optimalCol;
    currentPiece.rotation = bot->optimalRotatiton;
    delete bot;

    for (int i = 0; i < 2; i++)
    {
        currentPiece.r++;
        if (!board->isPositionLegal(currentPiece))
        {
            currentPiece.r--;
        }
    }
    
    nextPiece.piece_type = getRandom(0, 6);
    nextPiece.rotation = 0; // Pieces must always start flat according to the offical Tetris guidelines
}

void GameState::drawState()
{
    drawBoard();
    drawCurrentPiece(currentPiece);
    if (!board->isGameOver() && config::ghost_piece_enabled) drawGhostPiece(currentPiece);
    drawNextPiece(nextPiece);
    Texture* lineCleared = new Texture;
    lineCleared->loadFromText("Lines Cleared: " + std::to_string(board->lineCleared), config::default_text_color);
    lineCleared->render(config::lineCleared_box_x, config::lineCleared_box_y);
    lineCleared->free();
}


void GameState::initializeState()
{
    srand(time(0));
    // Get random first piece
    nextPiece.piece_type = getRandom(0, 6);
    nextPiece.rotation = 0;                 // Pieces must always start flat according to the offical Tetris guidelines
    createNewPiece();
    nextPiece.r = config::next_box_y;
    nextPiece.c = config::next_box_x;

    // Load necessary textures
    tetrominoSprites.loadFromImage("assets/tetrominoSprites.png");
    playfieldFrame.loadFromImage("assets/playfieldFrame.png");


    // Create the right clips sprites
    for (int i = 0; i < 7; i++)
    {
        tetrominoSpriteClips[i].x = 16 * i;
        tetrominoSpriteClips[i].y = 0;
        tetrominoSpriteClips[i].w = 16;
        tetrominoSpriteClips[i].h = 16;
    }
    for (int i = 0; i < 4; i++)
    {
        playfieldFrameClips[i].x = config::frame_sprite_size * i;
        playfieldFrameClips[i].y = 0;
        playfieldFrameClips[i].w = config::frame_sprite_size;
        playfieldFrameClips[i].h = config::frame_sprite_size;
    }
}

bool GameState::isGameOver()
{
    return board->isGameOver();
}

void GameState::movePieceDown()
{
    currentPiece.r++;
    if (!board->isPositionLegal(currentPiece))
    {
        currentPiece.r--;
        checkState();
    }
}

/*
 * ====================================
 * Private methods start here
 * ====================================
 */

void GameState::drawBoard()
{
    for (int i = 0; i < 2 * config::true_playfield_height; i++)
    {
        // Left frame
        playfieldFrame.render(config::width_to_playfield - config::frame_sprite_size, config::height_to_playfield + i * config::frame_sprite_size,
            &playfieldFrameClips[0]);
        // Right frame
        playfieldFrame.render(config::width_to_playfield + config::block_size * config::playfield_width - (config::frame_sprite_size -
            config::frame_width), config::height_to_playfield + i * config::frame_sprite_size, &playfieldFrameClips[0]);
    }
    // Then the 2 corners
    playfieldFrame.render(config::width_to_playfield - config::frame_sprite_size, config::height_to_playfield +
        config::block_size * config::true_playfield_height - (config::frame_sprite_size - config::frame_width), &playfieldFrameClips[2]);
    playfieldFrame.render(config::width_to_playfield + config::block_size * config::playfield_width, config::height_to_playfield +
        config::block_size * config::true_playfield_height - (config::frame_sprite_size - config::frame_width), &playfieldFrameClips[3]);

    for (int i = 0; i < 2 * config::playfield_width; i++)
    {
        // And the bottom frame
        playfieldFrame.render(config::width_to_playfield + i * config::frame_sprite_size, config::height_to_playfield +
            config::block_size * config::true_playfield_height, &playfieldFrameClips[1]);
    }

    // Then draw the placed blocks
    for (int row = 0; row < config::playfield_height; row++)
    {
        for (int col = 0; col < config::playfield_width; col++)
        {
            if (!board->isBlockFree(row, col))
            {
                tetrominoSprites.render(config::width_to_playfield + col * config::block_size, config::height_to_playfield +
                    (row - (config::playfield_height - config::true_playfield_height)) * config::block_size, &tetrominoSpriteClips[board->getTetromino(row, col)]);
            }
        }
    }
}

void GameState::drawCurrentPiece(Piece p)
{
    for (int row = 0; row < config::matrix_blocks; row++)
    {
        for (int col = 0; col < config::matrix_blocks; col++)
        {
            if (p.getBlockType(row, col) != 0)
            {
                tetrominoSprites.render(config::width_to_playfield + (col + p.c) * config::block_size, config::height_to_playfield +
                    (row + p.r - (config::playfield_height - config::true_playfield_height)) * config::block_size, &tetrominoSpriteClips[p.piece_type]);
            }
        }
    }
}

// Draws the ghost piece of the piece given
void GameState::drawGhostPiece(Piece p)
{
    ghostPiece = p;
    while (board->isPositionLegal(ghostPiece))
    {
        ghostPiece.r++;
    }
    ghostPiece.r--;

    tetrominoSprites.setAlphaMode(config::transparency_alpha);  // Change transparency to render the ghost piece

    for (int row = 0; row < config::matrix_blocks; row++)
    {
        for (int col = 0; col < config::matrix_blocks; col++)
        {
            if (ghostPiece.getBlockType(row, col) != 0)
            {
                tetrominoSprites.render(config::width_to_playfield + (col + ghostPiece.c) * config::block_size, config::height_to_playfield +
                    (row + ghostPiece.r - (config::playfield_height - config::true_playfield_height)) * config::block_size, &tetrominoSpriteClips[ghostPiece.piece_type]);
            }
        }
    }

    tetrominoSprites.setAlphaMode(255); // Don't forget to change it back to normal!
}

void GameState::drawNextPiece(Piece p)
{
    Texture* tmp = new Texture;
    tmp->loadFromText("Next Piece", config::default_text_color);
    tmp->render(config::next_box_x - 30, config::next_box_y - 50);
    tmp->free();
    for (int row = 0; row < config::matrix_blocks; row++)
    {
        for (int col = 0; col < config::matrix_blocks; col++)
        {
            if (p.getBlockType(row, col) != 0)
            {
                tetrominoSprites.render(config::next_box_x + col * config::block_size, config::next_box_y + row * config::block_size,
                    &tetrominoSpriteClips[p.piece_type]);
            }
        }
    }
}

int GameState::getRandom(int lower_limit, int upper_limit)
{
    return rand() % (upper_limit - lower_limit + 1) + lower_limit;
}
#pragma once
#include <SDL.h>

namespace config
{
    // Window title
    const char window_title[] = "PixelTetris!";

    // Logical resolution of the game. Most constants are in respect to these two values,
    // so do not change them unless you know what you're doing
    const int logical_window_width = 640;
    const int logical_window_height = 360;

    // Board class
    const int width_to_playfield = 245;                             // in pixels
    const int height_to_playfield = 34;                             // in pixels
    const int block_size = 16;                                      // in pixels
    const int playfield_width = 10;                                 // in blocks
    const int true_playfield_height = 20;                           // in blocks
    const int playfield_height = 22;                                // The playfield+2 rows directly above it for spawning the Tetrominos
    const int frame_width = 6;                                      // Frame that surrounds the playfield; in pixels
    const int frame_sprite_size = 8;                                // Size of each sprite clip for the playfield frame
    const int board_height = 10;                                     // distance from botton where the playfield begins; in pixels
    const int matrix_blocks = 5;                                    // matrix that holds tetromino info; in blocks


    // GameState class
    const int next_box_x = 475;                                     // Horizontal distance from top left corner; in pixels
    const int next_box_y = 70;                                      // Vertical distance from top left corner; in pixels
    const int lineCleared_box_x = 425;
    const int lineCleared_box_y = 200;

    // Constants relating to Tetris
    const int wait_time = 20;                                     // Time in milliseconds
    const bool ghost_piece_enabled = true;                                // Enables or disables the ghost piece to reduce misdrops

    // Visuals
    const SDL_Color default_text_color = { 0x72, 0x63, 0x63, 0xFF };  // Default color for all text
    const Uint8 background_r_light = 0xBD;                          // Background color for light mode
    const Uint8 background_g_light = 0xE6;
    const Uint8 background_b_light = 0xCF;
    const Uint8 transparency_alpha = 100;                           // Default alpha for transparency (with 255 being no transparency)                             
}
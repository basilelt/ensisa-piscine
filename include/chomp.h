/**
 * @file chomp.h
 * @brief Header file for the Chomp game functions.
 *
 * This file contains the function declarations for the Chomp game.
 * The Chomp game is a two-player strategy game where players take turns
 * choosing a cell from a grid and "eating" that cell and all cells to the right
 * and below it. The player forced to eat the top-left cell loses.
 */

#ifndef CHOMP_H
#define CHOMP_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "const.h"

/**
 * @brief Flushes and trims the input string.
 *
 * This function reads a string from the standard input, removes the newline
 * character if present, and flushes any remaining characters in the input buffer.
 *
 * @param str The input string.
 * @param size The size of the input string buffer.
 */
void flush_and_trim_input (char *str, size_t size);

/**
 * @brief Checks if the specified cell is 1 or 0.
 *
 * This function checks if the specified cell in the game table is 1 or 0.
 *
 * @param table The game table.
 * @param row The row of the cell.
 * @param col The column of the cell.
 * @return 1 if the cell is 0, 0 if the cell is 1.
 */
int is_this_cell_1_or_0(int table[ROWS][COLS], int row, int col);

/**
 * @brief Prints the game table.
 *
 * This function prints the current state of the game table, highlighting the
 * specified cell.
 *
 * @param table The game table.
 * @param col The column of the cell to highlight.
 * @param row The row of the cell to highlight.
 */
void print_table (int table[ROWS][COLS], int col, int row);

/**
 * @brief Deletes cells from the game table.
 *
 * This function deletes cells from the specified cell to the right and below it.
 *
 * @param table The game table.
 * @param row The row of the starting cell.
 * @param col The column of the starting cell.
 */
void delete_cells (int table[ROWS][COLS], int row, int col);

/**
 * @brief Calculates the number of cells to delete.
 *
 * This function calculates the number of cells that would be deleted if the
 * specified cell is chosen.
 *
 * @param table The game table.
 * @param row The row of the starting cell.
 * @param col The column of the starting cell.
 * @return The number of cells to delete.
 */
int calculate_num_to_delete (int table[ROWS][COLS], int row, int col);

/**
 * @brief Calculates the possible moves.
 *
 * This function calculates the possible moves based on the current state of the
 * game table. It returns a pointer to a static array of possible moves, where each
 * move is represented by a pair of integers indicating the row and column.
 *
 * @param table The game table.
 * @return A pointer to a static array of pairs of integers representing the possible moves.
 */
int (*calculate_possibility(int table[ROWS][COLS]))[2];

/**
 * @brief Plays the game.
 *
 * This function contains the main game loop, where players take turns choosing
 * cells until the game ends.
 *
 * @param table The game table.
 * @param flag A pointer to a boolean flag indicating whether the game is still ongoing.
 */
void play_game (int table[ROWS][COLS], bool *flag);

/**
 * @brief Initializes the game table.
 *
 * This function initializes the game table by setting all cells to 1.
 *
 * @param table The game table.
 */
void init_table (int table[ROWS][COLS]);

#endif /* CHOMP_H */

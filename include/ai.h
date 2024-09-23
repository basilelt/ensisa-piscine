#ifndef AI_H
#define AI_H

#include <stdbool.h>


// Function declarations

/**
 * @brief Calculate the number of cells in a row.
 * 
 * This function calculates the number of cells in the specified row.
 * 
 * @param table A 2D array representing the game table.
 * @param row The specified row.
 * @return The number of cells in the specified row.
 */
int number_of_cells_in_row(int table[ROWS][COLS], int row);

/**
 * @brief Calculate the number of cells in a column.
 * 
 * This function calculates the number of cells in the specified column.
 * 
 * @param table A 2D array representing the game table.
 * @param col The specified column.
 * @return The number of cells in the specified column.
 */
int number_of_cells_in_col(int table[ROWS][COLS], int col);

/**
 * @brief Calculate the number of cells in a 5x5 square.
 * 
 * This function calculates the number of cells in a 5x5 square 
 * with his top-left corner located by the specified row and column.
 * 
 * @param table A 2D array representing the game table.
 * @param row The specified row.
 * @param col The specified column.
 * @return The number of cells in the square.
 */
int number_of_cells_in_square(int table[ROWS][COLS], int row, int col);

/**
 * @brief Detects if the table has a row line pattern.
 * 
 * This function checks if the table has a row line pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool row_line_pattern(int table[ROWS][COLS]);

/**
 * @brief Detects if the table has a column line pattern.
 * 
 * This function checks if the table has a column line pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool col_line_pattern(int table[ROWS][COLS]);

/**
 * @brief Detects if the table has a square pattern.
 * 
 * This function checks if the table has a square pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool square_pattern(int table[ROWS][COLS]);

/**
 * @brief Detects if the table has a row rectangle pattern.
 * 
 * This function checks if the table has a row rectangle pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool row_rectangle_pattern(int table[ROWS][COLS]);

/**
 * @brief Detects if the table has a column rectangle pattern.
 * 
 * This function checks if the table has a column rectangle pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool col_rectangle_pattern(int table[ROWS][COLS]);

/**
 * @brief Detects if the table has a corner pattern.
 * 
 * This function checks if the table has a corner pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool corner_pattern(int table[ROWS][COLS]);

/**
 * @brief Detects if the table has a gun pattern.
 * 
 * This function checks if the table has a gun pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool gun_pattern(int table[ROWS][COLS]);

/**
 * @brief Detects the current pattern in the table.
 * 
 * This function checks for various patterns in the table and sets the current pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if a pattern is detected, otherwise false.
 */
bool detect_pattern(int table[ROWS][COLS]);

/**
 * @brief Returns the move coordinates based on the detected pattern.
 * 
 * This function returns the move coordinates based on the detected pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return The move coordinates.
 */
int pattern_move(int table[ROWS][COLS]);

int row_line_strategy();

int col_line_strategy();

int square_strategy();

int row_rectangle_strategy(int table[ROWS][COLS]);

int col_rectangle_strategy(int table[ROWS][COLS]);

int corner_strategy(int table[ROWS][COLS]);

int gun_strategy(int table[ROWS][COLS]);

#endif // AI_H
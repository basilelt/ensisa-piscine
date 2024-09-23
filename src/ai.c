/**
 * @file ai.c
 * @brief Implementation of the ai's behavior in the chomp game.
 * 
 * This file contains the implementation of the ai's behavior which is the
 * ability to see patterns in the end game in order to react with the perfect
 * move. In other cases it makes a random move or take the last square if
 * there is no choice.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chomp.h"
#include "const.h"

/**
 * @brief Calculate the number of cells in a row.
 * 
 * This function calculates the number of cells in the specified row.
 * 
 * @param table A 2D array representing the game table.
 * @param row The specified row.
 * @return The number of cells in the specified row.
 */
int
number_of_cells_in_row (int table[ROWS][COLS], int row)
{
    int num_cell_in_row = 0;
    for (int i = 0; i < COLS; i++) {
        if (table[row][i] == 1) {
            num_cell_in_row++;
        } else {
            break;
        }
    }
    return num_cell_in_row;
}

/**
 * @brief Calculate the number of cells in a column.
 * 
 * This function calculates the number of cells in the specified column.
 * 
 * @param table A 2D array representing the game table.
 * @param col The specified column.
 * @return The number of cells in the specified column.
 */
int
number_of_cells_in_col (int table[ROWS][COLS], int col)
{
    int num_cell_in_col = 0;
    for (int i = 0; i < ROWS; i++) {
        if (table[i][col] == 1) {
            num_cell_in_col++;
        } else {
            break;
        }
    }
    return num_cell_in_col;
}

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
int
number_of_cells_in_square (int table[ROWS][COLS], int row, int col)
{
    int num_cell_in_square = 0;
    for (int i = row; i < row + 5; i++) {
        for (int j = col; j < col + 5; j++) {
            if (table[i][j] == 1) {
                num_cell_in_square++;
            }
        }
    }
    return num_cell_in_square;
}

//pattern recognition functions

/**
 * @brief Detects if the table has a row line pattern.
 * 
 * This function checks if the table has a row line pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool
row_line_pattern (int table[ROWS][COLS])
{
    int num_cell_in_row = number_of_cells_in_row(table, 0);
    int num_cell_in_col = number_of_cells_in_col(table, 0);
    if (num_cell_in_col == 1 && num_cell_in_row <= 6 && num_cell_in_row > 1) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Detects if the table has a column line pattern.
 * 
 * This function checks if the table has a column line pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool
col_line_pattern (int table[ROWS][COLS])
{
    int num_cell_in_row = number_of_cells_in_row(table, 0);
    int num_cell_in_col = number_of_cells_in_col(table, 0);
    if (num_cell_in_row == 1 && num_cell_in_col <= 6 && num_cell_in_col > 1) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Detects if the table has a square pattern.
 * 
 * This function checks if the table has a square pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool
square_pattern (int table[ROWS][COLS])
{
    int num_cell_in_row = number_of_cells_in_row(table, 0);
    int num_cell_in_col = number_of_cells_in_col(table, 0);
    if (num_cell_in_col == num_cell_in_row && num_cell_in_col <= 3 && table[1][1] == 1) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Detects if the table has a row rectangle pattern.
 * 
 * This function checks if the table has a row rectangle pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool
row_rectangle_pattern (int table[ROWS][COLS])
{
    int num_cell_in_col = number_of_cells_in_col(table, 0);
    if (num_cell_in_col == 2 && abs(number_of_cells_in_row(table, 0) - number_of_cells_in_row(table, 1)) != 1 && abs(number_of_cells_in_row(table, 0) - number_of_cells_in_row(table, 1)) <= 6) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Detects if the table has a column rectangle pattern.
 * 
 * This function checks if the table has a column rectangle pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool
col_rectangle_pattern (int table[ROWS][COLS])
{
    int num_cell_in_row = number_of_cells_in_row(table, 0);

    if (num_cell_in_row == 2 && abs(number_of_cells_in_col(table, 0) - number_of_cells_in_col(table, 1)) != 1 && abs(number_of_cells_in_col(table, 0) - number_of_cells_in_col(table, 1)) <= 6) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Detects if the table has a corner pattern.
 * 
 * This function checks if the table has a corner pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool
corner_pattern (int table[ROWS][COLS])
{
    int num_cell_in_row = number_of_cells_in_row(table, 0);
    int num_cell_in_col = number_of_cells_in_col(table, 0);

    if (table[1][1] == 0 && num_cell_in_col != num_cell_in_row && num_cell_in_col != 1 && num_cell_in_row != 1 && abs(num_cell_in_col - num_cell_in_row) <= 5) {
        return true;
    } else {
        return false;
    }
}

/**
 * @brief Detects if the table has a gun pattern.
 * 
 * This function checks if the table has a gun pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if the pattern is detected, otherwise false.
 */
bool
gun_pattern (int table[ROWS][COLS])
{
    int num_cell_in_row = number_of_cells_in_row(table, 6);
    int num_cell_in_col = number_of_cells_in_col(table, 6);
    int num_cell_in_square = number_of_cells_in_square(table, 1, 1);
    
    if (num_cell_in_square > 0 && num_cell_in_square <= 5 && num_cell_in_col <= 1 && num_cell_in_row <= 1 && abs(number_of_cells_in_col(table, 0) - number_of_cells_in_row(table, 0)) <= 3) {
        return true;
    } else {
        return false;
    }
}

//100% winning strategy functions

/**
 * @brief Strategy for row line pattern.
 * 
 * This function returns the move coordinates for the row line pattern.
 * 
 * @return The move coordinates.
 */
int
row_line_strategy ()
{
    return 0 * COLS + 1;
}

/**
 * @brief Strategy for column line pattern.
 * 
 * This function returns the move coordinates for the column line pattern.
 * 
 * @return The move coordinates.
 */
int
col_line_strategy ()
{
    return 1 * COLS + 0;
}

/**
 * @brief Strategy for square pattern.
 * 
 * This function returns the move coordinates for the square pattern.
 * 
 * @return The move coordinates.
 */
int
square_strategy ()
{
    return 1 * COLS + 1;
}

/**
 * @brief Strategy for row rectangle pattern.
 * 
 * This function returns the move coordinates for the row rectangle pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return The move coordinates.
 */
int
row_rectangle_strategy (int table[ROWS][COLS])
{
    if (number_of_cells_in_row(table, 0) == number_of_cells_in_row(table, 1)) {
        return 1 * COLS + (number_of_cells_in_row(table, 0) - 1);
    } else {
        return 0 * COLS + (number_of_cells_in_row(table, 1) + 1);
    }
}

/**
 * @brief Strategy for column rectangle pattern.
 * 
 * This function returns the move coordinates for the column rectangle pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return The move coordinates.
 */
int
col_rectangle_strategy (int table[ROWS][COLS])
{
    if (number_of_cells_in_col(table, 0) == number_of_cells_in_col(table, 1)) {
        return (number_of_cells_in_col(table, 0) - 1) * COLS + 1;
    } else {
        return (number_of_cells_in_col(table, 1) + 1) * COLS + 0;
    }
}

/**
 * @brief Strategy for corner pattern.
 * 
 * This function returns the move coordinates for the corner pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return The move coordinates.
 */
int
corner_strategy(int table[ROWS][COLS])
{
    if (number_of_cells_in_col(table, 0) > number_of_cells_in_row(table, 0)) {
        return number_of_cells_in_row(table, 0) * COLS + 0;
    } else {
        return 0 * COLS + number_of_cells_in_col(table, 0);
    }
}

/**
 * @brief Strategy for gun pattern.
 * 
 * This function returns the move coordinates for the gun pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return The move coordinates.
 */
int
gun_strategy (int table[ROWS][COLS])
{
    if (number_of_cells_in_col(table, 0) == number_of_cells_in_row(table, 0)) {
        return 1 * COLS + 1;
    } else if(number_of_cells_in_col(table, 0) > number_of_cells_in_row(table, 0)) {
        if(table[number_of_cells_in_row(table, 0) - 1][1] != 1) {
            return (number_of_cells_in_row(table, 0) - 1) * COLS + 0;
        } else {
            return (number_of_cells_in_row(table, 0) - 1) * COLS + 1;
        }
    } else {
        if(table[1][number_of_cells_in_col(table, 0) - 1] != 1) {
            return 0 * COLS + number_of_cells_in_col(table, 0) - 1;
        } else {
            return 1 * COLS + number_of_cells_in_col(table, 0) - 1;
        }
    }
}

int current_pattern = 0;
/**
 * @brief Detects the current pattern in the table.
 * 
 * This function checks for various patterns in the table and sets the current pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return True if a pattern is detected, otherwise false.
 */
bool
detect_pattern (int table[ROWS][COLS])
{
    if (row_line_pattern(table)) {
        current_pattern = 1;
        return true;
    } else if (col_line_pattern(table)) {
        current_pattern = 2;
        return true;
    } else if (square_pattern(table)) {
        current_pattern = 3;
        return true;
    } else if (row_rectangle_pattern(table)) {
        current_pattern = 4;
        return true;
    } else if (col_rectangle_pattern(table)) {
        current_pattern = 5;
        return true;
    } else if (corner_pattern(table)) {
        current_pattern = 6;
        return true;
    } else if (gun_pattern(table)) {
        current_pattern = 7;
        return true;
    } else {
        return false;
    }
}
/**
 * @brief Returns the move coordinates based on the detected pattern.
 * 
 * This function returns the move coordinates based on the detected pattern.
 * 
 * @param table A 2D array representing the game table.
 * @return The move coordinates.
 */
int
pattern_move (int table[ROWS][COLS])
{
    int coordinates=0;
    if (current_pattern == 1) {
        coordinates = row_line_strategy();
    } else if (current_pattern == 2) {
        coordinates = col_line_strategy();
    } else if (current_pattern == 3) {
        coordinates = square_strategy();
    } else if (current_pattern == 4) {
        coordinates = row_rectangle_strategy(table);
    } else if (current_pattern == 5) {
        coordinates = col_rectangle_strategy(table);
    } else if (current_pattern == 6) {
        coordinates = corner_strategy(table);
    } else if (current_pattern == 7) {
        coordinates = gun_strategy(table);
    }

    if (coordinates != 0) {
        return coordinates;
    } else {
        printf("big error");
        return 0;
    }
}
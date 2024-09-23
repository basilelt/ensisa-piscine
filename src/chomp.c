/**
 * @file chomp.c
 * @brief Implementation file for the Chomp game functions.
 *
 * This file contains the function definitions for the Chomp game.
 * The Chomp game is a two-player strategy game where players take turns
 * choosing a cell from a grid and "eating" that cell and all cells to the right
 * and below it. The player forced to eat the top-left cell loses.
 */

#include "chomp.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "const.h"

const char *translate[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I"};

/**
 * @brief Flushes and trims the input string.
 *
 * This function reads a string from the standard input, removes the newline
 * character if present, and flushes any remaining characters in the input buffer.
 *
 * @param str The input string.
 * @param size The size of the input string buffer.
 */
void
flush_and_trim_input (char *str, size_t size)
{
    if (fgets(str, size, stdin) != NULL) {
        if (strchr(str, '\n') == NULL) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF); // Flush remaining characters
        } else {
            size_t len = strlen(str);
            if (len > 0 && str[len - 1] == '\n') {
                str[len - 1] = '\0'; // Remove newline character
            }
        }
    } else {
        str[0] = '\0'; // Handle input error
    }
}

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
int
is_this_cell_1_or_0 (int table[ROWS][COLS], int row, int col)
{
    if (table[row][col] == 0) {
        return 1; // Cell is 0
    } else {
        return 0; // Cell is 1
    }
}

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
void
print_table (int table[ROWS][COLS], int col, int row)
{
    printf("\n\033[1;34m    A    B    C    D    E    F    G    H    I\033[0m\n");
    for (int i = 0; i < ROWS; i++) {
        printf("\033[1;34m%d \033[0m", i + 1);
        for (int j = 0; j < COLS; j++) {
            if (col == j && row == i) {
                printf(" \033[1;33;43;1m   \033[0m "); // Highlight selected cell
            } else {
                if (table[i][j] == 1) {
                    printf(" \033[1;32;42;1m   \033[0m "); // Print cell as active
                } else {
                    printf(" \033[1;30;47;1m   \033[0m "); // Print cell as deleted
                }
            }
        }
        printf("\n\n");
    }
}
/* show the matrix
void
print_table (int table[ROWS][COLS], int col, int row)
{
    printf ("\n    A B C D E F G H I\n");
    for (int i = 0; i < ROWS; i++) {
        printf ("%d ", i + 1);
        for (int j = 0; j < COLS; j++) {
            if (col != -1 && row != -1 && col == j && row == i) {
                printf ("[%d] ", table[i][j]);
            } else {
                printf ("%d ", table[i][j]);
            }
        }
        printf ("\n");
    }
}
*/

/**
 * @brief Deletes cells from the game table.
 *
 * This function deletes cells from the specified cell to the right and below it.
 *
 * @param table The game table.
 * @param row The row of the starting cell.
 * @param col The column of the starting cell.
 */
void
delete_cells (int table[ROWS][COLS], int row, int col)
{
    int tmp_col = COLS;
    for (int i = row; i < ROWS; i++) {
        for (int j = col; j < tmp_col; j++) {
            if (table[i][j] == 0) {
                tmp_col = j; // Stop deleting if a cell is already deleted
                break;
            }
            table[i][j] = 0; // Delete cell
        }
    }
}

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
int
calculate_num_to_delete (int table[ROWS][COLS], int row, int col)
{
    int tmp_col = COLS;
    int num_cell_to_delete = 0;
    for (int i = row; i < ROWS; i++) {
        for (int j = col; j < tmp_col; j++) {
            if (table[i][j] == 1) {
                num_cell_to_delete++; // Count cells to delete
            } else {
                if (i == 0) {
                    break; // Stop if top row is reached
                }
                tmp_col = j; // Update column limit
                break;
            }
        }
    }
    return num_cell_to_delete;
}

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
int
(*calculate_possibility (int table[ROWS][COLS]))[2]
{
    static int possible_moves[ROWS * COLS][2];
    int move_index = 0;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (table[i][j] == 1 && calculate_num_to_delete(table, i, j) <= NUM_MAX_TO_DELETE) {
                possible_moves[move_index][0] = i;
                possible_moves[move_index][1] = j;
                move_index++;
            }
        }
    }
    for (int i = move_index; i < ROWS * COLS; i++) {
        possible_moves[i][0] = -1; // Mark remaining moves as invalid
        possible_moves[i][1] = -1;
    }

    return possible_moves;
}

/**
 * @brief Plays the game.
 *
 * This function contains the main game loop, where players take turns choosing
 * cells until the game ends.
 *
 * @param table The game table.
 * @param flag A pointer to a boolean flag indicating whether the game is still ongoing.
 */
void
play_game (int table[ROWS][COLS], bool *flag)
{
    int row = -1, col = -1;
    int row_tmp = -1, col_tmp = -1;
    char user_input[MAX_INPUT_SIZE + 1];

    while (*flag) {  // Main game loop
        int col_ok = 0;
        print_table(table, col, row); // Print current game state
        calculate_possibility(table); // Calculate possible moves
        if (tour_number % 2 == 0) {
            printf("\033[1m%s, it's your turn\033[0m\n", player1);
        } else {
            printf("\033[1m%s, it's your turn\033[0m\n", player2);
        }
        printf("Enter cell id (between A-I and 1-7): ");
        flush_and_trim_input(user_input, sizeof(user_input)); // Get user input
        if (strlen(user_input) != 2) {
            printf("\033[1;31m\nInvalid input.\nInput must be exactly two characters.\033[0m\n");
            continue;
        }
        user_input[0] = toupper(user_input[0]); // Convert column to uppercase

        for (int j = 0; j < COLS; j++) {
            if (user_input[0] == translate[j][0]) {
                col_tmp = j;
                col_ok = 1;
                break;
            }
        }
        row_tmp = user_input[1] - '0' - 1; // Convert row to integer

        if (col_ok == 0 || row_tmp < 0 || row_tmp >= ROWS) {
            printf("\033[1;31m\nInvalid input.\n%s is not in the range A1 to I7.\033[0m\n", user_input);
            continue;
        }
        if (is_this_cell_1_or_0(table, row_tmp, col_tmp) == 1) {
            printf("\n\033[1;31mThe cell %s is already deleted please try again\033[0m\n", user_input);
            continue;
        }

        if (calculate_num_to_delete(table, row_tmp, col_tmp) <= NUM_MAX_TO_DELETE) {
            row = row_tmp;
            col = col_tmp;
            delete_cells(table, row, col); // Delete selected cells
            tour_number++;
        } else {
            printf("\033[1;31m\nInvalid input.\nYou can delete at most 5 cells.\033[0m\n");
        }

        if (table[0][0] == 0) { // Check if game is over
            *flag = false;
            player = (tour_number % 2 == 0) ? 2 : 1; // Determine winner
        }
    }
}

/**
 * @brief Initializes the game table.
 *
 * This function initializes the game table by setting all cells to 1.
 *
 * @param table The game table.
 */
void init_table (int table[ROWS][COLS])
{
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            table[i][j] = 1; // Set all cells to active
        }
    }
}

/**
 * @file test_functions.c
 * @brief This file contains the implementation of various test functions for player names, scores, and game grid operations.
 * 
 * The tests include functions to initialize player names, compare player names, print player scores, delete cells, and initialize the game table.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_user.h"
#include "chomp.h"
#include "const.h"
#include "ai.h"


/**
 * @brief Test function to initialize and verify player names and scores.
 * 
 * This function initializes player names, verifies that they are correctly set, and prints the player scores.
 *
 */
void 
testPlayersNameAndScores() 
{

    /// initialization player name
    strcpy(player1, "Toto");
    strcpy(player2, "Momo");

    /// Test player name
    if (strcmp(player1, "Toto") == 0) {
        printf("Player 1 name is correctly set to Toto.\n");
    } 
    else {
        printf("Player 1 name is not correctly set.\n");
    }

    if (strcmp(player2, "Momo") == 0) {
        printf("Player 2 name is correctly set to Momo.\n");
    } 
    else {
        printf("Player 2 name is not correctly set.\n");
    }

    /// test scores
    player = 1; /// player 2 win
    printf("%s win \n",player2);
    print_score();

    score_player_1 = 0;
    score_player_2 = 0;

    player = 2; /// player 1 win
    printf("%s win \n",player1);
    print_score();

}


/**
 * @brief Test function to delete cells in the game grid.
 * 
 * This function tests the deletion of cells in the game grid and verifies the correctness of the operation.
 */
void 
testDeleteCells() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    int test_row = 5, test_col = 7;
    
    int num_cell_to_delete = calculate_num_to_delete(table, test_row, test_col);
    
    /// check cells count removed
    if (num_cell_to_delete == 4) { 
        printf("It's good, number cells to delete is correct \n");
    } else {
        printf("number cells to delete is not correct, result: %d\n", num_cell_to_delete);
    }


    delete_cells(table, test_row, test_col);

    /// checks cells (4,5), right and under are deleted.
    if (table[test_row][test_col] == 0 && table[test_row + 1][test_col] == 0) {
        printf("It's good, Cells (%d, %d), right and under are deleted.\n", test_row, test_col);
    } else {
        printf("Cells (%d, %d), right and under are not deleted.\n", test_row, test_col);
    }    
}

/**
 * @brief Test function to delete cells in the game grid when the grid is started.
 * 
 * This function tests the deletion of cells in the game grid when the grid is started and verifies the correctness of the operation.
 */
void 
testDeleteCellsBis()
{

    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int test_row = 3, test_col = 3;
    
    int num_cell_to_delete1 = calculate_num_to_delete(table, test_row, test_col);
    
    /// check cells count removed
    if (num_cell_to_delete1 == 1) { 
        printf("It's good, number cells to delete is correct \n");
    } else {
        printf("number cells to delete is not correct, result: %d\n", num_cell_to_delete1);
    }


    delete_cells(table, test_row, test_col);

    /// checks cells (4,5), right and under are deleted.
    if (table[test_row][test_col] == 0 && table[test_row + 1][test_col] == 0) {
        printf("It's good, Cells (%d, %d), right and under are deleted.\n", test_row, test_col);
    } else {
        printf("Cells (%d, %d), right and under are not deleted.\n", test_row, test_col);
    }
}

/**
 * @brief Test function to delete cells in the game grid when the cells are already empty.
 * 
 * This function tests the deletion of cells in the game grid when the cells are already empty and verifies the correctness of the operation.
 */
void 
testDeleteCellsFalse()
{

    int table[ROWS][COLS] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    int test_row = 1, test_col = 1;
    
    int num_cell_to_delete1 = calculate_num_to_delete(table, test_row, test_col);
    
    /// check cells count removed
    if (num_cell_to_delete1 == 1) { 
        printf("Failed, number cells to delete is correct \n");
    } else {
        printf("number cells to delete is not correct, result: %d delete OK\n", num_cell_to_delete1);
    }
    
}

/**
 * @brief Test function to handle the end game scenario.
 * 
 * This function tests the end game scenario and verifies the correctness of the operation.
 */
void 
endGame()
{

    int table[ROWS][COLS] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };
    int test_row = 0, test_col = 0;
    
    delete_cells(table,test_row,test_col);

    if (table[test_row][test_col] == 0)
    {
        printf("Table is Empty. End of the game\n");
    }
}

/**
 * @brief Test function to initialize the game table.
 * 
 * This function tests the initialization of the game table and verifies the correctness of the operation.
 */
void 
testInitTable()
{

    int table[ROWS][COLS] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    init_table(table);
    if (table[6][8] == 1){
        printf("table is well initialized\n");
    }
    else printf("failed initialization\n");
    
}

bool 
test_num_cell_row() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (number_of_cells_in_row(table, 0) == 6) {
        printf("the count of cells in a row is correct\n");
        return true;
    } else {
        printf("the count of cells in a row failed\n");
        return false;
    }
}

bool 
test_num_cell_col() 
{
    int table[ROWS][COLS] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (number_of_cells_in_col(table, 0) == 6) {
        printf("the count of cells in a col is correct\n");
        return true;
    } else {
        printf("the count of cells in a col failed\n");
        return false;
    }
}

bool 
test_num_cell_square() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (number_of_cells_in_square(table, 0, 0) == 13) {
        printf("the count of cells in a 5x5 square is correct\n");
        return true;
    } else {
        printf("the count of cells in a 5x5 square failed\n");
        return false;
    }
}

bool 
test_row_line_pattern() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (row_line_pattern(table)) {
        printf("the row line pattern is correctly recognized\n");
        return true;
    } else {
        printf("the row line pattern is not correctly recognized\n");
        return false;
    }
}

bool 
test_col_line_pattern() 
{
    int table[ROWS][COLS] = {
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (col_line_pattern(table)) {
        printf("the col line pattern is correctly recognized\n");
        return true;
    } else {
        printf("the col line pattern is not correctly recognized\n");
        return false;
    }
}

bool 
test_square_pattern() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 1, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (square_pattern(table)) {
        printf("the square pattern is correctly recognized\n");
        return true;
    } else {
        printf("the square pattern is not correctly recognized\n");
        return false;
    }
}

bool 
test_row_rectangle_pattern() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (row_rectangle_pattern(table)) {
        printf("the row rectangle pattern is correctly recognized\n");
        return true;
    } else {
        printf("the row rectangle pattern is not correctly recognized\n");
        return false;
    }
}

bool 
test_col_rectangle_pattern() 
{
    int table[ROWS][COLS] = {
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (col_rectangle_pattern(table)) {
        printf("the col rectangle pattern is correctly recognized\n");
        return true;
    } else {
        printf("the col rectangle pattern is not correctly recognized\n");
        return false;
    }
}

bool 
test_corner_pattern() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (corner_pattern(table)) {
        printf("the corner pattern is correctly recognized\n");
        return true;
    } else {
        printf("the corner pattern is not correctly recognized\n");
        return false;
    }
}

bool 
test_gun_pattern() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (gun_pattern(table)) {
        printf("the gun pattern is correctly recognized\n");
        return true;
    } else {
        printf("the gun pattern is not correctly recognized\n");
        return false;
    }
}

bool 
test_row_rectangle_strategy() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (row_rectangle_strategy(table) == 5) {
        printf("the row rectangle strategy is correct\n");
        return true;
    } else {
        printf("the row rectangle strategy is not correct\n");
        return false;
    }
}

bool 
test_col_rectangle_strategy() 
{
    int table[ROWS][COLS] = {
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (col_rectangle_strategy(table) == 45) {
        printf("the col rectangle strategy is correct\n");
        return true;
    } else {
        printf("the col rectangle strategy is not correct\n");
        return false;
    }
}

bool 
test_corner_strategy() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (corner_strategy(table) == 4) {
        printf("the corner strategy is correct\n");
        return true;
    } else {
        printf("the corner strategy is not correct\n");
        return false;
    }
}

bool 
test_gun_strategy() 
{
    int table[ROWS][COLS] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0}
    };

    if (gun_strategy(table) == 12) {
        printf("the gun strategy is correct\n");
        return true;
    } else {
        printf("the gun strategy is not correct\n");
        return false;
    }
}
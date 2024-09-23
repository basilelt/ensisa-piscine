/**
 * @file test_runtime.c
 * @brief This file contains the implementation of the test_runtime function, which measures the runtime performance of a game simulation.
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "chomp.h"
#include "const.h"

/**
 * @brief Measures the runtime performance of a game simulation.
 *
 * This function simulates a number of games and measures the time taken for each move and the calculation of possible moves.
 * It prints out the total number of games, total move actions, total next move calculations, average move action runtime,
 * average next move possible runtime, and the total runtime for all games.
 *
 * @param num_games The number of games to simulate.
 */
void
test_runtime(int num_games) {
    double total_move_time = 0.0;
    double total_next_move_time = 0.0;
    int total_move_count = 0;
    int total_next_move_count = 0;

    srand(time(NULL)); /// Seed the random number generator

    clock_t total_start = clock(); /// Start measuring total runtime

    for (int game = 0; game < num_games; game++) {

        clock_t start, end;
        double cpu_time_used;
        int move_count = 0;
        int next_move_count = 0;

        int table[ROWS][COLS];
        init_table(table); /// Initialize the game table

        while (table[0][0] != 0) {

            /// Measure runtime for next move possible
            start = clock();
            int (*possible_moves)[2] = calculate_possibility(table);

            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
            total_next_move_time += cpu_time_used;
            next_move_count++;

            /// Select a random move from the possible moves
            int move_index = 0;
            while (possible_moves[move_index][0] != -1 || possible_moves[move_index][1] != -1) {
                move_index++;
            }

            if (move_index > 0) {
                int random_index = rand() % move_index;
                int row = possible_moves[random_index][0];
                int col = possible_moves[random_index][1];

                /// Perform the selected move
                start = clock();

                delete_cells(table, row, col);

                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                total_move_time += cpu_time_used;
                move_count++;
            }
        }
        total_move_count += move_count;
        total_next_move_count += next_move_count;
    }

    clock_t total_end = clock(); /// End measuring total runtime
    double total_runtime = ((double) (total_end - total_start)) / CLOCKS_PER_SEC;

    printf("Total games: %d\n", num_games);
    printf("Total move actions: %d\n", total_move_count);
    printf("Total next move calculations: %d\n", total_next_move_count);
    printf("Average move action runtime: %f seconds\n", total_move_time / total_move_count);
    printf("Average next move possible runtime: %f seconds\n", total_next_move_time / total_next_move_count);
    printf("Total runtime for all games: %f seconds\n", total_runtime);
}
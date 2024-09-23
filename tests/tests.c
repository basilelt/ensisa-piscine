#include "test_functions.c"
#include "test_network.c"
#include "test_runtime.c"
#include <stdbool.h>
 
void run_test(bool (*f)(), int *successes, int *runs) {
    if (f()) (*successes)++;
    (*runs)++;
}

int main() {
    int test_count = 0, successes = 0;
    printf("test runtime\n");
    test_runtime(1000);

    printf("\ntest players and scores\n");
    testPlayersNameAndScores();

    printf("\n test full grid: check cells delete\n");
    testDeleteCells();
    printf("\n");

    printf("\n test grid started: check cells delete\n");
    testDeleteCellsBis();
    printf("\n");

    printf("\n Delete cells already empty : check cells delete\n");
    testDeleteCellsFalse();
    printf("\n");

    printf("Last shot : cell delete\n");
    endGame();
    printf("\n");

    testInitTable();
    printf("\n");

    test_receive_move();
    printf("\n");

    printf("testing ai functions...\n");
    run_test(test_num_cell_row, &successes, &test_count);
    run_test(test_num_cell_col, &successes, &test_count);
    run_test(test_num_cell_square, &successes, &test_count);
    run_test(test_row_line_pattern, &successes, &test_count);
    run_test(test_col_line_pattern, &successes, &test_count);
    run_test(test_square_pattern, &successes, &test_count);
    run_test(test_row_rectangle_pattern, &successes, &test_count);
    run_test(test_col_rectangle_pattern, &successes, &test_count);
    run_test(test_gun_pattern, &successes, &test_count);
    run_test(test_row_rectangle_strategy, &successes, &test_count);
    run_test(test_col_rectangle_strategy, &successes, &test_count);
    run_test(test_corner_strategy, &successes, &test_count);
    run_test(test_gun_strategy, &successes, &test_count);
    int failures = test_count - successes;
    printf("%d failures out of %d tests\n", failures, test_count);
    return successes;
}

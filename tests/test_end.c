#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gestion_user.h"
#include "chomp.h"
#include "const.h"


void end()
{
    char user="A1 \n";
// board initialisation
    int table[ROWS][COLS];
    init_table(table);

// Simulation of the game by calling  play_game()
    printf("Test's excution\n");
    bool game_over_flag = true;
    play_game(table, &game_over_flag);

//Checking if the cell A1 is deleted and if the game is finished
    if (table[0][0] == 0) {
        printf("A1 was selected, thus game is finished.\n");
    } else {
        printf("ERROR, the game did not end by selecting A1.\n");
    }
}

int main () 
{
    end();
    return 0;
}
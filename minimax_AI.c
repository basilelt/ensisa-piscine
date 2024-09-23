/* 

******************************************************************

The code will determine the behavior of CPU (AI)

The only AI algorithm used here is Minimax

******************************************************************


*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "chomp.h"
#include "const.h"
#include <limits.h> //a library that defines INT_MIN and INT_MAX for Minimax


/* 
shadow_clone function will make an exact copy of the board when it's AI's turn

WHY ? Well it will be usuful when the AI has to make its move, so instead of missing around

with the real board, the AI can test its strategy with a copy

bfo(before) is the board during t and aft(after) is the one during t+dt

aft is the board that will be tested by the CPU (AI) 

*/
void shadow_clone(int bfo[ROWS][COLS], int aft[ROWS][COLS]) 
{
    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            bfo[i][j] = aft[i][j];
        }
    }
}

int game_over0(int board[ROWS][COLS]) 
{
    return board[0][0] == 0;
}

/* 
The code below is for the minimax algorithm

The variable whos_turn defines the current player (Human or AI)

opt_score is the optimal score

The depth is going to limit the AI to certain number of possibilities instead of crashing out with tons of cases

Depth will be limited to only 5. 

Q. "bUt WhY oNlY 5 ?"  A."BECAUSE I SAID SO"

During Maximisation's phase, opt_score is set to INT_MIN

During Minimisation's phase, opt_score is set to INT_MAX

*/ 

int minimax(int table[ROWS][COLS],int depth ,int whos_turn) 
{
    if (game_over0(table)) 
    {
        return whos_turn ? -1 : 1;  
    }
    if (depth == 0) // if depth = 0, the match is finished. Q."BuT wHy ?"  A."bc no more possiblities"
    {
        return 0;  
    }
    int opt_score;
    if (whos_turn) 
    {
        opt_score = INT_MIN;           //it's the turn of max player
        for (int i = 0; i < ROWS; i++)            
        {
            for (int j = 0; j < COLS; j++) 
            {
                if (table[i][j] == 1)              //Running through the board to find a valid square to destroy
                {  
                    int board_t[ROWS][COLS];                        //Making a clone of the board  
                    shadow_clone(board_t, table);                      
                    
                    int destroyed = calculate_num_to_delete(board_t, i, j);   

                    if (destroyed <= NUM_MAX_TO_DELETE) 
                    {
                        int score = minimax(board_t,depth-1 ,0);  
                        opt_score = (score > opt_score) ? score : opt_score;  
                    }
                }
            }
        }
    } 
    else 
    {
        opt_score = INT_MAX;             
        for (int i = 0; i < ROWS; i++) 
        {
            for (int j = 0; j < COLS; j++) 
            {
                if (table[i][j] == 1) 
                {
                
                    int board_t[ROWS][COLS];
                    shadow_clone(board_t, table);  
                    int destroyed = calculate_num_to_delete(board_t, i, j);  
                    
                    if (destroyed <= NUM_MAX_TO_DELETE) 
                    {
                        int score = minimax(board_t, depth-1 , 1);  
                        opt_score = (score < opt_score) ? score : opt_score;  
                    }
                }
            }
        }
    }
    return opt_score;
}


/*
****************************************

The function arise_solider, will simulate the AI behavior

To be clear, it will determine how does the cpu(AI) move

****************************************
*/


void arise_solider(int board[ROWS][COLS]) 
{
    int opt_score = INT_MIN;
    int bestLig = -1;
    int bestCol = -1;

    for (int i = 0; i < ROWS; i++) 
    {
        for (int j = 0; j < COLS; j++) 
        {
            if (board[i][j] == 1)            //is this cell available  ?
            {  
                int plateau_t[ROWS][COLS];
                shadow_clone(plateau_t, board);

                int destroyed = calculate_num_to_delete(plateau_t, i, j);  

                if (destroyed <= NUM_MAX_TO_DELETE) 
                {
                    int score = minimax(plateau_t, 5, 0);  
                    if (score > opt_score) 
                    {
                        opt_score = score;
                        bestLig = i;
                        bestCol = j;
                    }
                }
            }
        }
    }


    if (bestLig != -1 && bestCol != -1)  // AI will choose the best cell
    {
        printf("The CPU had chosen (%d, %d)\n", bestLig, bestCol);
        calculate_num_to_delete(board, bestLig, bestCol);
    }
}
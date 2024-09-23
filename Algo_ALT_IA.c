
/* 

****************************************************
THIS IS THE FULL NON UPDATED VERSION 

THE UPDATED AND ADAPTED VERSION FOR THE PROJECT IS minimax_AI.c
 
The code will determine the behavior of CPU (AI)

The only AI algorithm used here is Minimax

Some usuful functions were re-coded as I didn't have acess to SVN 
******************************************************************

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h> //a library that defines INT_MIN and INT_MAX for Minimax

const int rows=7;            
const int cols=9;          
const int max_sqrs=5;        // Maximum squares that can be destroyed are is set to 5

// The Board will be 7*9

void print_board(int board[rows][cols]) 
{
    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++) 
        {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// game_over function will end the game when (0,0) square is destroyed

int game_over(int board[rows][cols]) 
{
    return board[0][0] == 0;
}

/* domain_collapse is a function that will limit players to only destroy 5 squares */

int domain_collapse(int board[rows][cols], int ligne, int colonne) 
{
    int square_rip = 0;  // how much squares have been destroyed during the current player's turn

    for (int j = colonne; j < cols && square_rip < max_sqrs; j++) // run through squares that are below the chosen one
    {
        if (board[ligne][j] == 1) 
        {
            board[colonne][j] = 0;  // destroy the square by replacing 1 by 0
            square_rip++;
        }
    }

    for (int i = ligne + 1; i < rows && square_rip < max_sqrs; i++) // run through squares that are right to the chosen one
    {
        if (board[i][colonne] == 1) 
        {
            board[i][colonne] = 0;  // destroy the square by replacing 1 by 0
            square_rip++;
        }
    }

    return square_rip;  
}

/* 
shadow_clone function will make an exact copy of the board when it's AI's turn

WHY ? Well it will usuful when the AI has to make its move, so instead of missing around

with the real board, the AI can test its strategy with a copy

bfo(before) is the board during t and aft(after) is the one during t+dt

aft is the board that will be tested by the CPU (AI) 

*/

void shadow_clone(int bfo[rows][cols], int aft[rows][cols]) 
{
    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++) 
        {
            bfo[i][j] = aft[i][j];
        }
    }
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

int minimax(int board[rows][cols],int depth ,int whos_turn) 
{
    if (game_over(board)) 
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
        for (int i = 0; i < rows; i++)            
        {
            for (int j = 0; j < cols; j++) 
            {
                if (board[i][j] == 1)              //Running through the board to find a valid square to destroy
                {  
                    int plateau_t[rows][cols];                        //Making a clone of the board  
                    shadow_clone(plateau_t, board);                      
                    
                    int destroyed = domain_collapse(plateau_t, i, j);   

                    if (destroyed <= max_sqrs) 
                    {
                        int score = minimax(plateau_t,depth-1 ,0);  
                        opt_score = (score > opt_score) ? score : opt_score;  
                    }
                }
            }
        }
    } 
    else 
    {
        opt_score = INT_MAX;             
        for (int i = 0; i < rows; i++) 
        {
            for (int j = 0; j < cols; j++) 
            {
                if (board[i][j] == 1) 
                {
                
                    int plateau_t[rows][cols];
                    shadow_clone(plateau_t, board);  
                    int destroyed = domain_collapse(plateau_t, i, j);  
                    
                    if (destroyed <= max_sqrs) 
                    {
                        int score = minimax(plateau_t, depth-1 , 1);  
                        opt_score = (score < opt_score) ? score : opt_score;  
                    }
                }
            }
        }
    }
    return opt_score;
}

// arise_solider, the function that will simulate AI behavior
void arise_solider(int board[rows][cols]) 
{
    int opt_score = INT_MIN;
    int bestLig = -1;
    int bestCol = -1;

    for (int i = 0; i < rows; i++) 
    {
        for (int j = 0; j < cols; j++) 
        {
            if (board[i][j] == 1)            //is this cell available  ?
            {  
                int plateau_t[rows][cols];
                shadow_clone(plateau_t, board);

                int destroyed = domain_collapse(plateau_t, i, j);  

                if (destroyed <= max_sqrs) 
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

    // AI will choose the best cell
    if (bestLig != -1 && bestCol != -1) 
    {
        printf("The CPU had chosen (%d, %d)\n", bestLig, bestCol);
        domain_collapse(board, bestLig, bestCol);
    }
}

int main() 
{
    int board[rows][cols] = 
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    print_board(board);

    
    while (game_over(board)) 
    {
        int ligne, colonne;

        
        printf("Your Turn : ");
        scanf("%d %d", &ligne, &colonne);

        if (ligne >= 0 && ligne < rows && colonne >= 0 && colonne < cols && board[ligne][colonne] == 1) 
        {
            int eaten = domain_collapse(board, ligne, colonne);
            if (eaten > max_sqrs) 
            {
                printf("Nah uh, there is a limit, only 5 Dummy.\n");
                continue;
            }
        } 
        
        else
        {
            printf("Nah uh, can't choose this one.\n");
            continue;
        }

        print_board(board);

        
        if (game_over(board))           // The player Won
        {
            printf("A win is a win\n");
            break;
        }

        
        arise_solider(board);               //CPU's turn 
        print_board(board);

        
        if (game_over(board))               // AI Won
        {
            printf("GIT GUD!\n");  
            break;
        }
    }

    return 0;
}
/**
 * @file gestion_user.c
 * @brief Implementation of user management functions.
 *
 * This file contains the implementation of functions for managing user
 * information such as player names and scores.
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "gestion_user.h"
#include "const.h"
#include "chomp.h"

// Global variables to store player names and scores
char player1[MAX_NAME_SIZE];
char player2[MAX_NAME_SIZE];
int player; // Indicates the current player (1 or 2)
int tour_number = 0; // Keeps track of the number of turns
int score_player_1 = 0; // Score for player 1
int score_player_2 = 0; // Score for player 2

/**
 * @brief Chooses the names of the players based on the game mode.
 *
 * This function allows the user to input their names for the game.
 * If no name is provided, default names are assigned.
 *
 * @param local_mode Boolean indicating if the game is local.
 * @param ai Boolean indicating if the game is against the AI.
 */
void
chose_players_names (bool local_mode, bool ai)
{
    if (local_mode && !ai) {
        // Prompt for player names in local mode
        printf ("What's your name player 1 (press enter to default to \"player 1\")?\n");
        flush_and_trim_input(player1, MAX_NAME_SIZE);
        if (player1[0] == '\0') {
            strcpy (player1, "player 1");
        }
        printf ("What's your name player 2 (press enter to default to \"player 2\")?\n");
        flush_and_trim_input(player2, MAX_NAME_SIZE);
        if (player2[0] == '\0') {
            strcpy (player2, "player 2");
        }
    } else if (!local_mode && !ai) {
        // Prompt for player names in network mode
        if (player == 1) {
            printf ("What's your name player 1 (press enter to default to \"player 1\")?\n");
            flush_and_trim_input(player1, MAX_NAME_SIZE);
            if (player1[0] == '\0') {
                strcpy (player1, "player 1");
            }
            strcpy (player2, "server"); // Default name for player 2 in network mode
        } else if (player == 2) {
            printf ("What's your name player 2 (press enter to default to \"player 2\")?\n");
            flush_and_trim_input(player2, MAX_NAME_SIZE);
            if (player2[0] == '\0') {
                strcpy (player2, "player 2");
            }
            strcpy (player1, "client"); // Default name for player 1 in network mode
        }
    }else if (local_mode && ai){
        printf ("What's your name player 1 (press enter to default to \"player 1\")?\n");
        flush_and_trim_input(player1, MAX_NAME_SIZE);
        if (player1[0] == '\0') {
            strcpy (player1, "player 1");
        }
        strcpy (player2, "CHOMP-AI");
        
    }else if (!local_mode && ai){
        if(player ==1){
            strcpy (player1, "CHOMP-AI");
            strcpy (player2, "server");
        } else{
            strcpy (player1, "client");
            strcpy (player2, "CHOMP-AI");
        }
    }

    // Print the assigned names for both players
    printf("\033[1;34m%s\033[0m is the player 1 \n", player1);
    printf("\033[1;34m%s\033[0m is the player 2 \n", player2);
}


/**
 * @brief Prints the current score of the players.
 *
 * This function prints the current score of player 1 and player 2 in a formatted
 * manner.
 */
void
print_score ()
{
    // Update scores based on the current player
    if (player == 1) {
        score_player_2++;
    } else if (player == 2) {
        score_player_1++;
    }
    // Print the scores in a formatted manner
    printf ("\033[1;34m%s\033[0m : \033[1;33m%d\033[0m  \033[1;34m%s\033[0m : \033[1;33m%d\033[0m \n", player1, score_player_1, player2, score_player_2);
}

/**
 * @brief Prints the current score of the players in a networked game.
 * 
 * This function prints the current score of player 1 and player 2 in a formatted
 * 
 * @param curent_player The current player (1 or 2).
 */
void
print_score_term_network (int curent_player)
{
    // Update scores based on the current player in network mode
    if (curent_player == 2) {
        score_player_2++;
    } else if (curent_player == 1) {
        score_player_1++;
    }
    // Print the scores in a formatted manner
    printf ("\033[1;34m%s\033[0m : \033[1;33m%d\033[0m  \033[1;34m%s\033[0m : \033[1;33m%d\033[0m \n", player1, score_player_1, player2, score_player_2);
}

/**
 * @brief Prints the current score of the players in a local game against the AI.
 *
 * This function prints the current score of player 1 and the AI in a formatted
 * manner.
 *
 * @param numb_tour The current turn number.
 */
void
print_score_ai_local (int numb_tour)
{
    if (numb_tour%2 == 0){ //TODO check if ok
        score_player_1++;
    } else {
        score_player_2++; //TODO check if ok
    }
    printf ("\033[1;34m%s\033[0m : \033[1;33m%d\033[0m  \033[1;34m%s\033[0m : \033[1;33m%d\033[0m \n", player1, score_player_1, player2, score_player_2);

}

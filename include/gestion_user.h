/**
 * @file gestion_user.h
 * @brief Header file for user management functions.
 *
 * This file contains the declarations of functions and variables used for
 * managing user information such as player names and scores.
 *
 * The functions provided allow for setting player names, printing scores,
 * and handling different game modes (local or network).
 */

#ifndef GESTION_USER_H
#define GESTION_USER_H

#include <stdio.h>
#include <stdbool.h>
#include "const.h"

/**
 * @brief Player 1's name.
 *
 * This variable stores the name of player 1. It is an array of characters
 * with a maximum size defined by MAX_NAME_SIZE.
 */
extern char player1[MAX_NAME_SIZE];

/**
 * @brief Player 2's name.
 *
 * This variable stores the name of player 2. It is an array of characters
 * with a maximum size defined by MAX_NAME_SIZE.
 */
extern char player2[MAX_NAME_SIZE];

/**
 * @brief Current player identifier.
 *
 * This variable holds the identifier of the current player. It is an integer
 * that can be used to determine which player's turn it is.
 */
extern int player;

/**
 * @brief Chooses the names of the players based on the game mode.
 *
 * This function allows the user to input their names for the game.
 * If no name is provided, default names are assigned.
 *
 * @param local_mode Boolean indicating if the game is local.
 * @param ai Boolean indicating if the game is against the AI.
 */
void chose_players_names (bool local_mode,bool ai);

/**
 * @brief Prints the current score of the players.
 *
 * This function prints the current score of player 1 and player 2 in a formatted
 * manner.
 */
void print_score (void);

/**
 * @brief Prints the current score of the players in a networked game.
 * 
 * This function prints the current score of player 1 and player 2 in a formatted
 * 
 * @param curent_player The current player (1 or 2).
 */
void print_score_term_network (int current_player);

/**
 * @brief Prints the current score of the players in a local game against the AI.
 *
 * This function prints the current score of player 1 and the AI in a formatted
 * manner.
 *
 * @param numb_tour The current turn number.
 */
void print_score_ai_local(int numb_tour);

#endif /* GESTION_USER_H */

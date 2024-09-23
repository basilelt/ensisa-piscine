/**
 * @file terminal.h
 * @brief Header file for terminal-based game functions.
 *
 * This file contains the declarations of functions used to start and manage
 * the terminal-based version of the game, both in single-player and network modes.
 *
 * The functions declared in this file are responsible for initializing the game,
 * displaying the welcome screen, and managing the game loop for both single-player
 * and networked games. The networked game functions handle moves from both players
 * and manage network communication.
 */

#ifndef TERMINAL_MODE_H
#define TERMINAL_MODE_H

#include "chomp.h"
#include "network.h"

/**
 * @brief Starts the terminal-based game in single-player mode.
 *
 * This function initializes the game, displays the welcome screen, and manages
 * the game loop for a single-player game.
 *
 * The game loop continues until the game is over, handling player moves and
 * updating the game state accordingly.
 */
void start_terminal(void);

/**
 * @brief Starts the terminal-based game in network mode.
 *
 * @param socket The socket descriptor for network communication.
 * @param player The player number (1 or 2).
 *
 * This function initializes the game, displays the welcome screen, and manages
 * the game loop for a networked game, handling moves from both players.
 *
 * The game loop continues until the game is over, handling player moves,
 * receiving moves from the network, and updating the game state accordingly.
 */
void start_terminal_network(int socket, int player);

/**
 * @brief Starts the terminal-based AI game.
 *
 * This function initializes the game table, displays the welcome screen, and 
 * allows the user to choose player names. It then enters a loop where it alternates 
 * between the player's turn and the AI's turn until the game is over. The player's 
 * turn involves input validation and cell deletion, while the AI's turn involves 
 * pattern detection and random moves. The game ends when the top-left cell of the 
 * table is deleted, and the final score and AI cheat count are displayed.
 *
 * @note The AI can cheat by making invalid moves, which is tracked by the ai_cheat_count variable. but it's for debug only
 */
void start_terminal_ai();

/**
 * @brief Starts the terminal network AI for the game.
 *
 * This function initializes the game table, displays the welcome screen, and
 * allows the AI to make moves in the game. It handles the game loop, where the
 * AI and the other player take turns making moves until the game is over.
 *
 * @param socket The socket descriptor for network communication.
 * @param player The player number (1 or 2) representing the AI to know if it plays fist.
 *
 * The function performs the following steps:
 * - Initializes the game table.
 * - Displays the welcome screen.
 * - Allows the user to choose player names.
 * - Enters the game loop where the AI and the other player take turns.
 * - The AI makes a move based on the game state and network communication.
 * - The function handles the move from the other player.
 * - Prints the game table after each move.
 * - Checks if the game is over and displays the appropriate game over screen.
 * - Prints the final score.
 */
void start_terminal_network_ai(int socket, int player);

#endif /* TERMINAL_MODE_H */

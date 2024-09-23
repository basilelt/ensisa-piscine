/**
 * @file const.h
 * @brief This file contains the constants and global variables used in the Chomp game.
 *
 * This header file defines the constants and global variables that are used throughout
 * the Chomp game. These include dimensions of the game board, buffer sizes, player names,
 * and various game state variables. The constants are defined using macros, and the global
 * variables are declared as extern to be used across different source files.
 */

#ifndef CHOMP_CONSTANTS_H
#define CHOMP_CONSTANTS_H

/**
 * @def MAX_INPUT_LENGTH
 * @brief Maximum length of input.
 */
#define MAX_INPUT_LENGTH 2

/**
 * @def COLS
 * @brief Number of columns in the game board.
 */
#define COLS 9

/**
 * @def ROWS
 * @brief Number of rows in the game board.
 */
#define ROWS 7

/**
 * @def NUM_MAX_TO_DELETE
 * @brief Maximum number of items to delete in one move.
 */
#define NUM_MAX_TO_DELETE 5

/**
 * @def MAX_INPUT_SIZE
 * @brief Maximum size of input.
 */
#define MAX_INPUT_SIZE 10

/**
 * @def MAX_VALID_INPUT_SIZE
 * @brief Maximum valid input size calculated as ROWS * COLS * 5.
 */
#define MAX_VALID_INPUT_SIZE (ROWS * COLS * 5)

/**
 * @def BUFFER_SIZE
 * @brief Size of the buffer.
 */
#define BUFFER_SIZE 10

/**
 * @def MAX_NAME_SIZE
 * @brief Maximum size of player names.
 */
#define MAX_NAME_SIZE 25

/**
 * @var translate
 * @brief Array of strings for translation.
 */
extern const char *translate[];

/**
 * @var player1
 * @brief Name of player 1.
 */
extern char player1[MAX_NAME_SIZE];

/**
 * @var player2
 * @brief Name of player 2.
 */
extern char player2[MAX_NAME_SIZE];

/**
 * @var player
 * @brief Current player.
 */
extern int player;

/**
 * @var tour_number
 * @brief Current tour number.
 */
extern int tour_number;

/**
 * @var score_player_1
 * @brief Score of player 1.
 */
extern int score_player_1;

/**
 * @var score_player_2
 * @brief Score of player 2.
 */
extern int score_player_2;

/**
 * @var welcome_screen
 * @brief Welcome screen message.
 */
extern const char *welcome_screen;

/**
 * @var game_over
 * @brief Game over message.
 */
extern const char *game_over;

/**
 * @var server_socket
 * @brief Server socket descriptor.
 */
extern int server_socket;

/**
 * @var client_socket
 * @brief Client socket descriptor.
 */
extern int client_socket;

#endif /* CHOMP_CONSTANTS_H */

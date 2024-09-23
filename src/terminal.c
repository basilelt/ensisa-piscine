/**
 * @file terminal.c
 * @brief Implementation of terminal-based game functions.
 *
 * This file contains the implementation of functions used to start and manage
 * the terminal-based version of the game, both in single-player and network modes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "terminal.h"
#include "gestion_user.h"
#include "chomp.h"
#include "const.h"
#include "network.h"
#include "ai.h"

// ASCII art for the welcome screen
const char *welcome_screen =
    " __          __  _                            _          _   _             _____ _    _  ____  __  __ _____     _____          __  __ ______ \n"
    " \\ \\        / / | |                          | |        | | | |           / ____| |  | |/ __ \\|  \\/  |  __ \\   / ____|   /\\   |  \\/  |  ____|\n"
    "  \\ \\  /\\  / /__| | ___ ___  _ __ ___   ___  | |_ ___   | |_| |__   ___  | |    | |__| | |  | | \\  / | |__) | | |  __   /  \\  | \\  / | |__   \n"
    "   \\ \\/  \\/ / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  | __| '_ \\ / _ \\ | |    |  __  | |  | | |\\/| |  ___/  | | |_ | / /\\ \\ | |\\/| |  __|  \n"
    "    \\  /\\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |_| | | |  __/ | |____| |  | | |__| | |  | | |      | |__| |/ ____ \\| |  | | |____ \n"
    "     \\/  \\/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/   \\__|_| |_|\\___|  \\_____|_|  |_|\\____/|_|  |_|_|       \\_____/_/    \\_\\_|  |_|______|\n";

// ASCII art for the game over screen
const char *game_over =
    "   _____          __  __ ______    ______      ________ _____  \n"
    "  / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ \n"
    " | |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |\n"
    " | | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  / \n"
    " | |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\ \n"
    "  \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\\n";

// ASCII art for the lose screen
const char *lose_screen =
    " __     ______  _    _   _      ____   _____ _______   _ \n"
    " \\ \\   / / __ \\| |  | | | |    / __ \\ / ____|__   __| | |\n"
    "  \\ \\_/ / |  | | |  | | | |   | |  | | (___    | |    | |\n"
    "   \\   /| |  | | |  | | | |   | |  | |\\___ \\   | |    | |\n"
    "    | | | |__| | |__| | | |___| |__| |____) |  | |    |_|\n"
    "    |_|  \\____/ \\____/  |______\\____/|_____/   |_|    (_)\n";

// ASCII art for the win screen
const char *win_screen =
    " __     ______  _    _  __          ______  _   _   _ \n"
    " \\ \\   / / __ \\| |  | | \\ \\        / / __ \\| \\ | | | |\n"
    "  \\ \\_/ / |  | | |  | |  \\ \\  /\\  / / |  | |  \\| | | |\n"
    "   \\   /| |  | | |  | |   \\ \\/  \\/ /| |  | | . ` | | |\n"
    "    | | | |__| | |__| |    \\  /\\  / | |__| | |\\  | |_|\n"
    "    |_|  \\____/ \\____/      \\/  \\/   \\____/|_| \\_| (_)\n";

/**
 * @brief Starts the terminal-based game in single-player mode.
 *
 * This function initializes the game, displays the welcome screen, and manages
 * the game loop for a single-player game.
 */
void
start_terminal (void)
{
    int flag_game = 1; // Flag to control the game loop
    int select_restart_or_quit; // Variable to store user's choice to restart or quit

    // Display the welcome screen in yellow
    printf ("\033[1;33m%s\033[0m", welcome_screen);
    chose_players_names (true,false); // Function to choose player names

    while (flag_game) {
        int table[ROWS][COLS]; // Game table
        bool game_over_flag = true; // Flag to indicate if the game is over

        init_table (table); // Initialize the game table
        play_game (table, &game_over_flag);  // Play the game, passing the address of game_over_flag
        
        // Display the game over screen in red
        printf ("\033[1;31m%s\033[0m", game_over);
        print_score (); // Print the score
        printf ("\033[1mPRESS 1 TO RETRY OR 0 TO QUIT\033[0m\n");
        scanf ("%d", &select_restart_or_quit); // Get user's choice

        if (select_restart_or_quit == 0) {
            flag_game = 0; // Exit the game loop if the user chooses to quit
        }
    }
}

/**
 * @brief Starts the terminal-based game in network mode.
 *
 * @param socket The socket descriptor for network communication.
 * @param player The player number (1 or 2).
 *
 * This function initializes the game, displays the welcome screen, and manages
 * the game loop for a networked game, handling moves from both players.
 */
void
start_terminal_network (int socket, int player)
{
    int table[ROWS][COLS]; // Game table
    int move = 0; // Variable to store the move
    int current_player = 1; // Variable to track the current player
    bool game_over_flag = false; // Flag to indicate if the game is over

    init_table (table); // Initialize the game table
    // Display the welcome screen in yellow
    printf ("\033[1;33m%s\033[0m", welcome_screen);

    chose_players_names (false,false); // Function to choose player names
    int row = -1, col = -1; // Variables to store the row and column of the move
    print_table (table, col, row); // Print the game table

    while (!game_over_flag) {
        if (current_player == player) {
            // Prompt the current player to make a move
            printf ("%s, it's your turn\n", (player == 1) ? player1 : player2);
            make_move (table, move); // Function to make a move
        } else {
            // Wait for the other player to make a move
            printf ("Waiting for the other player to make a move...\n");
            handle_client_move (socket, table, move, &game_over_flag); // Handle the move from the other player
        }
        print_table (table, last_col_played_term_serv, last_row_played_term_serv); // Print the game table
        current_player = (current_player == 1) ? 2 : 1; // Switch the current player

        // Check if the game is over
        if (table[0][0] == 0) {
            if (current_player == player) {
                // Display the game over and win screens if the current player wins
                printf ("\033[1;31m%s\033[0m", game_over);
                printf ("\033[1;33m%s\033[0m", win_screen);
            } else {
                // Display the game over and lose screens if the current player loses
                printf ("\033[1;31m%s\033[0m", game_over);
                printf ("\033[1;33m%s\033[0m", lose_screen);
            }
            print_score_term_network (current_player); // Print the score
            game_over_flag = true; // Set the game over flag to true
        }
    }
}

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
 */
void
start_terminal_ai ()
{
    int table[ROWS][COLS];
    int numb_tour = 0;
    bool game_over_flag = false;
    
    init_table(table);
    printf ("\033[1;33m%s\033[0m", welcome_screen);

    chose_players_names (true,true);
    int row = -1, col = -1;
    print_table(table, col, row);
    while (!game_over_flag) {
        if (numb_tour%2 == 0) { // put ==1 to make the ai start
            printf ("%s, it's your turn\n",  player1);
            bool user_move_ok = false;
            while(!user_move_ok){
                int col_ok = 0;
                int col_tmp;
                int row_tmp;
                char user_input[MAX_INPUT_SIZE + 1];

                flush_and_trim_input(user_input, sizeof(user_input)); // Get user input
                if (strlen(user_input) != 2) {
                    printf("\033[1;31m\nInvalid input.\nInput must be exactly two characters.\033[0m\n");
                    continue;
                }
                user_input[0] = toupper(user_input[0]); // Convert column to uppercase

                for (int j = 0; j < COLS; j++) {
                    if (user_input[0] == translate[j][0]) {
                        col_tmp = j;
                        col_ok = 1;
                        break;
                    }
                }
                row_tmp = user_input[1] - '0' - 1; // Convert row to integer

                if (col_ok == 0 || row_tmp < 0 || row_tmp >= ROWS) {
                    printf("\033[1;31m\nInvalid input.\n%s is not in the range A1 to I7.\033[0m\n", user_input);
                    continue;
                }
                if (is_this_cell_1_or_0(table, row_tmp, col_tmp) == 1) {
                    printf("\n\033[1;31mThe cell %s is already deleted please try again\033[0m\n", user_input);
                    continue;
                }

                if (calculate_num_to_delete(table, row_tmp, col_tmp) <= NUM_MAX_TO_DELETE) {
                    row = row_tmp;
                    col = col_tmp;
                    delete_cells(table, row, col); // Delete selected cells
                    user_move_ok = true;
                } else {
                    printf("\033[1;31m\nInvalid input.\nYou can delete at most 5 cells.\033[0m\n");
                }
            }
        } else {
            printf ("Waiting for the AI to make it's move\n");
            if (!detect_pattern(table)) {
                int (*possible_moves)[2] = calculate_possibility(table);
                int move_index = 0;
                while (possible_moves[move_index][0] != -1 || possible_moves[move_index][1] != -1) {
                    move_index++;
                }
                if (move_index > 1) {
                    int random_index = rand() % move_index;
                    int i = 0;
                    while (possible_moves[random_index+i][0]==0 && possible_moves[random_index+i][1]==0) {
                        i++;
                    }
                    row = possible_moves[random_index+i][0];
                    col = possible_moves[random_index+i][1];
                    delete_cells(table, row, col);
                } else if (move_index == 1) {
                    row = 0;
                    col = 0;
                    delete_cells(table,row,col);
                }
            } else {
                int move = pattern_move(table); //TODO CHECK IF VALID MOVE
                row = move/COLS;
                col = move%COLS;
                delete_cells(table, row, col);
            }
        }

        print_table(table, col, row);
        numb_tour++;

        if (table[0][0] == 0) {
            if (numb_tour%2==0) { //  put ==1 if the ai is starting
                printf ("\033[1;31m%s\033[0m", game_over);
                printf("\033[1;33m%s\033[0m",win_screen);
            } else {
                printf ("\033[1;31m%s\033[0m", game_over);
                printf("\033[1;33m%s\033[0m", lose_screen);
            }
            print_score_ai_local(numb_tour);
            game_over_flag = true;
        }
    }
}

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
void
start_terminal_network_ai (int socket, int player)
{
    int table[ROWS][COLS]; // Game table
    int move = 0; // Variable to store the move
    int current_player = 1; // Variable to track the current player
    bool game_over_flag = false; // Flag to indicate if the game is over

    init_table (table); // Initialize the game table
    // Display the welcome screen in yellow
    printf ("\033[1;33m%s\033[0m", welcome_screen);

    chose_players_names (false,true); // Function to choose player names
    int row = -1, col = -1; // Variables to store the row and column of the move
    print_table (table, col, row); // Print the game table

    while (!game_over_flag) {
        if (current_player == player) {
            printf("it's the ai turn\n");
            bool ai_move_ok = false;

            while(!ai_move_ok) {
                //sleep(1);
                if (!detect_pattern(table)) {
                    int (*possible_moves)[2] = calculate_possibility(table);
                    int move_index = 0;
                    while (possible_moves[move_index][0] != -1 || possible_moves[move_index][1] != -1) {
                        move_index++;
                    }

                    if (move_index > 1) {
                        int random_index = rand() % move_index;
                        int i = 0;
                        while (possible_moves[random_index+i][0]==0 && possible_moves[random_index+i][1]==0){
                            i++;
                        }
                        row = possible_moves[random_index+i][0];
                        col = possible_moves[random_index+i][1];



                        if(make_ai_move_network(table, row*COLS+col, socket)){
                            ai_move_ok = true;
                        } else {

                            continue;
                        }                   
                    } else if (move_index == 1){
                        row = 0;
                        col = 0;

                        if(make_ai_move_network(table, row*COLS+col, socket)){
                            ai_move_ok = true;
                        } else {

                            continue;
                        }    
                    }
                } else {
                    int move = pattern_move(table); //TODO CHECK IF VALID MOVE
                    row = move/COLS;
                    col = move%COLS;
                    if(make_ai_move_network(table, move, socket)){ // Function to make a move
                        ai_move_ok = true;
                    } else {

                        continue;
                    }    
                }
            }


        } else {
            printf("Waiting for the other player to make a move\n");
            // Wait for the other player to make a move

            handle_client_move(socket, table, move, &game_over_flag); // Handle the move from the other player
        }
        print_table(table, last_col_played_term_serv, last_row_played_term_serv); // Print the game table
        current_player = (current_player == 1) ? 2 : 1; // Switch the current player

        // Check if the game is over
        if (table[0][0] == 0) {
            if (current_player == player) {
                // Display the game over and win screens if the current player wins
                printf("\033[1;31m%s\033[0m", game_over);
                printf("\033[1;33m%s\033[0m", win_screen);
            } else {
                // Display the game over and lose screens if the current player loses
                printf("\033[1;31m%s\033[0m", game_over);
                printf("\033[1;33m%s\033[0m", lose_screen);
            }
            print_score_term_network(current_player); // Print the score

            game_over_flag = true; // Set the game over flag to true
        }
    }
}

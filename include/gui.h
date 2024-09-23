/**
 * @file gui.h
 * @brief Header file for the GUI functions of the Chomp game.
 *
 * This file contains the declarations of the functions used to manage the
 * graphical user interface of the Chomp game using GTK3.
 */

#ifndef GUI_H
#define GUI_H

#include <gtk/gtk.h>
#include "network.h"

/**
 * @brief Starts the GUI in server mode.
 * 
 * This function starts the GUI in server mode.
 * It creates a server socket, accepts a client connection,
 * and starts the game GUI.
 *
 * @param port The port number to listen on.
 * @param ai TRUE if playing against AI, FALSE otherwise.
 * @return The result of start_gui_game().
 */
int start_gui_server_mode(short port, bool ai);

/**
 * @brief Starts the GUI in client mode.
 * 
 * This function starts the GUI in client mode.
 * It creates a client socket, connects to the server,
 * and starts the game GUI.
 *
 * @param ip The IP address of the server.
 * @param port The port number to connect to.
 * @param ai TRUE if playing against AI, FALSE otherwise.
 * @return The result of start_gui_game().
 */
int start_gui_client_mode(const char *ip, short port,bool ai);

/**
 * @brief Starts the GUI for the Chomp game.
 * 
 * This function initializes the GUI for the Chomp game.
 * It sets up the window, grid, buttons, and labels.
 * It also handles different game modes: local, network, and AI.
 *
 * @param argc The argument count (unused).
 * @param argv The argument vector (unused).
 * @param network TRUE if in network mode, FALSE otherwise.
 * @param ai TRUE if playing against AI, FALSE otherwise.
 * @param player_num The player number (1 for client, 2 for server, 0 for local).
 * 
 * @return The exit status.
 */
int start_gui_game(int argc, char *argv[], bool network, bool ai, int player_num);

/**
 * @brief Handles the response to the game over dialog.
 * 
 * This function handles the response to the game over dialog.
 * It checks the response ID and performs the appropriate action:
 * close the game or start a new game.
 *
 * @param dialog The dialog widget.
 * @param response_id The response ID.
 * @param user_data User data (unused).
 */
void on_game_over_response(GtkDialog *dialog, gint response_id, gpointer user_data);

/**
 * @brief Restarts the game.
 *
 * This function resets the game state, including the table,
 * button styles, turn number, and score labels.
 *
 * @param widget The widget that triggered the restart (unused).
 * @param data User data (unused).
 */
void restart_game(GtkWidget *widget, gpointer data);

/**
 * @brief Shows an error dialog with a given message.
 *
 * This function displays an error dialog with the specified message.
 * It is used to inform the user of any errors that occur during the game.
 *
 * @param parent The parent widget.
 * @param message The error message to display.
 */
void show_error_dialog(GtkWidget *parent, const char *message);

/**
 * @brief Shows the game over dialog with the winner's name.
 *
 * This function displays the game over dialog and shows the name of the
 * winner. It is called when the game ends.
 *
 * @param parent The parent widget.
 * @param winner The name of the winner.
 */
void show_game_over_dialog(GtkWidget *parent, const char *winner);

/**
 * @brief Shows the turn dialog with the current player's name.
 *
 * This function displays a dialog indicating that it's the current player's turn.
 *
 * @param parent The parent widget.
 * @param player The name of the current player.
 */
void show_turn_dialog(GtkWidget *parent, const char *player);

/**
 * @brief Updates the style of a button at a given position.
 *
 * This function updates the style of the button located at the specified
 * row and column. It changes the button's color to the specified color.
 *
 * @param row The row of the button.
 * @param col The column of the button.
 * @param color The color to set.
 */
void update_button_style(int row, int col, const char *color);

/**
 * @brief Handles the button click event.
 *
 * This function is called when a button is clicked. It processes the
 * button click event and performs the necessary actions.
 *
 * @param widget The button widget.
 * @param data User data.
 */
void on_button_clicked(GtkWidget *widget, gpointer data);

/**
 * @brief Handles the move entered event.
 *
 * This function is called when a move is entered. It processes the move
 * and updates the game state accordingly.
 *
 * @param widget The widget where the move was entered.
 * @param data User data.
 */
void on_move_entered(GtkWidget *widget, gpointer data);

/**
 * @brief Updates the GUI cells based on the game state.
 *
 * This function updates the GUI cells starting from the specified row
 * and column based on the current game state.
 *
 * @param row The starting row.
 * @param col The starting column.
 */
void update_gui_cells(int row, int col);

/**
 * @brief Shows the player name dialog.
 *
 * This function displays a dialog to enter the player names.
 * It handles different modes: local game, network game, and AI game.
 *
 * @param parent The parent widget.
 * @param is_server_or_client_or_nothing 0 for local game, 1 for network client, 2 for network server or AI game.
 */
void show_player_name_dialog(GtkWidget *parent, int is_server_or_client_or_nothing);

/**
 * @brief Receives the opponent's move in a separate thread.
 * 
 * This function receives the opponent's move from the network.
 * It validates the move and schedules a GUI update in the main thread.
 */
void receive_opponent_move(void);

/**
 * @brief Updates the GUI based on the opponent's move.
 * 
 * This function is called from the main thread to update the GUI
 * after receiving the opponent's move.
 *
 * @param data The opponent's move as a pointer.
 * @return FALSE to remove the idle handler.
 */
gboolean update_opponent_move_gui(gpointer data);

/**
 * @brief Sets the sensitivity of the game board buttons.
 * 
 * This function sets the sensitivity of the game board buttons.
 * It enables or disables the buttons based on the specified sensitivity.
 *
 * @param sensitive TRUE to enable buttons, FALSE to disable.
 */
void set_board_sensitive(gboolean sensitive);

/**
 * @brief AI chooses its move.
 * 
 * This function implements the AI's move selection.
 * It uses pattern detection or random selection from possible moves.
 */
void ai_choosing_its_move(void);

/**
 * @brief Shows the game over dialog for network mode.
 *
 * This function displays a dialog indicating that the game is over
 * and announces the winner in network mode.
 * 
 * @param parent The parent widget.
 */
void show_game_over_dialog_network (GtkWidget *parent);

#endif /* GUI_H */

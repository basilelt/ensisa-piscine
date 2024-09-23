/**
 * @file gui.c
 * @brief Implementation of the GUI functions for the Chomp game.
 *
 * This file contains the implementation of the functions used to manage the
 * graphical user interface of the Chomp game using GTK3.
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <string.h>
#include "chomp.h"
#include "const.h"
#include "gui.h"
#include "ai.h"
#include "network.h"


#define LONG_STR 512
#define COLOR_BLUE "blue"
#define COLOR_RED "red"
#define COLOR_YELLOW "yellow"

int table[ROWS][COLS];

char player1_name[LONG_STR] = "Player 1";
char player2_name[LONG_STR] = "Player 2";

int player1_score = 0;
int player2_score = 0;
bool last_move_by_local = false; 
// storing the last played cell to add some color to the terminal
int last_col_played = -1; 
int last_row_played = -1;
int last_index_clicked = -1;
extern int client_socket;
extern int server_socket;
int player_gui = 0;  // 1 for client, 2 for server, 3 for client ai, 4 for server ai
bool is_ai_local = false;
bool is_ai_network = false;
bool is_network = false;
void set_board_sensitive(gboolean sensitive);
void receive_opponent_move();
gboolean update_opponent_move_gui(gpointer data);
int start_gui_game(int argc, char *argv[], bool network, bool ai,int player_num);
int current_player_playing = 0;

// Function declarations (forward declarations)
void on_game_over_response(GtkDialog *dialog, gint response_id, gpointer user_data);
void restart_game(GtkWidget *widget, gpointer data);
void show_error_dialog(GtkWidget *parent, const char *message);
void show_game_over_dialog(GtkWidget *parent, const char *winner);
void show_turn_dialog(GtkWidget *parent, const char *player);
void update_button_style(int row, int col, const char *color);
void on_button_clicked(GtkWidget *widget, gpointer data);
void on_move_entered(GtkWidget *widget, gpointer data);
void update_gui_cells(int row, int col);
void show_player_name_dialog(GtkWidget *parent,int is_server_or_client_or_nothing);
void ai_choosing_its_move();
void show_game_over_dialog_network (GtkWidget *parent);

GtkWidget *last_clicked_button = NULL;
GtkWidget *turn_label = NULL;
GtkWidget *score_label = NULL;
int turn_number = 0;

struct dev_button {
    GtkWidget *btn;
    GtkCssProvider *bp;
};

struct dev_button buttons[ROWS][COLS];

/**
 * @brief Shows an error dialog with a given message.
 * 
 * This function displays an error dialog with a given message.
 * It creates a message dialog and displays the error message.
 *
 * @param parent The parent widget.
 * @param message The error message to display.
 */
void
show_error_dialog (GtkWidget *parent, const char *message)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_ERROR,
                                               GTK_BUTTONS_CLOSE,
                                               "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

/**
 * @brief Shows the game over dialog with the winner's name.
 *
 * This function displays a dialog indicating that the game is over
 * and announces the winner.
 * 
 * @param parent The parent widget.
 * @param winner The name of the winner.
 */
void
show_game_over_dialog (GtkWidget *parent, const char *winner)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_INFO,
                                            GTK_BUTTONS_NONE,
                                            "Game Over! %s wins!", winner);

    gtk_dialog_add_button(GTK_DIALOG(dialog), "Close", GTK_RESPONSE_CLOSE);
    gtk_dialog_add_button(GTK_DIALOG(dialog), "New Game", GTK_RESPONSE_ACCEPT);

    g_signal_connect(dialog, "response", G_CALLBACK(on_game_over_response), parent);

    gtk_widget_show_all(dialog);
}

/**
 * @brief Shows the game over dialog for network mode.
 *
 * This function displays a dialog indicating that the game is over
 * and announces the winner in network mode.
 * 
 * @param parent The parent widget.
 */
void
show_game_over_dialog_network (GtkWidget *parent)
{
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                            GTK_DIALOG_DESTROY_WITH_PARENT,
                                            GTK_MESSAGE_INFO,
                                            GTK_BUTTONS_NONE,
                                            "%s", (last_move_by_local == 0) ? "You win!" : "You lose!");

    gtk_dialog_add_button(GTK_DIALOG(dialog), "Close", GTK_RESPONSE_CLOSE);

    g_signal_connect(dialog, "response", G_CALLBACK(on_game_over_response), parent);

    gtk_widget_show_all(dialog);
}

/**
 * @brief Shows the turn dialog with the current player's name.
 *
 * This function displays a dialog indicating that it's the current player's turn.
 *
 * @param parent The parent widget.
 * @param player The name of the current player.
 */
void
show_turn_dialog (GtkWidget *parent, const char *player)
{
    if (player == 0){
    GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(parent),
                                               GTK_DIALOG_DESTROY_WITH_PARENT,
                                               GTK_MESSAGE_INFO,
                                               GTK_BUTTONS_CLOSE,
                                               "It's %s's turn!", player);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    }
}

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
void
on_game_over_response (GtkDialog *dialog, gint response_id, gpointer user_data)
{
    if (response_id == GTK_RESPONSE_CLOSE) {
        gtk_main_quit();
    } else if (response_id == GTK_RESPONSE_ACCEPT) {
        restart_game(GTK_WIDGET(dialog), user_data);
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));
}

/**
 * @brief Restarts the game.
 *
 * This function resets the game state, including the table,
 * button styles, turn number, and score labels.
 *
 * @param widget The widget that triggered the restart (unused).
 * @param data User data (unused).
 */
void
restart_game (GtkWidget *widget, gpointer data)
{
    (void)widget;
    (void)data;

    init_table(table);
    last_clicked_button = NULL;
    turn_number = 0;

    // Reset the button styles and make them sensitive again
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            // Reset button styles here
            update_button_style(i, j, COLOR_BLUE);
            gtk_widget_set_sensitive(buttons[i][j].btn, TRUE);
        }
    }

    // Set the turn label to player 1's name
    gtk_label_set_text(GTK_LABEL(turn_label), player1_name);

    // Update the score label
    char score_text[LONG_STR];
    snprintf(score_text, sizeof(score_text), "Scores: %s: %d, %s: %d", player1_name, player1_score, player2_name, player2_score);
    gtk_label_set_text(GTK_LABEL(score_label), score_text);
}

/**
 * @brief Updates the style of a button at a given position.
 *
 * This function updates the style of a button at a given position.
 * It sets the background color of the button based on the specified color.
 *
 * @param row The row of the button.
 * @param col The column of the button.
 * @param color The color to set.
 */
void
update_button_style (int row, int col, const char *color)
{
    char css[LONG_STR];
    snprintf(css, sizeof(css), ".btnid_%02d_%02d{background: %s}", row, col, color);
    gtk_css_provider_load_from_data(buttons[row][col].bp, css, -1, NULL);
}

/**
 * @brief Updates the GUI cells based on the game state.
 *
 * This function updates the GUI cells starting from the specified row
 * and column based on the current game state.
 *
 * @param row The starting row.
 * @param col The starting column.
 */
void
update_gui_cells (int row, int col)
{
    int tmp_col = COLS;
    for (int i = row; i < ROWS; i++) {
        for (int j = col; j < tmp_col; j++) {
            if (table[i][j] == 0) {
                update_button_style(i, j, COLOR_RED);
                gtk_widget_set_sensitive(buttons[i][j].btn, FALSE);
            }
        }
    }
    if(last_index_clicked != -1){
        int last_row = last_index_clicked / COLS;
        int last_col = last_index_clicked % COLS;
        update_button_style(last_row, last_col, COLOR_RED);
    }
    last_index_clicked = row * COLS + col;
    update_button_style(row, col, COLOR_YELLOW);


    // Process all pending GTK events to ensure the GUI updates immediately
    while (gtk_events_pending()) gtk_main_iteration();
}

/**
 * @brief Prints the current state of the table to the console.
 *
 * This function prints the current state of the table to the console.
 * It highlights the last played cell in terminal.
 */
void
print_table_state ()
{
    printf("Current table state:\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (j == last_col_played && i == last_row_played) { // Highlight the last played cell in terminal
                printf("\033[1;33m%d \033[0m", table[i][j]);
            } else {
                printf("%d ", table[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * @brief Handles the button click event.
 * 
 * This function handles the button click event for the game board.
 * It validates the move, updates the game state, checks for game over,
 * and handles network communication if in network mode.
 *
 * @param widget The button widget that was clicked.
 * @param data User data containing the button's index.
 */
void
on_button_clicked (GtkWidget *widget, gpointer data)
{
    int index = GPOINTER_TO_INT(data);
    int row = index / COLS;
    int col = index % COLS;
    bool valid_move = false;
    // Check if the move is valid
    if (calculate_num_to_delete(table, row, col) <= NUM_MAX_TO_DELETE) {
        delete_cells(table, row, col);
        valid_move = true;
        if (row == 0 && col == 0) {
            last_move_by_local = true;
        }
        // Store the last played cell to add some color to the terminal
        last_col_played = col;  
        last_row_played = row;
        
        // Update the GUI based on the updated table
        update_gui_cells(row, col);

        // Highlight the current clicked button
        print_table_state();

        // Check if the game is over
        if (table[0][0] == 0) {
            const char *winner = (turn_number % 2 == 0) ? player2_name : player1_name;
            if (turn_number % 2 == 0) {
                player2_score++;
            } else {
                player1_score++;
            }
            if (is_network) {
                show_game_over_dialog_network(gtk_widget_get_toplevel(widget));
            } else {
                show_game_over_dialog(gtk_widget_get_toplevel(widget), winner);
            }

        } else {
            // Update the turn number and the current player label
            turn_number++;
            const char *current_player = (turn_number % 2 == 0) ? player1_name : player2_name;
            gtk_label_set_text(GTK_LABEL(turn_label), current_player);
            show_turn_dialog(gtk_widget_get_toplevel(widget), current_player);
        }
    } else {
        // Show an error dialog if the move is invalid
        show_error_dialog(gtk_widget_get_toplevel(widget), "Invalid move. You can delete at most 5 cells.");
    }

    // Send the move to the opponent if in network mode
    int move = row * COLS + col;
    if ((player_gui == 1 || player_gui==2) && valid_move){
        if (send_move(client_socket, move) == -1 ) {
            show_error_dialog(gtk_widget_get_toplevel(widget), "Failed to send move to the opponent.");
            return;
        }
        set_board_sensitive(FALSE);

        // Wait for the opponent's move
        g_thread_new(NULL, (GThreadFunc)receive_opponent_move, NULL);
    
    }
    if (is_ai_network) {
        if (send_move(client_socket, move) == -1 ) {
            show_error_dialog(gtk_widget_get_toplevel(widget), "Failed to send move to the opponent.");
            return;
        }
        set_board_sensitive(FALSE);

        // Wait for the opponent's move
        g_thread_new(NULL, (GThreadFunc)receive_opponent_move, NULL);
    }
    if (is_ai_local && (turn_number%2 == 1)) {
        ai_choosing_its_move();
    }
  
}

/**
 * @brief Handles the move entered event.
 * 
 * This function handles the move entered event.
 * It validates the move format and updates the game state accordingly.
 *
 * @param widget The widget where the move was entered.
 * @param data User data.
 */
void
on_move_entered (GtkWidget *widget, gpointer data)
{
    (void)data;

    const char *move = gtk_entry_get_text(GTK_ENTRY(widget));
    if (strlen(move) < 2 || strlen(move) > 3) {
        show_error_dialog(gtk_widget_get_toplevel(widget), "Invalid move format. Use format like A1.");
        return;
    }

    char col_char = move[0];
    int row = atoi(&move[1]) - 1;
    int col = -1;

    // Translate the column character to an index
    for (int i = 0; i < COLS; i++) {
        if (translate[i][0] == col_char) {
            col = i;
            break;
        }
    }

    // Check if the move is valid
    if (col == -1 || row < 0 || row >= ROWS) {
        show_error_dialog(gtk_widget_get_toplevel(widget), "Invalid move. Use format like A1.");
        return;
    }

    if (table[row][col] == 0) {
        show_error_dialog(gtk_widget_get_toplevel(widget), "Invalid move. The cell is already deleted.");
        return;
    }

    int index = row * COLS + col;
    on_button_clicked(buttons[row][col].btn, GINT_TO_POINTER(index));
    gtk_entry_set_text(GTK_ENTRY(widget), "");
}

/**
 * @brief Shows the player name dialog.
 * 
 * This function displays a dialog to enter the player names.
 * It handles different modes: local game, network game, and AI game.
 *
 * @param parent The parent widget.
 * @param is_server_or_client_or_nothing 0 for local game, 1 for network client, 2 for network server or AI game.
 */
void
show_player_name_dialog (GtkWidget *parent,int is_server_or_client_or_nothing)
{
    if(is_server_or_client_or_nothing == 0){
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Enter Player Names",
                                                    GTK_WINDOW(parent),
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    NULL);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    GtkWidget *grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(content_area), grid);

    GtkWidget *player1_label = gtk_label_new("Player 1 Name:");
    gtk_grid_attach(GTK_GRID(grid), player1_label, 0, 0, 1, 1);
    GtkWidget *player1_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), player1_entry, 1, 0, 1, 1);

    GtkWidget *player2_label = gtk_label_new("Player 2 Name:");
    gtk_grid_attach(GTK_GRID(grid), player2_label, 0, 1, 1, 1);
    GtkWidget *player2_entry = gtk_entry_new();
    gtk_grid_attach(GTK_GRID(grid), player2_entry, 1, 1, 1, 1);

    gtk_widget_show_all(dialog);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
        const char *player1_text = gtk_entry_get_text(GTK_ENTRY(player1_entry));
        const char *player2_text = gtk_entry_get_text(GTK_ENTRY(player2_entry));
        if (player1_text[0] == '\0') {
            player1_text = "Player 1"; // setting names to default if no names are chosen for player 1
        }
        if (player2_text[0] == '\0') {
            player2_text = "Player 2"; // setting names to default if no names are chosen for player 2
        }
        strncpy(player1_name, player1_text, LONG_STR);
        strncpy(player2_name, player2_text, LONG_STR);
    }

    gtk_widget_destroy(dialog);
    } else if (is_server_or_client_or_nothing == 1) {
        const char *player1_text = "client";
        const char *player2_text = "server";
        strncpy(player1_name, player1_text, LONG_STR);
        strncpy(player2_name, player2_text, LONG_STR);
    } else if (is_server_or_client_or_nothing == 2) {
        strncpy(player2_name, "CHOMP-AI", LONG_STR);
        strncpy(player1_name, "player 1", LONG_STR);
    } else if (is_server_or_client_or_nothing == 3 && player_gui == 3) {
        strncpy(player1_name, "CHOMP-AI", LONG_STR);
        strncpy(player2_name, "server", LONG_STR);
    } else if (is_server_or_client_or_nothing == 3 && player_gui == 4) {
        strncpy(player1_name, "client", LONG_STR);
        strncpy(player2_name, "CHOMP-AI", LONG_STR);
    }
}

/**
 * @brief Receives the opponent's move in a separate thread.
 * 
 * This function receives the opponent's move from the network.
 * It validates the move and schedules a GUI update in the main thread.
 */
void
receive_opponent_move (void)
{
    int opponent_move = receive_move (client_socket);
    if (opponent_move == -1) {
        show_error_dialog (NULL, "Failed to receive move from opponent.");
        return;
    }

    // Schedule GUI update in the main thread
    g_idle_add ((GSourceFunc) update_opponent_move_gui, GINT_TO_POINTER (opponent_move));
    
}

/**
 * @brief Updates the GUI based on the opponent's move.
 * 
 * This function is called from the main thread to update the GUI
 * after receiving the opponent's move.
 *
 * @param data The opponent's move as a pointer.
 * @return FALSE to remove the idle handler.
 */
gboolean
update_opponent_move_gui (gpointer data)
{
  int opponent_move = GPOINTER_TO_INT (data);
  int opp_row = opponent_move / COLS;
  int opp_col = opponent_move % COLS;

    if (!validate_and_apply_move (table, opponent_move)) {
        show_error_dialog (NULL, "Invalid move received from the opponent.");
        return FALSE;
    }

    last_col_played = opp_col;
    last_row_played = opp_row;

    // Update GUI to reflect the opponent's move
    update_gui_cells (opp_row, opp_col);
    if (opp_row == 0 && opp_col == 0){
        last_move_by_local = false;
    }
    // Check for game over
    if (table[0][0] == 0) {
        const char *winner = (player == 1) ? player2_name : player1_name;
        if (is_network) {
            show_game_over_dialog_network (NULL);
        } else {
            show_game_over_dialog (NULL, winner);
        }
    } else {
        // Enable the board for the local player's turn
        set_board_sensitive (TRUE);
    }
    if (is_ai_network){
        ai_choosing_its_move();

    }

  return FALSE; // Remove the idle handler
}

/**
 * @brief Sets the sensitivity of the game board buttons.
 * 
 * This function sets the sensitivity of the game board buttons.
 * It enables or disables the buttons based on the specified sensitivity.
 *
 * @param sensitive TRUE to enable buttons, FALSE to disable.
 */
void
set_board_sensitive (gboolean sensitive)
{
  for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (table[i][j] == 1) {
                gtk_widget_set_sensitive (buttons[i][j].btn, sensitive);
            }
        }
    }
}

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
int
start_gui_server_mode (short port, bool ai)
{
    if (!ai){
        server_socket = start_server (port);
        client_socket = accept (server_socket, NULL, NULL);
        player_gui = 2;  // Server is player 2
        is_network = true;
        current_player_playing = 1;
        return start_gui_game(0, NULL, true,false, 2);
    }else{
        server_socket = start_server (port);
        client_socket = accept (server_socket, NULL, NULL);
        player_gui = 4;  // Server  ai is player 4
        is_ai_network = true;
        is_network = true;
        current_player_playing = 1;
        return start_gui_game(0, NULL, true,true, 2);
    }
}

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
int
start_gui_client_mode (const char *ip, short port, bool ai)
{
    if (!ai){
        client_socket = start_client (ip, port);
        player_gui = 1;  // Client is player 1
        is_network = true;
        current_player_playing = 0;
        return start_gui_game(0, NULL,true,false, 1);
    }else{
        client_socket = start_client (ip, port);
        player_gui = 3;  // Client ai is player 3
        is_ai_network = true;
        is_network = true;
        current_player_playing = 0;
        return start_gui_game(0, NULL,true,true, 1);
    }
}

/**
 * @brief AI chooses its move.
 * 
 * This function implements the AI's move selection.
 * It uses pattern detection or random selection from possible moves.
 */
void ai_choosing_its_move ()
{
    int col_ai;
    int row_ai;
    if (!detect_pattern(table)){
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
                    row_ai = possible_moves[random_index+i][0];
                    col_ai = possible_moves[random_index+i][1];
                    on_button_clicked(buttons[row_ai][col_ai].btn, GINT_TO_POINTER(row_ai*COLS + col_ai));

                }else if (move_index == 1){
                    row_ai = 0;
                    col_ai = 0;
                    on_button_clicked(buttons[row_ai][col_ai].btn, GINT_TO_POINTER(row_ai*COLS + col_ai));

                }
            }else{
                int move = pattern_move(table); //TODO CHECK IF VALID MOVE
                row_ai = move/COLS;
                col_ai = move%COLS;
                on_button_clicked(buttons[row_ai][col_ai].btn, GINT_TO_POINTER(move));
            }
}
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
int
start_gui_game (int argc, char *argv[], bool network,bool ai, int player_num)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *entry;

    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    if (network && player_gui == 1) {
        gtk_window_set_title (GTK_WINDOW (window), "CHOMP GAME - CLIENT");
    } else if (network && player_gui == 2) {
        gtk_window_set_title (GTK_WINDOW (window), "CHOMP GAME - SERVER");
    } else if (network && player_gui == 3) {
        gtk_window_set_title (GTK_WINDOW (window), "CHOMP GAME - CLIENT AI");
    } else if (network && player_gui == 4) {
        gtk_window_set_title (GTK_WINDOW (window), "CHOMP GAME - SERVER AI");
    } else {
        gtk_window_set_title (GTK_WINDOW (window), "CHOMP GAME");
    }
    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
    g_signal_connect (window, "destroy", G_CALLBACK (gtk_main_quit), NULL);

    grid = gtk_grid_new ();
    gtk_container_add (GTK_CONTAINER (window), grid);

    init_table (table);

    // Create column labels
    for (int j = 0; j < COLS; j++) {
        GtkWidget *label = gtk_label_new (translate[j]);
        gtk_grid_attach (GTK_GRID (grid), label, j + 1, 0, 1, 1);
    }

    // Create row labels and buttons
    for (int i = 0; i < ROWS; i++) {
        GtkWidget *label = gtk_label_new (g_strdup_printf ("%d", i + 1));
        gtk_grid_attach (GTK_GRID (grid), label, 0, i + 1, 1, 1);

        for (int j = 0; j < COLS; j++) {
          buttons[i][j].btn = gtk_button_new ();
          buttons[i][j].bp = gtk_css_provider_new ();

          gtk_widget_set_size_request (buttons[i][j].btn, 50, 50);

          char css_class[20];
          snprintf (css_class, sizeof (css_class), "btnid_%02d_%02d", i, j);
          GtkStyleContext *context = gtk_widget_get_style_context (buttons[i][j].btn);
          gtk_style_context_add_class (context, css_class);

          g_signal_connect (buttons[i][j].btn, "clicked", G_CALLBACK (on_button_clicked), GINT_TO_POINTER (i * COLS + j));
          gtk_grid_attach (GTK_GRID (grid), buttons[i][j].btn, j + 1, i + 1, 1, 1);
        }
    }

    // Create entry for move input
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "Enter move (e.g., A1)");
    g_signal_connect(entry, "activate", G_CALLBACK(on_move_entered), entry);
    gtk_grid_attach(GTK_GRID(grid), entry, 0, ROWS + 1, COLS + 1, 1);

    // Create labels for turn and score
    turn_label = gtk_label_new(player1_name);
    gtk_grid_attach(GTK_GRID(grid), turn_label, 0, ROWS + 2, COLS + 1, 1);

    score_label = gtk_label_new("");
    gtk_grid_attach(GTK_GRID(grid), score_label, 0, ROWS + 3, COLS + 1, 1);

    gtk_widget_show_all(window);

    // Apply CSS providers to buttons and set initial styles
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            GtkStyleContext *context = gtk_widget_get_style_context(buttons[i][j].btn);
            gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(buttons[i][j].bp), GTK_STYLE_PROVIDER_PRIORITY_USER);
            update_button_style(i, j, COLOR_BLUE);
        }
    }

    // Show dialog to enter player names
    if(!network && !ai){
        show_player_name_dialog(window,0);
    }else if(network && !ai){
        show_player_name_dialog(window,1);
    }else if(!network && ai){
        current_player_playing = 0;
        is_ai_local = true;
        show_player_name_dialog(window,2);
    }else if(network && ai){
        show_player_name_dialog(window,3);
    }
    
    // Update the score label
    char score_text[LONG_STR];
    snprintf(score_text, sizeof(score_text), "Scores: %s: %d, %s: %d", player1_name, player1_score, player2_name, player2_score);
    gtk_label_set_text(GTK_LABEL(score_label), score_text);

    // Set board sensitivity based on player and start receiving moves if server
    if (player_num == 1) {
        set_board_sensitive (TRUE); // Client starts first
        if (ai){
            ai_choosing_its_move();
        }
    } else if (player_num == 2) {
        set_board_sensitive (FALSE); // Wait for opponent
        g_thread_new (NULL, (GThreadFunc) receive_opponent_move, NULL);
    } else { // Local GUI mode
        set_board_sensitive (TRUE); // Allow both players to play locally
    }

    gtk_main ();

  return 0;
}

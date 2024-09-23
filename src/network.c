/**
 * @file network.c
 * @brief Implementation of network operations for server-client communication in the game.
 *
 * This file contains the function definitions for setting up server-client communication,
 * sending and receiving moves, handling signals, and validating and applying moves.
 */

#include "network.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <ctype.h>
#include "chomp.h"
#include "const.h"

int client_socket = -1;
int server_socket = -1;

int last_row_played_term_serv = -1;
int last_col_played_term_serv = -1;

/**
 * @brief Starts the server on the specified port.
 *
 * This function initializes the server socket, binds it to the specified port,
 * and starts listening for incoming connections.
 *
 * @param port The port number to start the server on.
 * @return The server socket file descriptor.
 */
int
start_server (short port)
{
    struct sockaddr_in server_addr;

    setup_signal_handlers (); // Set up signal handlers for clean termination

    server_socket = socket (AF_INET, SOCK_STREAM, 0); // Create a TCP socket
    if (server_socket < 0) {
           perror ("Socket creation failed");
           exit (EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (port); // Convert port to network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any available network interface

    if (bind (server_socket, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0) {
        perror ("Bind failed");
        close (server_socket);
        exit (EXIT_FAILURE);
    }

    if (listen (server_socket, 1) < 0) {
        perror ("Listen failed");
        close (server_socket);
        exit (EXIT_FAILURE);
    }

    printf ("Server listening on port %d\n", port);
    return server_socket;
}

/**
 * @brief Starts the client and connects to the specified server IP and port.
 *
 * This function initializes the client socket and connects it to the server
 * specified by the IP address and port number.
 *
 * @param ip The IP address of the server.
 * @param port The port number of the server.
 * @return The client socket file descriptor.
 */
int
start_client (const char *ip, short port)
{
    struct sockaddr_in server_addr;

    setup_signal_handlers (); // Set up signal handlers for clean termination

    client_socket = socket (AF_INET, SOCK_STREAM, 0); // Create a TCP socket
    if (client_socket < 0) {
        perror ("Socket creation failed");
        exit (EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons (port); // Convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr (ip); // Convert IP address to binary form

    if (connect (client_socket, (struct sockaddr *) &server_addr, sizeof (server_addr)) < 0) {
        perror ("Connect failed");
        close (client_socket);
        exit (EXIT_FAILURE);
    }

    printf ("Connected to server %s:%d\n", ip, port);
    return client_socket;
}

/**
 * @brief Gets the move from the user.
 *
 * This function prompts the user to enter a move in the format of a letter
 * followed by a digit (e.g., I7). It validates the input and converts it
 * to the corresponding move index.
 *
 * @return The move entered by the user, or -1 if the input is invalid.
 */
int
get_move (void)
{
    char input[MAX_INPUT_SIZE]; /* Use defined constant for buffer size */
    int row, col;

    printf ("Enter your move (e.g., I7): ");
    if (fgets (input, sizeof (input), stdin) != NULL) {
        size_t len = strlen (input);
        if (len > 0 && input[len - 1] != '\n') {
            /* Input too long, clear stdin */
            int c;
            while ((c = getchar ()) != '\n' && c != EOF);
        } else {
            /* Remove newline character */
            input[strcspn (input, "\n")] = '\0';
        }

        // Validate input format
        if (strlen (input) < 2 || !isalpha (input[0]) || !isdigit (input[1])) {
            printf ("Invalid input format. Please enter a letter followed by a digit (e.g., I7).\n");
            return -1;
        }

        col = toupper (input[0]) - 'A'; // Convert letter to column index
        row = input[1] - '1'; // Convert digit to row index

        // Check if move is within board range
        if (col < 0 || col >= COLS || row < 0 || row >= ROWS) {
            printf ("Invalid move. Please enter a valid move within the board range.\n");
            return -1;
        }

        return row * COLS + col; // Return move as a single index
    } else {
        printf ("Error reading input.\n");
        return -1;
    }
}

/**
 * @brief Sends a move to the specified socket.
 *
 * This function sends the move to the specified socket.
 *
 * @param socket The socket file descriptor.
 * @param move The move to send.
 * @return 0 on success, -1 on failure.
 */
int
send_move (int socket, int move)
{
    char move_str[3];
    move_str[0] = 'A' + (move % COLS); // Convert column index to letter
    move_str[1] = '1' + (move / COLS); // Convert row index to digit
    move_str[2] = '\0';
    printf("%s will be sent\n",move_str);
    if (send(socket, move_str, sizeof(move_str), 0) < 0) {
        perror("Send move failed");
        return -1;
    }
    return 0;
}

/**
 * @brief Receives a move from the specified socket.
 *
 * This function receives a move from the specified socket.
 *
 * @param socket The socket file descriptor.
 * @return The received move, or -1 on failure.
 */
int
receive_move (int socket)
{
    char move_str[3];
    ssize_t bytes_received = recv(socket, move_str, sizeof(move_str), 0);
    if (bytes_received < 0) {
        perror("Receive move failed");
        return -1;
    } else if (bytes_received == 0) {
        printf("Client disconnected.\n");
        return -1;
    }

    // Validate received move format
    if (strlen(move_str) < 2 || !isalpha(move_str[0]) || !isdigit(move_str[1])) {
        printf("Invalid move format received.\n");
        return -1;
    }
    int col = toupper(move_str[0]) - 'A'; // Convert letter to column index
    int row = move_str[1] - '1'; // Convert digit to row index

    // Check if move is within board range
    if (col < 0 || col >= COLS || row < 0 || row >= ROWS) {
        printf("Invalid move received: out of bounds.\n");
        return -1;
    }

    return row * COLS + col; // Return move as a single index
}

/**
 * @brief Validates and applies the move to the game table.
 *
 * This function validates the move and applies it to the game table if it is valid.
 *
 * @param table The game table.
 * @param move The move to validate and apply.
 * @return true if the move is valid and applied, false otherwise.
 */
bool
validate_and_apply_move (int table[ROWS][COLS], int move)
{
    if (move == -1) {
        printf ("Invalid move: move cannot be empty.\n");
        return false;
    }

    int row = move / COLS;
    int col = move % COLS;
    if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
        printf ("Invalid move: out of bounds.\n");
        return false;
    }

    if (table[row][col] == 0) {
        printf ("Invalid move: cell is already deleted.\n");
        return false;
    }

    int num_to_delete = calculate_num_to_delete (table, row, col); // Calculate number of cells to delete
    if (num_to_delete <= 0) {
        printf ("Invalid move: no cells to delete.\n");
        return false;
    }

    if (num_to_delete > NUM_MAX_TO_DELETE) {
        printf ("Invalid move: exceeds maximum number of cells to delete.\n");
        return false;
    }

    delete_cells (table, row, col); // Apply the move by deleting cells
    last_row_played_term_serv = row;
    last_col_played_term_serv = col;
    return true;
}

/**
 * @brief Handles the client's move.
 *
 * This function receives a move from the client, validates it, and applies it to the game table.
 *
 * @param client_socket The client socket file descriptor.
 * @param table The game table.
 * @param move The move to handle.
 * @param game_over_flag Pointer to a flag indicating if the game is over.
 */
void
handle_client_move (int client_socket, int table[ROWS][COLS], int move, bool *game_over_flag)
{
    while (1) {
        move = receive_move(client_socket); // Receive move from client
        if (move == -1) {
            printf("Invalid move from client or client disconnected.\n");
            *game_over_flag = true; // Set game over flag if move is invalid or client disconnected
            break;
        }
        if (!validate_and_apply_move(table, move)) {
            printf("Invalid move from client.\n");
            continue; // Continue to receive next move if current move is invalid
        }
        break; // Break loop if move is valid
    }
}

/**
 * @brief Makes a move and sends it to the server.
 *
 * This function gets a move from the user, validates it, applies it to the game table,
 * and sends it to the server.
 *
 * @param table The game table.
 * @param move The move to make.
 */
void
make_move (int table[ROWS][COLS], int move)
{
    while (1) {
        move = get_move (); // Get move from user
        if (move == -1) {
            continue; // Continue to prompt user if move is invalid
        }
        if (!validate_and_apply_move (table, move)) {
            continue; // Continue to prompt user if move is invalid
        }
        if (send_move (client_socket, move) == -1) {
            continue; // Continue to prompt user if sending move fails
        }
        break; // Break loop if move is valid and sent successfully
    }
}

/**
 * @brief Makes a move in the game and sends it over the network.
 *
 * This function attempts to make a move in the game by validating and applying
 * the move to the game table. If the move is valid, it then sends the move to
 * the specified socket.
 *
 * @param table The game table represented as a 2D array of integers.
 * @param move The move to be made, represented as an integer.
 * @param socket The socket descriptor used to send the move over the network.
 * @return true if the move was successfully made and sent, false otherwise.
 */
bool
make_ai_move_network(int table[ROWS][COLS], int move,int socket)
{
    if (!validate_and_apply_move(table,move)) {
        return false;
    } else if (send_move(socket, move) == -1) {
        return false;
    } else {
        return true;
    }
}
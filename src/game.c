/**
 * @file game.c
 * @brief Main entry point for the application.
 *
 * This file contains the main function. 
 * It handles the command-line arguments to determine
 * the mode of operation.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gestion_user.h"
#include "main_func.h"
#include "terminal.h"
#include "gui.h"

/**
 * @brief Main function.
 * 
 * This is the main entry point of the program. It sets up signal handlers and parses command-line arguments
 * to determine the mode of operation (GUI, server, client, or local terminal).
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 on success, or 1 on invalid arguments.
 */
int
main (int argc, char *argv[])
{
    setup_signal_handlers(); // Set up signal handlers

    int gui_mode = 1; // Default to GUI mode
    int ai_mode = 0;
    int local_mode = 0;
    int server_mode = 0;
    int client_mode = 0;

    char *server_ip = NULL;
    short server_port = 0;

    // Array to keep track of used arguments (to avoid assigning them multiple times)
    bool used_args[argc];
    for (int i = 0; i < argc; i++) {
        used_args[i] = false;
    }

    // First pass: Set flags and identify flags that require parameters
    for (int i = 1; i < argc; i++) {
        if (used_args[i]) {
            continue; // Skip if already used as a parameter
        }

        if (strcmp(argv[i], "-t") == 0) {
            gui_mode = 0; // Disable GUI mode, enable terminal mode
            used_args[i] = true;
        } else if (strcmp(argv[i], "-ia") == 0) {
            ai_mode = 1; // Enable AI mode
            used_args[i] = true;
        } else if (strcmp(argv[i], "-l") == 0) {
            local_mode = 1; // Set mode to local
            used_args[i] = true;
        } else if (strcmp(argv[i], "-s") == 0) {
            server_mode = 1; // Set mode to server
            used_args[i] = true;

            // Search for the next unused non-flag argument as the port
            bool port_found = false;
            for (int j = i + 1; j < argc; j++) {
                if (!used_args[j] && argv[j][0] != '-') {
                    server_port = (short)atoi(argv[j]); // Get port number for server mode
                    used_args[j] = true;
                    port_found = true;
                    break;
                }
            }

            if (!port_found) {
                fprintf(stderr, "Error: No port specified for server mode.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-c") == 0) {
            client_mode = 1; // Set mode to client
            used_args[i] = true;

            // Search for the next unused non-flag argument as the IP:Port
            bool ipport_found = false;
            for (int j = i + 1; j < argc; j++) {
                if (!used_args[j] && argv[j][0] != '-') {
                    char *ip_port = argv[j];
                    char *colon_ptr = strchr(ip_port, ':'); // Find ':' to split IP and port
                    if (colon_ptr != NULL) {
                        *colon_ptr = '\0'; // Null-terminate IP string
                        server_ip = ip_port; // Extract IP address
                        server_port = (short)atoi(colon_ptr + 1); // Extract port number
                        used_args[j] = true;
                        ipport_found = true;
                        break;
                    }
                    else {
                        fprintf(stderr, "Error: Invalid IP:Port format for client mode.\n");
                        return 1;
                    }
                }
            }

            if (!ipport_found) {
                fprintf(stderr, "Error: No IP:Port specified for client mode.\n");
                return 1;
            }
        } else {
            fprintf(stderr, "Invalid argument: %s\n", argv[i]); // Invalid argument
            return 1;
        }
    }

    // Debugging output to verify flag states
    printf("AI Mode: %d, GUI Mode: %d, Server Mode: %d, Client Mode: %d\n", ai_mode, gui_mode, server_mode, client_mode);

    // Ensure that only one mode is selected among local, server, or client
    int mode_count = local_mode + server_mode + client_mode;
    if (mode_count > 1) {
        fprintf(stderr, "Error: Multiple modes selected. Please choose one mode: -l, -s, or -c.\n");
        return 1;
    } else if (mode_count == 0) {
        // Default to local mode if no mode is specified
        local_mode = 1;
    }

    // Start the game based on the selected mode and options
    if (local_mode) {
        extern int player;
        player = 1; // Set player to local terminal

        if (gui_mode && ai_mode) {
            printf("Starting local game with GUI and AI mode\n");
            return start_gui_game(argc, argv, false,true, -1); // Start local game with GUI and ai
        } else if (gui_mode) {
            printf("Starting local game with GUI mode\n");
            return start_gui_game(argc, argv, false,false, -1); // Start local game with GUI
        } else if (ai_mode) {
            printf("Starting local game with AI mode\n");
            start_terminal_ai(); // Start local game with AI
        } else {
            printf("Starting local game in terminal mode\n");
            start_terminal(); // Start local game in terminal mode
        }
    } else if (server_mode) {
        if (gui_mode && ai_mode) {
            printf("Starting server with GUI and AI mode\n");
            return start_gui_server_mode(server_port, true); // Start server mode with GUI and ai
        }

        if (gui_mode) {
            printf("Starting server mode with GUI on port %d\n", server_port);
            return start_gui_server_mode(server_port,false); // Start server mode with GUI
        } else if (ai_mode) {
            printf("Starting server mode with AI on port %d\n", server_port);
            start_server_mode(server_port, true); // Start server mode with AI in terminal
        } else {
            printf("Starting server mode on port %d\n", server_port);
            start_server_mode(server_port, false); // Start server mode without AI
        }
    } else if (client_mode) {
        if (gui_mode && ai_mode) {
            printf("Starting client with GUI and AI mode\n");
            return start_gui_client_mode(server_ip, server_port,true); // Start client with GUI
        } else if (gui_mode) {
            printf("Starting client with GUI mode\n");
            return start_gui_client_mode(server_ip, server_port,false); // Start client with GUI
        } else if (ai_mode) {
            printf("Starting client with AI mode\n");
            start_client_mode(server_ip, server_port, true); // Start client in terminal mode with AI
        } else {
            printf("Starting client in terminal mode\n");
            start_client_mode(server_ip, server_port, false); // Start client in terminal mode
        }
    } else {
        fprintf(stderr, "Error: Unknown mode.\n"); // Should not reach here
        return 1;
    }

    return 0;
}

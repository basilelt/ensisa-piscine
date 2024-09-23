/**
 * @file test_network.c
 * @brief This file contains the implementation of network-related test functions.
 * 
 * The tests include functions to start a server, start a client, send moves, and receive moves.
 * These tests ensure that the network communication between the server and client works correctly.
 */
#include "network.h"
#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

/**
 * @brief Function to handle server-side operations in a separate thread.
 * 
 * This function accepts a client connection and sends a move to the client.
 * 
 * @param arg Pointer to an integer representing the server socket file descriptor.
 * @return NULL
 */
void* 
server_thread_func(void* arg) 
{
    int server_socket = *(int*)arg;
    int client_socket = accept(server_socket, NULL, NULL);
    assert(client_socket > 0);

    // Example move to send
    int move = 5 * COLS + 2;
    int result = send_move(client_socket, move);
    assert(result == 0);
    printf("Test sent move OK\n");

    return NULL;
}

/**
 * @brief Test function to start a server, start a client, and receive a move.
 * 
 * This function starts a server and a client, then receives a move sent from the server.
 */
void 
test_receive_move() 
{
    // Start server
    int server_socket = start_server(1234);
    assert(server_socket > 0);
    printf("Test Start Server OK\n");

    // Create server thread
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, server_thread_func, &server_socket);

    // Wait for the server to start
    sleep(1);

    // Start client
    int client_socket = start_client("127.0.0.1", 1234);
    assert(client_socket > 0);
    printf("\nTest start Client OK\n");

    // Receive move on client side
    int received_move = receive_move(client_socket);
    int expected_move = 5 * COLS + 2;
    assert(received_move == expected_move);
    if (received_move == expected_move) {
        printf("test_receive_move OK.\n");
    } else {
        printf("test_receive_move FAILED.\n");
    }

// Close sockets
close(client_socket);
close(server_socket);

}
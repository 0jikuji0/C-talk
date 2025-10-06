#include "../include/server.h"
#include "../include/client.h"
#include "../include/network.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    char value;

    printf("Serveur ou client ? [s/c] : ");
    scanf("%c", &value);

    if (value == 'c')
    {
        client_socket sockets = initialize_client(DEFAULT_LISTENING_PORT, DEFAULT_CONNECTION_HOST);

        send_message_client(sockets.socket_fd, "coucou\n");

        close_client(sockets);
    }

    if (value == 's')
    {
        /*
        server_socket sockets = initialize_server(DEFAULT_LISTENING_PORT, DEFAULT_PENDING_QUEUE_MAX_LENGHT);

        char buffer[DEFAULT_BUFFER_SIZE];
        receive_message_server(sockets.connected_socket_fd, DEFAULT_BUFFER_SIZE, buffer);
        printf("%s", buffer);

        close_server(sockets);
         */

        ServerSocket s_sock = init_server(DEFAULT_LISTENING_PORT);

        char* message = receive_message(s_sock.client_socket.socket);
        if (message) {
            printf("Message reçu: %s\n", message);
            free(message);
        }

        close_server_socket(s_sock);
    }

}

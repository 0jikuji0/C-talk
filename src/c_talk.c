#include "../include/client.h"
#include "../include/server.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        printf("Veuillez spécifier le mode du programme ([S]erveur ou [C]lient)");
        return 0;
    }

    char value = argv[1][0];

    if (value == 'c')
    {
        Socket socket = initialize_client(DEFAULT_LISTENING_PORT, DEFAULT_CONNECTION_HOST);

        send_message_client(socket.socket, "coucou\n");

        for (;;) {
            printf("Votre message: ");

            char* message = get_message();

            if (message != NULL) {
                send_message_client(socket.socket, message);
            }

            free(message);
            message = NULL;
        }

        close_socket(&socket);
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


        char* message = receive_message_server(s_sock.connection.socket);
        if (message) {
            printf("Message reçu: %s\n", message);
            free(message);
        }

        for (;;) {
            char* message = receive_message_server(s_sock.connection.socket);
            if (message) {
                printf("Message reçu: %s\n", message);
                free(message);
                message = NULL;
            }
        }

        close_server(s_sock);
    }

}

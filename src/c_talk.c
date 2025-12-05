#include "../include/client.h"
#include "../include/server.h"
#include "../include/crypto.h"
#include <stddef.h>
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

        // génération des clés

        uint64_t p, g, secret_key, public_key, private_key;
        publicParams(&p, &g);
        privateParams(&secret_key);

        public_key = publicKey(p, g, secret_key);

        private_key = privateKey(p, public_key, secret_key);

        char * ciphertext = NULL;

        char * plaintext = "coucou";

        //encrypt(plaintext, &ciphertext, private_key);
        //send_message_client(socket.socket, ciphertext);

        send_message_client(socket.socket, plaintext);
        free(ciphertext);


        for (;;) {
            printf("Votre message: ");

            char* message = get_message();

            if (message == NULL) {
                free(message);
                continue;
            }

            if (strcmp(message, "/exit") == 0) {
                free(message);
                message = NULL;
                break;
            }

            //encrypt(plaintext, &ciphertext, private_key);
            //send_message_client(socket.socket, ciphertext);
            send_message_client(socket.socket, message);
            free(ciphertext);
            ciphertext = NULL;
        }

        close_socket(socket);
        free_socket(socket);
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

        // ServerSocket s_sock = init_server(DEFAULT_LISTENING_PORT);

        size_t p, g, secret_key, public_key, private_key;
        size_t r;
        publicParams(&p, &g);
        privateParams(&secret_key);

        // soit secret_key_c la clé secrète du client et secret_key_s la clé sercrète du serveur
        // échanger p et g
        //exchange_primes(s_sock.connection.socket);

        // public_key = échanger le résultat r de (g^secret_key_s) % p
        r = publicKey(p, g, secret_key);

        // public_key du client
        //public_key = exchange_public_keys(s_sock.connection.socket, r);

        // (r ^ secret_key_s) % p
        // private_key = On a donc (g ^ secret_key_c * secret_key_s) % p
        private_key = privateKey(p, public_key, secret_key);


        run_server_loop(DEFAULT_LISTENING_PORT);
    }

}


/*

Server -| Client 1

Client 1 | Client 2 | Client 3

Server -> Client 1 clé

Server -> Client 2 clé
-> Client 1 clé
-> Client 3 clé

Message reçu: houcou

*/

#include "../include/client.h"
#include "../include/server.h"
#include "../include/crypto.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

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
        mpz_t p, g, secret_key, public_key, private_key, public_key_server_mpz;
        mpz_inits(p, g, secret_key, public_key, private_key, public_key_server_mpz, ((void *)0));

        publicParams(p, g);
        privateParams(secret_key);

        publicKey(public_key, p, g, secret_key);

        char buffer_public_key[42];
        gmp_snprintf(buffer_public_key, sizeof(buffer_public_key), "%Zd", public_key);
        const char * public_key_server = "345";
        send_message_client(socket.socket, buffer_public_key);

        // receive_message_client(socket.socket, sizeof(uint32_t), public_key_server);

        // printf("[Public Key Server] %s\n", public_key_server_mpz);
        gmp_printf("[Public Key Server] %Zd\n", public_key_server_mpz);
        mpz_set_str(public_key_server_mpz, public_key_server, 10);

        privateKey(private_key, p, public_key_server_mpz, secret_key);

        char * ciphertext = NULL;

        const char * plaintext = "coucou";

        encrypt(plaintext, &ciphertext, private_key);

        send_message_client(socket.socket, ciphertext);

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

            encrypt(plaintext, &ciphertext, private_key);
            send_message_client(socket.socket, ciphertext);
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

        ServerSocket s_sock = init_server(DEFAULT_LISTENING_PORT);

        mpz_t p, g, secret_key, public_key, private_key, public_key_client_mpz;
        mpz_inits(p, g, secret_key, public_key, private_key, public_key_client_mpz, NULL);

        publicParams(p, g);
        privateParams(secret_key);

        // soit secret_key_c la clé secrète du client et secret_key_s la clé sercrète du serveur
        // échanger p et g
        //exchange_primes(s_sock.connection.socket);

        // public_key = échanger le résultat r de (g^secret_key_s) % p
        publicKey(public_key, p, g, secret_key);

        // public_key du client

        char buffer_public_key[42];
        // snprintf(buffer_public_key, sizeof(buffer_public_key), "%zu", public_key);
        gmp_snprintf(buffer_public_key, sizeof(buffer_public_key), "%Zd", public_key);

        char* public_key_client = receive_message_server(s_sock.connection.socket);
        
        send_message_server(s_sock.connection.socket, buffer_public_key);

        mpz_set_str(public_key_client_mpz, public_key_client, 10);

        // printf("[Public Key Client] %lu\n", 42);
        // (r ^ secret_key_s) % p
        // private_key = On a donc (g ^ secret_key_c * secret_key_s) % p
        privateKey(private_key, p, public_key_client_mpz, secret_key);

        char* message = receive_message_server(s_sock.connection.socket);

        if (message) {
            printf("Message reçu: %s\n", message);
            free(message);
        }

        for (;;) {
            char* ciphertext = receive_message_server(s_sock.connection.socket);
            if (ciphertext) {
                char * decrypted = NULL;
                decrypt(ciphertext, &decrypted, private_key);
                printf("Message reçu \nAvant dechiffrement : %s\nApres dechiffrement : %s\n", ciphertext, decrypted);
                free(ciphertext);
                ciphertext = NULL;
                free(decrypted);
                decrypted = NULL;
            }        
        }

        close_server(s_sock);
        free_server(s_sock);
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

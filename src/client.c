/*
 * =============================================================================
 * Fichier : client.c
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Implémentation des fonctions pour le module client.
 *   Permet de se connecter à un serveur distant et d'échanger des messages.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#include "../include/network.h"
#include "../include/client.h"

#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERROR] (CLIENT) %s : %s (code: %d)\n", (msg), strerror(errno), errno); exit(EXIT_FAILURE);

Socket initialize_client(uint16_t port, char * connection_host) {

    int sock = init_socket(TCP);

    // Configuration du socket
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = port;

    int inet_return_code = inet_pton(AF_INET, connection_host, &socket_address.sin_addr);

    if (inet_return_code == -1) {
        LOG_ERROR("Adresse invalide");
    }

    Socket socket = newSocket(sock, socket_address);

    int socket_address_length = sizeof(socket_address);
    int connection_status = connect(socket.socket, (struct sockaddr *) &socket_address, socket_address_length);

    if (connection_status == -1) {
        LOG_ERROR("Echec de la connexion au serveur");
    }

    return socket;
}

void receive_message_client(int socket_fd, uint32_t buffer_size, char *buffer) {
    int received_bytes = recv(socket_fd, buffer, buffer_size, 0);

    if (received_bytes == -1) {
        LOG_ERROR("Echec de la reception du message du serveur");
    }

    buffer[received_bytes] = '\0';
}

int send_message_client(int socket_fd, char message[]) {
    int send_bytes = send(socket_fd, message, strlen(message), 0);

    if (send_bytes == -1) {
        LOG_ERROR("Echec de l'envoie du message au serveur");
    }

    return 0;
}

char* get_message() {
    char* buffer;

    assert((buffer = malloc(MAX_BUFFER_SIZE * sizeof(char))) != NULL);

    if (fgets(buffer, MAX_BUFFER_SIZE, stdin) == NULL) {
        free(buffer);
        return NULL;
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    if (strlen(buffer) > 0) {
        return buffer;
    }

    free(buffer);
    return NULL;
}

int close_client(Socket client_socket) {
    close(client_socket.socket);

    return 0;
}

/*
 * =============================================================================
 * Fichier : server.c
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Implémentation des fonctions pour le module serveur.
 *   Écoute les connexions entrantes et redistribue les messages aux clients.
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#include "../include/server.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERROR] (SERVER) %s : %s (code: %d)\n", (msg), strerror(errno), errno); exit(EXIT_FAILURE);

server_socket initialize_server(uint16_t port, uint16_t pending_queue_max_length) {

    // Création du socket
    server_socket sockets;

    sockets.socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Gestion des erreurs de la création d'initialisation du socket
    if (sockets.socket_fd == -1) {
        LOG_ERROR("Echec d'initialisation du socket");
    }

    // Configuration du socket
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = port;
    socket_address.sin_addr.s_addr = INADDR_ANY;

    int socket_address_length = sizeof(socket_address);

    // Liaison du socket
    int bind_return_code = bind(sockets.socket_fd, (struct sockaddr*) &socket_address, socket_address_length);

    // Gestion des erreurs de la liaison du socket
    if (bind_return_code == -1) {
        LOG_ERROR("Echec de la liaison pour le socket");
    }

    // Attente de nouvelles connexions avec gestion des erreurs
    if (listen(sockets.socket_fd, pending_queue_max_length) == -1) {
        LOG_ERROR("Echec de demarrage de l'ecoute des connexions entrantes");
    }

    puts("En attente de nouvelles connexions ...");

    sockets.connected_socket_fd = accept(sockets.socket_fd, (struct sockaddr*) &socket_address, (socklen_t *) &socket_address_length);

    if (sockets.connected_socket_fd == -1) {
        LOG_ERROR("Echec d'etablissement de la connexion");
    }

    return sockets;
}

void receive_message_server(int connected_socket_fd, uint32_t buffer_size, char *buffer) {
    int received_bytes = recv(connected_socket_fd, buffer, buffer_size, 0);

    if (received_bytes == -1) {
        LOG_ERROR("Echec de la reception du message du client");
    }

    buffer[received_bytes] = '\0';
}

int send_message_server(int connected_socket_fd, char message[]) {
    int send_bytes = send(connected_socket_fd, message, strlen(message), 0);

    if (send_bytes == -1) {
        LOG_ERROR("Echec de l'envoie du message au client");
    }

    return 0;
}

int close_server(server_socket server_socket) {
    close(server_socket.socket_fd);
    close(server_socket.connected_socket_fd);

    return 0;
}

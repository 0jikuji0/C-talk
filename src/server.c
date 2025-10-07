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
#include "../include/network.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERROR] (SERVER) %s : %s (code: %d)\n", (msg), strerror(errno), errno); exit(EXIT_FAILURE);

ServerSocket init_server(uint16_t port) {
    // Utilisation, pour l'instant, du port par défaut
    Socket sock = create_socket(TCP, port);
    sock.s_listen(sock.socket);
    ClientSocket sc = sock.s_accept(sock.socket);

    ServerSocket server_socket = newServerSocket(sock, sc);

    return server_socket;
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

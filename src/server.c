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

    // TODO créer un thread afin que ça ne bloque pas le programme.
    // Modifier le comportement pour supporter le multi-threading ?
    s_listen(sock.socket);
    Socket client_sock = s_accept(sock.socket);

    // TODO socket UDP pour signaler la room

    ServerSocket server_socket = newServerSocket(&sock, &client_sock);

    return server_socket;
}

char* receive_message_server(int connected_socket_fd) {
    char* message_ptr = receive_message(connected_socket_fd);

    if (message_ptr == NULL) {
        LOG_ERROR("Echec de la reception du message du client");
    }

    return message_ptr;
}

int send_message_server(int connected_socket_fd, char message[]) {
    int send_bytes = send(connected_socket_fd, message, strlen(message), 0);

    if (send_bytes == -1) {
        LOG_ERROR("Echec de l'envoie du message au client");
    }

    return 0;
}

int close_server(ServerSocket socket) {
    close_server_socket(&socket);
    return 0;
}

int free_server(ServerSocket server_socket) {
    free_socket(server_socket.connection);
    free_socket(server_socket.listener);
}

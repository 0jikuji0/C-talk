/*
 * =============================================================================
 * Fichier : network.c
 * Projet  : C-talk - Application de messagerie en C
 * ----------------------------------------------------------------------------
 * Description :
 *   Implémentation des fonctions pour la gestion des sockets et des
 *   connexions réseau côté serveur.
 *
 * ----------------------------------------------------------------------------
 * Auteur(s) : FortyTwo Dev
 * Date de création : 05/10/2025
 * Dernière modification : 05/10/2025
 * =============================================================================
 */

#include "../include/network.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERREUR] (RESEAU) %s : %s (code: %d)\n", (msg), strerror(errno), errno); exit(EXIT_FAILURE);

Socket newSocket(int socket, struct sockaddr_in address) {
    Socket s;
    s.socket = socket;
    s.address = &address;
    return s;
}

ServerSocket newServerSocket(Socket* listening_socket, Socket* client_socket) {
    ServerSocket ss;
    ss.listener = *listening_socket;
    ss.connection = *client_socket;
    return ss;
}

int close_socket(Socket* socket) {
    close(socket->socket);
    return 0;
}

void close_server_socket(ServerSocket* sock) {
    close(sock->listener.socket);
    close(sock->connection.socket);
}

void get_ip_str(struct sockaddr_in client_address, char* buff) {
    inet_ntop(AF_INET, &(client_address.sin_addr), buff, INET_ADDRSTRLEN);
}

int init_socket(enum Mode mode){
    if (mode != TCP && mode != UDP) {
        LOG_ERROR("Mode socket non supporté");
    }

    int sockfd = socket(AF_INET, mode, 0);
    if (sockfd == -1) {
        LOG_ERROR("Erreur lors de la création du socket");
    }

    return sockfd;
}

void param_socket(int socket) {
    int opt = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0) {
    	LOG_ERROR("Echec de paramètrage");
    }
}

struct sockaddr_in attach_address(int socket, uint16_t port) {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    // Ecoute sur toutes les adresses
    address.sin_addr.s_addr = INADDR_ANY;
    // Conversion du port en valeur réseaux (Host TO Network Short)
    address.sin_port = port;

    if (bind(socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
        LOG_ERROR("Echec d'attachement: ");
    }

    return address;
}

Socket create_socket(enum Mode mode, uint16_t port) {
    int sock = init_socket(mode);
    param_socket(sock);
    struct sockaddr_in address = attach_address(sock, port);

    printf("Socket crée\n");

    return newSocket(sock, address);
}

void s_listen(int socket) {
    // SOMAXCONN représente la val max de la file de connexion
    int err = listen(socket, SOMAXCONN);
    if (err != 0) {
        printf("Echec de la mise en écoute: %s\n", strerror(errno));
    }
    printf("J'écoute...\n");
}

Socket s_accept(int socket) {
    printf("Fonction s_accept\n");
    int client_sock;
    struct sockaddr_in client_address;
    socklen_t addr_len = sizeof(client_address);

    client_sock = accept(socket, (struct sockaddr *) &client_address, (socklen_t *)&addr_len);
    printf("Acceptation...\n");
    if (client_sock == -1) {
        LOG_ERROR("Acceptation ratée");
    }

    char ip[INET_ADDRSTRLEN];
    get_ip_str(client_address, ip);
    printf("Connexion de %s:%i\n", ip, ntohs(client_address.sin_port));

    return newSocket(client_sock, client_address);
}

int send_message(int socket, const char *message) {
    size_t len = strlen(message);
    ssize_t sent = send(socket, message, len, 0);

    if (sent == -1) {
        LOG_ERROR("Echec lors de l'envoi du message");
    } else if (((size_t)sent < len)) {
        fprintf(stderr, "[AVERTISSEMENT] Message partiellement envoyé (%zd/%zu octets)\n", sent, len);
        exit(EXIT_FAILURE);
    }

    return 1;
}

char* receive_message(int socket) {
    char* buf_ptr = NULL;
    assert((buf_ptr = (char* )malloc(MAX_BUFFER_SIZE)) != NULL);

    ssize_t received = recv(socket, buf_ptr, MAX_BUFFER_SIZE - 1, 0);

    if (received == -1) {
        free(buf_ptr);
        LOG_ERROR("Erreur lors de la réception du message.");
    } else if (received == 0) {
        printf("Connexion fermée par l'interlocuteur\n");
        free(buf_ptr);
        return NULL;
    }

    buf_ptr[received] = '\0';
    return buf_ptr;
}

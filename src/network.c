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
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERREUR] %s : %s (code: %d)\n", (msg), strerror(errno), errno); exit(EXIT_FAILURE);

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
    	printf("Echec de paramètrage: %s\n", strerror(errno));
    }
}

void attach_address(int socket, uint16_t port) {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    // Ecoute sur toutes les adresses
    address.sin_addr.s_addr = INADDR_ANY;
    // Conversion du port en valeur réseaux (Host TO Network Short)
    address.sin_port = htons(port);

    if (bind(socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
    	printf("Echec d'attachement: %s\n", strerror(errno));
    }
}

Socket create_socket(enum Mode mode, uint16_t port) {
    int sock = init_socket(mode);
    param_socket(sock);
    attach_address(sock, port);

    Socket s;
    s.socket = sock;
    s.s_listen = &s_listen;
    s.s_accept = &s_accept;
    return s;
}

void s_listen(int socket) {
    // SOMAXCONN représente la val max de la file de connexion
    int err = listen(socket, SOMAXCONN);
    if (err != 0) {
        printf("Echec de la mise en écoute: %s\n", strerror(errno));
    }
}

ClientSocket s_accept(int socket) {
    int client_sock;
    struct sockaddr_in client_address;
    unsigned int addrLen = sizeof(client_address);

    client_sock = accept(socket, (struct sockaddr *) &client_address, &addrLen);
    if (client_sock == -1) {
        LOG_ERROR("Acceptation ratée");
    }

    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client_address.sin_addr), ip, INET_ADDRSTRLEN);
    printf("Connexion de %s:%i\n", ip, client_address.sin_port);

    ClientSocket client_socket;
    client_socket.socket = client_sock;
    client_socket.address = client_address;

    return client_socket;
}

void send_message(int socket, const char *message) {
    size_t len = strlen(message);
    ssize_t sent = send(socket, message, len, 0);

    if (sent == -1) {
        LOG_ERROR("Echec lors de l'envoi du message");
    } else if (((size_t)sent < len)) {
        fprintf(stderr, "[AVERTISSEMENT] Message partiellement envoyé (%zd/%zu octets)\n", sent, len);
        exit(EXIT_FAILURE);
    }
}

char* receive_message(int socket) {
    char buffer[1024];
    ssize_t received = recv(socket, buffer, sizeof(buffer) - 1, 0);

    if (received == -1) {
        LOG_ERROR("Erreur lors de la réception du message.");
    } else if (received == 0) {
        printf("Connexion fermée par l'interlocuteur");
    }

    buffer[received] = '\0';

    // Incorrect
    // Se servir de malloc (compliqué) vs mettre un buffer en argument de la fonction (embêtant)
    return buffer;
}


int init_server(uint16_t port) {
    // Utilisation, pour l'instant, du port par défaut
    Socket sock = create_socket(TCP, DEFAULT_PORT);
    sock.s_listen(sock.socket);
    sock.s_accept(sock.socket);

    return 1;
}

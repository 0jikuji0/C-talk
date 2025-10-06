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
#include <unistd.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERREUR] %s : %s (code: %d)\n", (msg), strerror(errno), errno)

int init_socket(enum Mode mode){
    if (mode != TCP && mode != UDP) {
        LOG_ERROR("Mode socket non supporté");
        exit(EXIT_FAILURE);
    }

    int sockfd = socket(AF_INET, mode, 0);
    if (sockfd == -1) {
        LOG_ERROR("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}

void param_socket(int socket) {
    int opt = 1;
    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) != 0) {
    	printf("Echec de paramètrage: %s\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }
}

void attach_address(int socket) {
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    // Ecoute sur toutes les adresses
    address.sin_addr.s_addr = INADDR_ANY;
    // Conversion du port en valeur réseaux (Host TO Network Short)
    address.sin_port = htons(8080);

    if (bind(socket, (struct sockaddr *) &address, sizeof(address)) != 0) {
    	printf("Echec d'attachement: %s\n", strerror(errno));
    	exit(EXIT_FAILURE);
    }
}

int create_socket(enum Mode mode) {
    int sock = init_socket(mode);
    param_socket(sock);
    attach_address(sock);

    return sock;
}


int init_server(uint16_t port) {
    int sock = create_socket(TCP);

    return 1;
}

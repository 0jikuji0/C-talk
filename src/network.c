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

#include "network.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERREUR] %s : %s (code: %d)\n", (msg), strerror(errno), errno)

int init_server(uint16_t port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        LOG_ERROR("Impossible de créer la socket");
        return -1;
    }
    // Suite de l'implémentation...
}
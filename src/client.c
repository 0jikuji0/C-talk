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

#include "client.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERREUR] %s : %s (code: %d)\n", (msg), strerror(errno), errno)

int connect_to_server(const char *ip, uint16_t port) {
    // Implémentation...
}
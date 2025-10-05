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

#include "server.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LOG_ERROR(msg) fprintf(stderr, "[ERREUR] %s : %s (code: %d)\n", (msg), strerror(errno), errno)

int init_server(uint16_t port) {
    // Implémentation...
}